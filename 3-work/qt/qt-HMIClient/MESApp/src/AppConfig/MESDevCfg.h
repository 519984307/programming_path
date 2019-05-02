/*
 * MESDevCfg.h
 *
 *  Created on: 2015��2��5��
 *      Author: cj
 */

#ifndef MESDEVCFG_H_
#define MESDEVCFG_H_

#include <QtCore>

/*
 *�豸�����ļ�
 */
class MESDevCfg
{
//	�汾�ţ�6�ֽڵ���������(HEX)
//	�˿ڲɼ�˳��20�ֽڣ��ο���ϸ����ĵ��еġ����ݲɼ��˿ڶ��塱
//	���ڲɼ���ţ�20�ֽڣ������ֽ�Ϊһ�����ڵ���ʼ�ͽ�����ţ���������10��ɼ�����
//  1 2 3 4 5 6

//  1 6 3 5 1 1
//	                       Ŀǰ�����������ڣ�
//	i.	�������ڣ�2�ֽڣ�
//	ii.	������ڣ�2�ֽڣ�
//	iii.	�������ڣ�2�ֽڣ�
//	iv.	����������14�ֽڣ�
//	���ܲɼ�ʹ�ܣ�20�ֽڣ�8�ֽڶ�ӦIOģ��ÿ���˿ڵ�״̬��0Ϊ�رգ�����Ϊʹ�ܡ������ֽڱ�����
public:
	char version[6];	//�汾�ţ�6�ֽڵ���������(HEX)
	unsigned char portCollect[20];	//�˿ڲɼ�˳��20�ֽڣ��ο���ϸ����ĵ��еġ����ݲɼ��˿ڶ��塱
	unsigned char CycleCollect[20];	//���ڲɼ���ţ�20�ֽڣ������ֽ�Ϊһ�����ڵ���ʼ�ͽ�����ţ���������10��ɼ�����
	//	Ŀǰ�����������ڣ�
	//i.	�������ڣ�2�ֽڣ�The machine cycle
	//ii.	������ڣ�2�ֽڣ� Fill
	//iii.	�������ڣ�2�ֽڣ�Forming
	//iv.	����������14�ֽڣ�Other
	unsigned char FunCollectEnable[20];//���ܲɼ�ʹ�ܣ�20�ֽڣ�8�ֽڶ�ӦIOģ��ÿ���˿ڵ�״̬��0Ϊ�رգ�����Ϊʹ�ܡ������ֽڱ�����

	MESDevCfg();
	//��������Start
	unsigned char getCycleMachineStart() const
	{
		return CycleCollect[0];
	}
	//��������end
	unsigned char getCycleMachineEnd() const
	{
		return CycleCollect[1];
	}
	//�������Start
	unsigned char getCycleFillStart() const
	{
		return CycleCollect[2];
	}
	//�������end
	unsigned char getCycleFillEnd() const
	{
		return CycleCollect[3];
	}
	//��������Start
	unsigned char getCycleFormingStart() const
	{
		return CycleCollect[4];
	}
	//��������End
	unsigned char getCycleFormingEnd() const
	{
		return CycleCollect[5];
	}

    virtual bool write(const QString &path); //����д�뵽�ļ�
    virtual bool read(const QString &path); //���ô��ļ���ȡ
};

#endif /* MESDEVCFG_H_ */
