/*
 * rtmp_h264.c
 *
 *  Created on: 2020��11��9��
 *      Author: cc
 *
 *                .-~~~~~~~~~-._       _.-~~~~~~~~~-.
 *            __.'              ~.   .~              `.__
 *          .'//                  \./                  \\`.
 *        .'//                     |                     \\`.
 *      .'// .-~"""""""~~~~-._     |     _,-~~~~"""""""~-. \\`.
 *    .'//.-"                 `-.  |  .-'                 "-.\\`.
 *  .'//______.============-..   \ | /   ..-============.______\\`.
 *.'______________________________\|/______________________________`.
 *.'_________________________________________________________________`.
 * 
 * ��ע������ʽ
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rtmp_video.h"
#include "sps_decode.h"
#include "rtmp.h"
#include "amf.h"

#ifdef _DEBUG
FILE* netstackdump = NULL;
FILE* netstackdump_read = NULL;
#endif

#define MEDIA_DATA_H264 7
#define MEDIA_DATA_H265 12

int RTMP_ChangeChunkSize(RTMP *r, int outChunkSize);

int RTMP_SendMetaData(RTMP *rtmp, int code_id, int width, int height, int fps);

//����SPS PPS
int RTMP_SendVideoSpsPps(RTMP *rtmp,
						 uint32_t timestamp,
						 unsigned char *sps, int sps_len,
						 unsigned char *pps, int pps_len);

//����H264����
int RTMP_H264SendPacket(RTMP *rtmp,
						unsigned char *data, unsigned int size,
						int bIsKeyFrame,
						unsigned int nTimeStamp);

int H264_NALFindPos(uint8_t *data, int size, int *pnal_head, int *pnal_len)
{
	int pos = 0;
	int nal_head_flag = 0;
	int nal_split_pos = 0;
	int nal_head_pos = 0;
	int nal_len = 0;

	for (pos = 0; pos < size - 3; pos++)
	{
		if (pos < size - 4 && memcmp(data + pos, "\x00\x00\x00\x01", 4) == 0)
		{
			nal_head_flag = 1;
			nal_split_pos = pos;
			nal_head_pos = pos + 4;
			break;
		}
		if (pos < size - 3 && memcmp(data + pos, "\x00\x00\x01", 3) == 0)
		{
			nal_head_flag = 1;
			nal_split_pos = pos;
			nal_head_pos = pos + 3;
			break;
		}
	}
	if (!nal_head_flag)
		return -1;

	for (pos = nal_head_pos; pos < size; pos++)
	{
		if (pos < size - 4 && memcmp(data + pos, "\x00\x00\x00\x01", 4) == 0)
			break;
		if (pos < size - 3 && memcmp(data + pos, "\x00\x00\x01", 3) == 0)
			break;
	}

	nal_len = pos - nal_head_pos;

	if (pnal_head)
		*pnal_head = nal_head_pos;
	if (pnal_len)
		*pnal_len = nal_len;
	return nal_split_pos;
}

#define RTMP_HEAD_SIZE (sizeof(RTMPPacket) + RTMP_MAX_HEADER_SIZE)

char *put_byte(char *output, uint8_t nVal)
{
	output[0] = nVal;
	return output + 1;
}

char *put_be16(char *output, uint16_t nVal)
{
	output[1] = nVal & 0xff;
	output[0] = nVal >> 8;
	return output + 2;
}

char *put_be24(char *output, uint32_t nVal)
{
	output[2] = nVal & 0xff;
	output[1] = nVal >> 8;
	output[0] = nVal >> 16;
	return output + 3;
}

char *put_be32(char *output, uint32_t nVal)
{
	output[3] = nVal & 0xff;
	output[2] = nVal >> 8;
	output[1] = nVal >> 16;
	output[0] = nVal >> 24;
	return output + 4;
}

char *put_be64(char *output, uint64_t nVal)
{
	output = put_be32(output, nVal >> 32);
	output = put_be32(output, nVal);
	return output;
}

char *put_amf_string(char *c, const char *str)
{
	uint16_t len = strlen(str);
	c = put_be16(c, len);
	memcpy(c, str, len);
	return c + len;
}

char *put_amf_double(char *c, double d)
{
	*c++ = AMF_NUMBER; /* type: Number */
	{
		unsigned char *ci, *co;
		ci = (unsigned char *)&d;
		co = (unsigned char *)c;
		co[0] = ci[7];
		co[1] = ci[6];
		co[2] = ci[5];
		co[3] = ci[4];
		co[4] = ci[3];
		co[5] = ci[2];
		co[6] = ci[1];
		co[7] = ci[0];
	}
	return c + 8;
}

