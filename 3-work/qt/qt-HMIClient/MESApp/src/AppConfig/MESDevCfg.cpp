/*
 * MESDevCfg.cpp
 *
 *  Created on: 2015��2��5��
 *      Author: cj
 */

#include "MESDevCfg.h"
#include "string.h"

MESDevCfg::MESDevCfg()
{
	memset(version, 0, sizeof(version));
	memset(portCollect, 0, sizeof(portCollect));
	memset(CycleCollect, 0, sizeof(CycleCollect));
    memset(FunCollectEnable, 0, sizeof(FunCollectEnable));
}

bool MESDevCfg::write(const QString &path)
{
    //char version[6];	//�汾�ţ�6�ֽڵ���������(HEX)
    //char portCollect[20];	//�˿ڲɼ�˳��20�ֽڣ��ο���ϸ����ĵ��еġ����ݲɼ��˿ڶ��塱
    //char CycleCollect[20];	//���ڲɼ���ţ�20�ֽڣ������ֽ�Ϊһ�����ڵ���ʼ�ͽ�����ţ���������10��ɼ�����
//	Ŀǰ�����������ڣ�
//i.	�������ڣ�2�ֽڣ�
//ii.	������ڣ�2�ֽڣ�
//iii.	�������ڣ�2�ֽڣ�
//iv.	����������14�ֽڣ�
//char FunCollectEnable[20];//���ܲɼ�ʹ�ܣ�20�ֽڣ�8�ֽڶ�ӦIOģ��ÿ���˿ڵ�״̬��0Ϊ�رգ�����Ϊʹ�ܡ������ֽڱ�����

    QDateTime dateTime = QDateTime::currentDateTime();
    version[0] = dateTime.date().year() % 100;
    version[1] = dateTime.date().month();
    version[2] = dateTime.date().day();
    version[3] = dateTime.time().hour();
    version[4] = dateTime.time().minute();

    QFile fileRead(path);
    if (fileRead.open(QIODevice::WriteOnly))
    {
        QDataStream readDataStream(&fileRead);
        readDataStream.writeRawData((char*) this->version, sizeof(this->version));
        readDataStream.writeRawData((char*) this->portCollect, sizeof(this->portCollect));
        readDataStream.writeRawData((char*) this->CycleCollect, sizeof(this->CycleCollect));
        readDataStream.writeRawData((char*) this->FunCollectEnable, sizeof(this->FunCollectEnable));
        fileRead.close();
        return true;
    }
    return false;
}

bool MESDevCfg::read(const QString &path)
{
    //char version[6];	//�汾�ţ�6�ֽڵ���������(HEX)
    //char portCollect[20];	//�˿ڲɼ�˳��20�ֽڣ��ο���ϸ����ĵ��еġ����ݲɼ��˿ڶ��塱
    //char CycleCollect[20];	//���ڲɼ���ţ�20�ֽڣ������ֽ�Ϊһ�����ڵ���ʼ�ͽ�����ţ���������10��ɼ�����
//	Ŀǰ�����������ڣ�
//i.	�������ڣ�2�ֽڣ�
//ii.	������ڣ�2�ֽڣ�
//iii.	�������ڣ�2�ֽڣ�
//iv.	����������14�ֽڣ�
    //char FunCollectEnable[20];//���ܲɼ�ʹ�ܣ�20�ֽڣ�8�ֽڶ�ӦIOģ��ÿ���˿ڵ�״̬��0Ϊ�رգ�����Ϊʹ�ܡ������ֽڱ�����
//QString str=QCoreApplication::applicationDirPath()+ "/config_file/dev_cfg.bin";
    QFile fileRead(path);
    if (fileRead.open(QIODevice::ReadOnly))
    {
        QDataStream readDataStream(&fileRead);
        readDataStream.readRawData((char*) this->version, sizeof(this->version));
        readDataStream.readRawData((char*) this->portCollect,
                sizeof(this->portCollect));
        readDataStream.readRawData((char*) this->CycleCollect,
                sizeof(this->CycleCollect));
        readDataStream.readRawData((char*) this->FunCollectEnable,
                sizeof(this->FunCollectEnable));
        fileRead.close();
        return true;
    }
    return false;
}