int SendPacket(RTMP *rtmp,
			   unsigned int nPacketType,
			   unsigned char *data, unsigned int size,
			   unsigned int nTimestamp)
{
	RTMPPacket *packet;
	packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE + size); /*������ڴ�ͳ�ʼ��,lenΪ���峤��*/
	memset(packet, 0, RTMP_HEAD_SIZE);
	packet->m_body = (char *)packet + RTMP_HEAD_SIZE; /*�����ڴ�*/
	packet->m_nBodySize = size;
	memcpy(packet->m_body, data, size);

	packet->m_hasAbsTimestamp = 0;
	packet->m_packetType = nPacketType; /*�˴�Ϊ����������һ������Ƶ,һ������Ƶ*/
	packet->m_nInfoField2 = rtmp->m_stream_id;
	packet->m_nChannel = 0x04;
	packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
	if (RTMP_PACKET_TYPE_AUDIO == nPacketType && size != 4)
	{
		packet->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
	}
	packet->m_nTimeStamp = nTimestamp;
	/*����*/
	int nRet = -1;
	if (RTMP_IsConnected(rtmp))
	{
		nRet = RTMP_SendPacket(rtmp, packet, FALSE); /*TRUEΪ�Ž����Ͷ���,FALSE�ǲ��Ž����Ͷ���,ֱ�ӷ���*/
		nRet = nRet == 1 ? 0 : -1;
	}
	/*�ͷ��ڴ�*/
	free(packet);
	return nRet;
}

//����������С�ĺ���
int RTMP_ChangeChunkSize(RTMP *r, int outChunkSize)
{
	RTMPPacket packet;
	char pbuf[RTMP_MAX_HEADER_SIZE + 4];

	packet.m_nBytesRead = 0;
	packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

	packet.m_packetType = RTMP_PACKET_TYPE_CHUNK_SIZE;
	packet.m_nChannel = 0x04;
	packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
	packet.m_nTimeStamp = 0;
	packet.m_nInfoField2 = 0;
	packet.m_hasAbsTimestamp = 0;
	packet.m_nBodySize = 4;
	r->m_outChunkSize = outChunkSize;

	r->m_outChunkSize = htonl(r->m_outChunkSize);

	memcpy(packet.m_body, &r->m_outChunkSize, 4);

	r->m_outChunkSize = ntohl(r->m_outChunkSize);
	return RTMP_SendPacket(r, &packet, TRUE);
}

int RTMP_SendMetaData(RTMP *rtmp, int code_id, int width, int height, int fps)
{
	RTMPPacket *packet = NULL; //rtmp���ṹ
	unsigned char *body = NULL;
	int i;
	packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE + 1024);
	packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
	body = (unsigned char *)packet->m_body;

	char *p = body;

	p = put_byte(p, AMF_STRING);
	p = put_amf_string(p, "@setDataFrame");

	p = put_byte(p, AMF_STRING);
	p = put_amf_string(p, "onMetaData");

	p = put_byte(p, AMF_OBJECT);
	//	p = put_amf_string(p, "copyright");
	//p = put_byte(p, AMF_STRING);
	//p = put_amf_string(p, "firehood");

	p = put_amf_string(p, "duration");
	p = put_amf_double(p, 0);

	p = put_amf_string(p, "width");
	p = put_amf_double(p, width);

	p = put_amf_string(p, "height");
	p = put_amf_double(p, height);

	//	p = put_amf_string(p, "framerate");
	//	p = put_amf_double(p, 25);

	p = put_amf_string(p, "videodatarate");
	p = put_amf_double(p, 0);

	p = put_amf_string(p, "videocodecid");
	p = put_amf_double(p, code_id); //FLV_CODECID_H264=7

	p = put_amf_string(p, "");
	p = put_byte(p, AMF_OBJECT_END);

	packet->m_packetType = RTMP_PACKET_TYPE_INFO; //0x12
	packet->m_nChannel = 0x04;
	packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
	packet->m_nTimeStamp = 0;
	packet->m_nInfoField2 = rtmp->m_stream_id;
	packet->m_nBodySize = p - packet->m_body;

	//printf("m_nBodySize=%d\n", packet->m_nBodySize);
	/*���÷��ͽӿ�*/
	int nRet = RTMP_SendPacket(rtmp, packet, TRUE);
	nRet = nRet == 1 ? 0 : -1;
	free(packet); //�ͷ��ڴ�
	return nRet;
}

/**
 * ������Ƶ��sps��pps��Ϣ
 *
 * @param pps �洢��Ƶ��pps��Ϣ
 * @param pps_len ��Ƶ��pps��Ϣ����
 * @param sps �洢��Ƶ��pps��Ϣ
 * @param sps_len ��Ƶ��sps��Ϣ����
 *
 * @�ɹ��򷵻� 1 , ʧ���򷵻�0
 */
int RTMP_SendVideoSpsPps(RTMP *rtmp,
						 uint32_t timestamp,
						 unsigned char *sps, int sps_len,
						 unsigned char *pps, int pps_len)
{
	RTMPPacket *packet = NULL; //rtmp���ṹ
	unsigned char *body = NULL;
	int i;
	packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE + 1024);
	//RTMPPacket_Reset(packet);//����packet״̬
	memset(packet, 0, RTMP_HEAD_SIZE + 1024);
	packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
	body = (unsigned char *)packet->m_body;
	i = 0;
	//
	body[i++] = 0x17;
	body[i++] = 0x00;
	body[i++] = 0x00;
	body[i++] = 0x00;
	body[i++] = 0x00;

	/*AVCDecoderConfigurationRecord*/
	body[i++] = 0x01;
	body[i++] = sps[1];
	body[i++] = sps[2];
	body[i++] = sps[3];
	body[i++] = 0xff;

	/*sps*/
	body[i++] = 0xe1;
	body[i++] = (sps_len >> 8) & 0xff;
	body[i++] = sps_len & 0xff;
	memcpy(&body[i], sps, sps_len);
	i += sps_len;

	/*pps*/
	body[i++] = 0x01;
	body[i++] = (pps_len >> 8) & 0xff;
	body[i++] = (pps_len)&0xff;
	memcpy(&body[i], pps, pps_len);
	i += pps_len;

	packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
	packet->m_nBodySize = i;
	packet->m_nChannel = 0x04;
	packet->m_nTimeStamp = timestamp & 0xffffff;
	packet->m_hasAbsTimestamp = 0;
	packet->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
	packet->m_nInfoField2 = rtmp->m_stream_id;

	//printf("sps bps body size:%d\n", i);
	/*���÷��ͽӿ�*/
	int nRet = RTMP_SendPacket(rtmp, packet, TRUE);
	nRet = nRet == 1 ? 0 : -1;
	free(packet); //�ͷ��ڴ�
	return nRet;
}

int RTMP_H264SendPacket(RTMP *rtmp, unsigned char *data, unsigned int size,
						int bIsKeyFrame,
						unsigned int nTimeStamp)
{
	if (data == NULL && size < 11)
		return -1;

	RTMPPacket *packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE + size + 9);
	memset(packet, 0, RTMP_HEAD_SIZE);
	RTMPPacket_Reset(packet);
	packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
	packet->m_nBodySize = size + 9;

	unsigned char *body = packet->m_body;
	memset(body, 0, size + 9);

	int i = 0;
	if (bIsKeyFrame)
		body[i++] = 0x17; // 1:Iframe  7:AVC
	else
		body[i++] = 0x27; // 2:Pframe  7:AVC
	body[i++] = 0x01;	  // AVC NALU
	body[i++] = 0x00;
	body[i++] = 0x00;
	body[i++] = 0x00;

	// NALU size
	body[i++] = size >> 24 & 0xff;
	body[i++] = size >> 16 & 0xff;
	body[i++] = size >> 8 & 0xff;
	body[i++] = size & 0xff;

	// NALU data
	memcpy(&body[i], data, size);
	int ret = -1;

	packet->m_hasAbsTimestamp = 0;
	packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
	packet->m_nInfoField2 = rtmp->m_stream_id;
	packet->m_nChannel = 0x04;
	packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
	packet->m_nTimeStamp = nTimeStamp & 0xffffff;

	if (RTMP_IsConnected(rtmp))
	{
		ret = RTMP_SendPacket(rtmp, packet, TRUE); // return 1 success; -1 err;
		ret = ret == 1 ? 0 : -1;
	}
	free(packet);
	return ret;
}

#define BODY_0_H265 0x0C
typedef struct HEVCDecoderConfigurationRecord
{
	uint8_t configurationVersion;
	uint8_t general_profile_space;
	uint8_t general_tier_flag;
	uint8_t general_profile_idc;
	uint32_t general_profile_compatibility_flags;
	uint64_t general_constraint_indicator_flags;
	uint8_t general_level_idc;
	uint16_t min_spatial_segmentation_idc;
	uint8_t parallelismType;
	uint8_t chromaFormat;
	uint8_t bitDepthLumaMinus8;
	uint8_t bitDepthChromaMinus8;
	uint16_t avgFrameRate;
	uint8_t constantFrameRate;
	uint8_t numTemporalLayers;
	uint8_t temporalIdNested;
	uint8_t lengthSizeMinusOne;
	uint8_t numOfArrays;
	//    HVCCNALUnitArray *array;
} HEVCDecoderConfigurationRecord;

/**
 * ������Ƶ��sps��pps��vps��Ϣ
 *
 * @param pps �洢��Ƶ��pps��Ϣ
 * @param pps_len ��Ƶ��pps��Ϣ����
 * @param sps �洢��Ƶ��pps��Ϣ
 * @param sps_len ��Ƶ��sps��Ϣ����
 * @param vps �洢��Ƶ��vps��Ϣ
 * @param vps_len ��Ƶ��vps��Ϣ����
 *
 * @�ɹ��򷵻� 1 , ʧ���򷵻�0
 */
int RTMP_SendVideoSpsPpsVps(RTMP *rtmp,
							uint32_t timestamp,
							unsigned char *pps, int pps_len,
							unsigned char *sps, int sps_len,
							unsigned char *vps, int vps_len)
{
	RTMPPacket *packet = NULL; //rtmp���ṹ
	unsigned char *body = NULL;
	int i;
	packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE + 1024);
	//RTMPPacket_Reset(packet);//����packet״̬
	memset(packet, 0, RTMP_HEAD_SIZE + 1024);
	packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
	body = (unsigned char *)packet->m_body;

	i = 0;
	body[i++] = 0x10 | BODY_0_H265;

	//          4 | HEVCDecoderConfigurationRecord������С����23�ֽڣ�
	//            | 01 3  4  5  6  8  9  10             17
	//00 00 00 00 | 01 01 60 00 00 00 b0 00 00 00 00 00 7b f0 00 fc fd f8 f8 00 00 0f 03
	// SPS:  4201 | 01 01 60 00 00 03 00 b0 00 00 03 00 00 03 00 7b a0 03 c0 80 10 e5 8d ae 49 14 bf 37 01 01 01 00 80
	//        42 01 01 01 60 00 00 00 b0 00 00 00 00 00 5d a0 02 80 80 2d 16 36 b9 24 52 fc dc 04 04 04 02 02 02 02
	//       0  1    2 3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
	body[i++] = 0x00; // AVC sequence header   1byte
	body[i++] = 0x00; //composition time 3 byte
	body[i++] = 0x00;
	body[i++] = 0x00;

	body[i++] = 0x01;
	/*
	 * unsigned int(2) general_profile_space;
	 * unsigned int(1) general_tier_flag;
	 * unsigned int(5) general_profile_idc;
	 */
	body[i++] = sps[3];
	body[i++] = sps[4]; /* unsigned int(32) general_profile_compatibility_flags; */ //0x60
	body[i++] = sps[5];
	body[i++] = sps[6];
	body[i++] = 00;

	body[i++] = sps[8]; /* unsigned int(48) general_constraint_indicator_flags; */
	body[i++] = sps[9];
	body[i++] = sps[10];
	body[i++] = 0;
	body[i++] = 0;
	body[i++] = 0;
	body[i++] = sps[17]; /* unsigned int(8) general_level_idc; */
	/*
	 * bit(4) reserved = ��1111��b;
	 * unsigned int(12) min_spatial_segmentation_idc;
	 */
	body[i++] = 0xF0;
	body[i++] = 0;
	/*
	 * bit(6) reserved = ��111111��b;
	 * unsigned int(2) parallelismType;
	 */
	body[i++] = 0xFC;
	/*
	 * bit(6) reserved = ��111111��b;
	 * unsigned int(2) chromaFormat;
	 */
	body[i++] = 0xFD;
	/*
	 * bit(5) reserved = ��11111��b;
	 * unsigned int(3) bitDepthLumaMinus8;
	 */
	body[i++] = 0xF8;
	/*
	 * bit(5) reserved = ��11111��b;
	 * unsigned int(3) bitDepthChromaMinus8;
	 */
	body[i++] = 0xF8;
	/* bit(16) avgFrameRate; */
	body[i++] = 0x00;
	body[i++] = 0x00;
	/*
	 * bit(2) constantFrameRate;
	 * bit(3) numTemporalLayers;
	 * bit(1) temporalIdNested;
	 * unsigned int(2) lengthSizeMinusOne;
	 */
	body[i++] = 0x0F;
	/* unsigned int(8) numOfArrays; */
	body[i++] = 0x03;

	// VPS
	body[i++] = 0x20;
	body[i++] = (1 >> 8) & 0xff;
	body[i++] = 1 & 0xff;
	body[i++] = (vps_len >> 8) & 0xff;
	body[i++] = (vps_len)&0xff;
	memcpy(&body[i], vps, vps_len);
	i += vps_len;

	// sps
	body[i++] = 0x21;
	body[i++] = (1 >> 8) & 0xff;
	body[i++] = 1 & 0xff;
	body[i++] = (sps_len >> 8) & 0xff;
	body[i++] = sps_len & 0xff;
	memcpy(&body[i], sps, sps_len);
	i += sps_len;

	// pps
	pps_len++;
	body[i++] = 0x22;
	body[i++] = (1 >> 8) & 0xff;
	body[i++] = 1 & 0xff;
	body[i++] = (pps_len >> 8) & 0xff;
	body[i++] = (pps_len)&0xff;
	memcpy(&body[i], pps, pps_len - 1);
	body[i + pps_len - 1] = 0;
	i += pps_len;

	packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
	packet->m_nBodySize = i;
	packet->m_nChannel = 0x04;
	packet->m_nTimeStamp = timestamp & 0xffffff;
	packet->m_hasAbsTimestamp = 0;
	packet->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
	packet->m_nInfoField2 = rtmp->m_stream_id;

	//	printf("[%d]SPS/PPS/VPS =%d\n", time(NULL), i);
	//	for (int j = 0; j < i; j++)
	//	{
	//		printf("%02x ", body[j]);
	//	}
	//	printf("\n");

	/*���÷��ͽӿ�*/
	int nRet = RTMP_SendPacket(rtmp, packet, TRUE);
	nRet = nRet == 1 ? 0 : -1;
	free(packet); //�ͷ��ڴ�
	return nRet;
}

/**
 * ����H265����֡
 *
 * @param data �洢����֡����
 * @param size ����֡�Ĵ�С
 * @param bIsKeyFrame ��¼��֡�Ƿ�Ϊ�ؼ�֡
 * @param nTimeStamp ��ǰ֡��ʱ���
 *
 * @�ɹ��򷵻� 1 , ʧ���򷵻�0
 */

int RTMP_SendH265Packet(RTMP *rtmp, struct media_data *mdata,
						unsigned char *data, unsigned int size,
						int bIsKeyFrame, unsigned int nTimeStamp)
{
	if (data == NULL && size < 11)
		return -1;

	if (bIsKeyFrame)
		RTMP_SendVideoSpsPpsVps(rtmp, nTimeStamp, mdata->pps, mdata->pps_len, mdata->sps, mdata->sps_len, mdata->vps,
								mdata->vps_len);

	RTMPPacket *packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE + size + 9);
	memset(packet, 0, RTMP_HEAD_SIZE);
	RTMPPacket_Reset(packet);
	packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
	packet->m_nBodySize = size + 9;

	unsigned char *body = packet->m_body;
	memset(body, 0, size + 9);

	int i = 0;
	if (bIsKeyFrame)
		body[i++] = 0x10 | BODY_0_H265; // 1:Iframe
	else
		body[i++] = 0x20 | BODY_0_H265; // 2:Pframe
	body[i++] = 0x01;					// AVC NALU
	body[i++] = 0x00;
	body[i++] = 0x00;
	body[i++] = 0x00;

	// NALU size
	body[i++] = size >> 24 & 0xff;
	body[i++] = size >> 16 & 0xff;
	body[i++] = size >> 8 & 0xff;
	body[i++] = size & 0xff;

	// NALU data
	memcpy(&body[i], data, size);
	int ret = -1;

	packet->m_hasAbsTimestamp = 0;
	packet->m_packetType = RTMP_PACKET_TYPE_VIDEO; //0x09
	packet->m_nInfoField2 = rtmp->m_stream_id;
	packet->m_nChannel = 0x04;
	packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
	packet->m_nTimeStamp = nTimeStamp & 0xffffff;

	//	printf("[%s] len=%d\n", bIsKeyFrame ? "IDR" : "R", packet->m_nBodySize);
	//	for (i = 0; i < 10; i++)
	//		printf("%02X ", data[i]);
	//	printf("\n");

	if (RTMP_IsConnected(rtmp))
	{
		ret = RTMP_SendPacket(rtmp, packet, TRUE);
		ret = ret == 1 ? 0 : -1;
	}
	free(packet);
	return ret;
}

void RTMPStreamOut_close(struct RTMPStreamOut *out)
{
	if (out->rtmp)
	{
		RTMP_Close(out->rtmp);
		RTMP_Free(out->rtmp);
		out->rtmp = NULL;
	}
	if (out->mdata.sps)
		free(out->mdata.sps);
	if (out->mdata.pps)
		free(out->mdata.pps);
	memset(&out->mdata, 0, sizeof(struct media_data));
	out->isopen = 0;
	out->frame_sps_exists = 0;
}

void RTMPStreamOut_setfps(RTMPOut *out, int fps)
{
	if (out)
		out->mdata.fps = fps;
}

int RTMPStreamOut_open(struct RTMPStreamOut *out, const char *url)
{
	RTMP *rtmp = NULL;
	RTMPPacket *packet = NULL;

	out->isopen = 0;
	rtmp = RTMP_Alloc();
	if (!rtmp)
	{
		fprintf(stderr, RED "RTMP_Alloc failed\n" NONE);
		return -1;
	}
	strcpy(out->url, url);
	//��ʼ���ṹ�塰RTMP���еĳ�Ա����
	RTMP_Init(rtmp);
	rtmp->Link.timeout = 5;
	if (!RTMP_SetupURL(rtmp, out->url))
	{
		fprintf(stderr, RED "RTMP_SetupURL failed\n" NONE);
		RTMP_Free(rtmp);
		return -1;
	}
	RTMP_SetBufferMS(rtmp, 3600 * 1000);

	//��������ʱ�����Ҫʹ�á������ʹ��������������
	RTMP_EnableWrite(rtmp);
	//����RTMP���ӣ�����һ��RTMPЭ��淶�е�NetConnection
	if (!RTMP_Connect(rtmp, NULL))
	{
		fprintf(stderr, RED "RTMP_Connect failed\n" NONE);
		RTMP_Free(rtmp);
		return -1;
	}
	//����һ��RTMPЭ��淶�е�NetStream
	if (!RTMP_ConnectStream(rtmp, 0))
	{
		fprintf(stderr, RED "RTMP_ConnectStream failed\n" NONE);
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		return -1;
	}
	out->rtmp = rtmp;
	out->isopen = 1;
	out->frame_sps_exists = 0;
	return 0;
}

static void log_print_hex(void *ptr, int olen, FILE *fp, int maxlen)
{
	int i = 0;
	for (i = 0; i < olen; i++)
	{
		fprintf(fp, "%02X ", ((uint8_t *)ptr)[i]);
	}
	fprintf(fp, "\n");
}

static int _RTMP_H264SendPacket(RTMP *rtmp, struct media_data *mdata, uint8_t *data, int size, int ms)
{
	// ��ȡSPS֡
	// ��ȡPPS֡
	// ����SPS,��ȡ��Ƶͼ�������Ϣ
	//databuff_out_hex((uint8_t*) data, size, stdout, 16 * 5);
	int nal_head_pos = 0;
	int nal_split_pos = 0;
	int nal_len = 0;
	uint8_t *ptr = data;
	int len = size;
	int ret = 0;

	do
	{
		nal_split_pos = H264_NALFindPos(ptr, len, &nal_head_pos, &nal_len);
		if (nal_split_pos == -1)
			break;
		uint8_t type = ptr[nal_head_pos];
		H264_NALU_HEADER header;

		header.forbidden_bit = (type & 0x80) >> 7;
		header.nal_reference_idc = (type & 0x60) >> 5;
		header.nal_unit_type = (type & 0x1F);

		//printf(">%d,NAL:(%02X) %d, %d, %d: len=%d\n", nal_head_pos,
		//			ptr[nal_head_pos], header.forbidden_bit, header.nal_reference_idc, header.nal_unit_type, nal_len);

		switch (header.nal_unit_type)
		{
		case NALU_TYPE_SPS: //7
		{
			//printf("SPS:\n");
			// ����SPS,��ȡ��Ƶͼ�������Ϣ
			//log_print_hex(ptr + nal_head_pos, nal_len, stdout, nal_len);
			int width = 0, height = 0, fps = 0;

			h264_decode_sps(ptr + nal_head_pos, nal_len, &width, &height, &fps);
			if (mdata->width != 0 && mdata->height != 0 && mdata->width != width && mdata->height != height)
			{
				mdata->status = 0;
				printf("�ֱ����б仯:%dx%d\n", width, height);
			}
			mdata->width = width;
			mdata->height = height;
			//mdata->fps = fps;
			if (mdata->sps)
				free(mdata->sps);

			mdata->sps = (char *)malloc(nal_len);
			mdata->sps_len = nal_len;
			memcpy(mdata->sps, ptr + nal_head_pos, mdata->sps_len);
		}
		break;
		case NALU_TYPE_PPS: //8
		{
			//printf("PPS\n");
			if (mdata->pps)
				free(mdata->pps);
			mdata->pps = (char *)malloc(nal_len);
			mdata->pps_len = nal_len;
			memcpy(mdata->pps, ptr + nal_head_pos, mdata->pps_len);
			//				log_print_hex(mdata->pps, mdata->pps_len, stdout, mdata->pps_len);
		}
		break;
		case NALU_TYPE_IDR: //5 I֡
		{
			//printf("IDR\n");
			if (mdata->status == 0)
			{
				printf("RTMP_SendMetaData: width:%d, height:%d fps:%d\n", mdata->width,
					   mdata->height,
					   mdata->fps);
				ret = RTMP_SendMetaData(rtmp,
										MEDIA_DATA_H264,
										mdata->width,
										mdata->height,
										mdata->fps);
				printf("RTMP_SendVideoSpsPps: sps=%d, pps=%d\n", mdata->sps_len, mdata->pps_len);
				ret = RTMP_SendVideoSpsPps(rtmp, 0,
										   mdata->sps,
										   mdata->sps_len,
										   mdata->pps,
										   mdata->pps_len);
				mdata->status = 1;
			}
			ret = RTMP_H264SendPacket(rtmp, ptr + nal_head_pos, nal_len, 1, mdata->nTimeStamp);
			mdata->nTimeStamp += ms;
			if (ret == -1)
				return ret;
		}
		break;
		case NALU_TYPE_SLICE: //1
		{
			//printf("SLICE\n");
			ret = RTMP_H264SendPacket(rtmp, ptr + nal_head_pos, nal_len, 0, mdata->nTimeStamp);
			mdata->nTimeStamp += ms;
			if (ret == -1)
				return ret;
		}
		break;
		case NALU_TYPE_SEI: //������ǿ��Ϣ
		{
			//printf("SEI\n");
			//ret = RTMP_H264SendPacket(rtmp, ptr + nal_head_pos, nal_len, 1, nTimeStamp);
			//if (ret == -1)
			//	return ret;
		}
		break;
		default:
		{
			printf("errr type=%d \n", header.nal_unit_type);
		}
		break;
		}

		ptr += nal_head_pos + nal_len;
		len -= nal_len + (nal_head_pos - nal_split_pos);
	} while (1);

	//mdata->nTimeStamp += ms;				// 1000 / 25; //��Ҫ֪������ms
	return ret;
}

int RTMPStreamOut_Send(RTMPOut *out, uint8_t *data, int size, int ms)
{ //��֡����ΪSPS
	int ret;
	int nal_split_pos = 0;
	int nal_head_pos = 0;
	int nal_len = 0;
	if (!out)
		return -1;
	if (!out->isopen)
		return -1;

	nal_split_pos = H264_NALFindPos(data, size, &nal_head_pos, &nal_len);
	int nal_unit_type = (data[nal_head_pos] & 0x1F);
	if (nal_unit_type == NALU_TYPE_SPS) //��֡ SPS/PPS/I
		out->frame_sps_exists = 1;

	if (out->frame_sps_exists)
	{
		ret = _RTMP_H264SendPacket(out->rtmp, &out->mdata, data, size, ms); //Ӧ��Я��fps?
		return ret;
	}
	return RTMP_IsConnected(out->rtmp) ? 0 : -1;
}

const char *HevcNaluType_ToString(int type)
{
	switch (type)
	{
	case NAL_UNIT_CODED_SLICE_TRAIL_N:
		return "NAL_UNIT_CODED_SLICE_TRAIL_N";
#define CASE_ITEM(v) \
	case v:          \
		return #v;

		CASE_ITEM(NAL_UNIT_CODED_SLICE_TRAIL_R)

		CASE_ITEM(NAL_UNIT_CODED_SLICE_TSA_N) // 2
		CASE_ITEM(NAL_UNIT_CODED_SLICE_TLA)	  // 3   // Current name in the spec: TSA_R

		CASE_ITEM(NAL_UNIT_CODED_SLICE_STSA_N) // 4
		CASE_ITEM(NAL_UNIT_CODED_SLICE_STSA_R) // 5

		CASE_ITEM(NAL_UNIT_CODED_SLICE_RADL_N) // 6
		CASE_ITEM(NAL_UNIT_CODED_SLICE_DLP)	   // 7 // Current name in the spec: RADL_R

		CASE_ITEM(NAL_UNIT_CODED_SLICE_RASL_N) // 8
		CASE_ITEM(NAL_UNIT_CODED_SLICE_TFD)	   // 9 // Current name in the spec: RASL_R

		CASE_ITEM(NAL_UNIT_RESERVED_10)
		CASE_ITEM(NAL_UNIT_RESERVED_11)
		CASE_ITEM(NAL_UNIT_RESERVED_12)
		CASE_ITEM(NAL_UNIT_RESERVED_13)
		CASE_ITEM(NAL_UNIT_RESERVED_14)
		CASE_ITEM(NAL_UNIT_RESERVED_15)
		CASE_ITEM(NAL_UNIT_CODED_SLICE_BLA)		 // 16   // Current name in the spec: BLA_W_LP
		CASE_ITEM(NAL_UNIT_CODED_SLICE_BLANT)	 // 17   // Current name in the spec: BLA_W_DLP
		CASE_ITEM(NAL_UNIT_CODED_SLICE_BLA_N_LP) // 18
		CASE_ITEM(NAL_UNIT_CODED_SLICE_IDR)		 // 19  // Current name in the spec: IDR_W_DLP
		CASE_ITEM(NAL_UNIT_CODED_SLICE_IDR_N_LP) // 20
		CASE_ITEM(NAL_UNIT_CODED_SLICE_CRA)		 // 21
		CASE_ITEM(NAL_UNIT_RESERVED_22)
		CASE_ITEM(NAL_UNIT_RESERVED_23)

		CASE_ITEM(NAL_UNIT_RESERVED_24)
		CASE_ITEM(NAL_UNIT_RESERVED_25)
		CASE_ITEM(NAL_UNIT_RESERVED_26)
		CASE_ITEM(NAL_UNIT_RESERVED_27)
		CASE_ITEM(NAL_UNIT_RESERVED_28)
		CASE_ITEM(NAL_UNIT_RESERVED_29)
		CASE_ITEM(NAL_UNIT_RESERVED_30)
		CASE_ITEM(NAL_UNIT_RESERVED_31)
		CASE_ITEM(NAL_UNIT_VPS)					  // 32
		CASE_ITEM(NAL_UNIT_SPS)					  // 33
		CASE_ITEM(NAL_UNIT_PPS)					  // 34
		CASE_ITEM(NAL_UNIT_ACCESS_UNIT_DELIMITER) // 35
		CASE_ITEM(NAL_UNIT_EOS)					  // 36
		CASE_ITEM(NAL_UNIT_EOB)					  // 37
		CASE_ITEM(NAL_UNIT_FILLER_DATA)			  // 38
		CASE_ITEM(NAL_UNIT_SEI)					  // 39 Prefix SEI
		CASE_ITEM(NAL_UNIT_SEI_SUFFIX)			  // 40 Suffix SEI
		CASE_ITEM(NAL_UNIT_RESERVED_41)
		CASE_ITEM(NAL_UNIT_RESERVED_42)
		CASE_ITEM(NAL_UNIT_RESERVED_43)
		CASE_ITEM(NAL_UNIT_RESERVED_44)
		CASE_ITEM(NAL_UNIT_RESERVED_45)
		CASE_ITEM(NAL_UNIT_RESERVED_46)
		CASE_ITEM(NAL_UNIT_RESERVED_47)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_48)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_49)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_50)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_51)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_52)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_53)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_54)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_55)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_56)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_57)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_58)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_59)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_60)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_61)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_62)
		CASE_ITEM(NAL_UNIT_UNSPECIFIED_63)
		CASE_ITEM(NAL_UNIT_INVALID)
	}
	return "unknow";
}

int _RTMP_H265SendPacket(RTMP *rtmp, struct media_data *mdata, uint8_t *data, int size, int ms)
{
	int nal_head_pos = 0;
	int nal_split_pos = 0;
	int nal_len = 0;
	uint8_t *ptr = data;
	int len = size;
	int ret = 0;

	do
	{
		nal_split_pos = H264_NALFindPos(ptr, len, &nal_head_pos, &nal_len);
		if (nal_split_pos == -1)
			break;

		int nal_unit_type = (ptr[nal_head_pos] & 0x7E) >> 1;

		//		printf(">%d,NAL:(%02X) type=%d(%s), len=%d\n", nal_head_pos,
		//					ptr[nal_head_pos],
		//					nal_unit_type,
		//					HevcNaluType_ToString(nal_unit_type),
		//					nal_len);

		switch (nal_unit_type)
		{
		case NAL_UNIT_SPS: //7
		{
			//printf("SPS\n");
			// ����SPS,��ȡ��Ƶͼ�������Ϣ
			//log_print_hex(ptr + nal_head_pos, nal_len, stdout, nal_len);
			int width = 0, height = 0, fps = 0;

			if (mdata->sps)
				free(mdata->sps);

			mdata->sps = (unsigned char *)malloc(nal_len);
			mdata->sps_len = nal_len;
			memcpy(mdata->sps, ptr + nal_head_pos, mdata->sps_len);

			h265_decode_sps(ptr + nal_head_pos, nal_len, &width, &height, &fps);

			if (mdata->width != 0 && mdata->height != 0 && mdata->width != width && mdata->height != height)
			{
				mdata->status = 0;
				printf("SPS�ֱ����б仯:%dx%d\n", width, height);
			}
			mdata->width = width;
			mdata->height = height;
		}
		break;
		case NAL_UNIT_PPS: //8
		{
			//				printf("PPS\n");
			if (mdata->pps)
				free(mdata->pps);
			mdata->pps = (unsigned char *)malloc(nal_len);
			mdata->pps_len = nal_len;
			memcpy(mdata->pps, ptr + nal_head_pos, mdata->pps_len);
			//				log_print_hex(mdata->pps, mdata->pps_len, stdout, mdata->pps_len);
		}
		break;
		case NAL_UNIT_VPS:
		{
			//				printf("VPS\n");
			if (mdata->vps)
				free(mdata->vps);
			mdata->vps = (unsigned char *)malloc(nal_len);
			mdata->vps_len = nal_len;
			memcpy(mdata->vps, ptr + nal_head_pos, mdata->vps_len);
		}
		break;
		case NAL_UNIT_CODED_SLICE_IDR:
		case NAL_UNIT_CODED_SLICE_IDR_N_LP:
		case NAL_UNIT_CODED_SLICE_TRAIL_R:
		case NAL_UNIT_CODED_SLICE_BLA:
		case NAL_UNIT_CODED_SLICE_BLA_N_LP:
		{
			int iskey = (nal_unit_type == NAL_UNIT_CODED_SLICE_IDR || nal_unit_type == NAL_UNIT_CODED_SLICE_IDR_N_LP) ? 1 : 0;

			if (iskey && mdata->status == 0)
			{
				printf("RTMP_SendMetaData: width:%d, height:%d fps:%d\n", mdata->width,
					mdata->height,
					mdata->fps);
				RTMP_SendMetaData(rtmp, MEDIA_DATA_H265, mdata->width, mdata->height, mdata->fps);
				mdata->status = 1;
			}

			ret = RTMP_SendH265Packet(rtmp, mdata,
									  ptr + nal_head_pos, nal_len,
									  iskey, mdata->nTimeStamp);
			mdata->nTimeStamp += ms;
			if (ret == -1)
				return ret;
		}
		break;
		case NAL_UNIT_SEI:
			break;
		default:
		{
			printf("unknow send nalu type [%s]%d...\n", HevcNaluType_ToString(nal_unit_type), nal_unit_type);
		}
		break;
		}

		ptr += nal_head_pos + nal_len;
		len -= nal_len + (nal_head_pos - nal_split_pos);
	} while (1);
	return ret;
}

int RTMPStreamOut_SendH265(RTMPOut *out, uint8_t *data, int size, int ms)
{
	//VPS/SPS/PPS/SEI/IDR
	int ret;
	int nal_split_pos = 0;
	int nal_head_pos = 0;
	int nal_len = 0;
	if (!out)
		return -1;

	if (!out->isopen)
		return -1;

	nal_split_pos = H264_NALFindPos(data, size, &nal_head_pos, &nal_len);
	int nal_unit_type = (data[nal_head_pos] & 0x7E) >> 1;

	if (nal_unit_type == NAL_UNIT_VPS) //��֡ SPS/PPS/I
		out->frame_sps_exists = 1;

	if (out->frame_sps_exists)
	{
		ret = _RTMP_H265SendPacket(out->rtmp, &out->mdata, data, size, ms); //Ӧ��Я��fps?
		return ret;
	}
	return RTMP_IsConnected(out->rtmp) ? 0 : -1;
}
