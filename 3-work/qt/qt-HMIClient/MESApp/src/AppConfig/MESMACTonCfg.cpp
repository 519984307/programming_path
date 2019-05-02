/*
 * MESMachineTon.cpp
 *
 *  Created on: 2015��2��2��
 *      Author: cj
 */

#include "MESMACTonCfg.h"



bool MESMACTonCfg::write(const QString &path)
{
    return false;
}

bool MESMACTonCfg::read(const QString &path)
{
    //char version[6]; //	�汾�ţ�6�ֽڵ���������(HEX)
    quint16 macSize; //	����������2�ֽڣ�n��
    QFile fileRead(path);

    this->TonList.clear();
    if (fileRead.open(QIODevice::ReadOnly))
    {
        QDataStream readDataStream(&fileRead);
        readDataStream.readRawData(this->version, sizeof(this->version));
        readDataStream.readRawData((char*) &macSize, sizeof(macSize));

        this->macSize = macSize;

        for (int i = 0; i < macSize; i++)
        {
            quint16 index; //��ţ�2�ֽ�
            char number[20]; //������ţ�20�ֽ�
            quint32 ton; //��λֵ��4�ֽ�
            char ipAddress[16]; //15
            char machinoBrand[30];
//			��ţ�2�ֽ�
//			������ţ�20�ֽ�
//			��λֵ��4�ֽ�
//			IP��ַ��15�ֽ�
//			����Ʒ�ƣ�30�ֽ�
            memset(ipAddress, 0, 16);
            readDataStream.readRawData((char*) &index, sizeof(index));
            readDataStream.readRawData(number, sizeof(number));
            readDataStream.readRawData((char*) &ton, sizeof(ton));
            readDataStream.readRawData(ipAddress, 15);
            readDataStream.readRawData(machinoBrand, sizeof(machinoBrand));

            this->TonList.append(
                    MESMACTonCfg::TonInfo(index, number, ipAddress,
                            machinoBrand, ton));
        }
        fileRead.close();
        return true;
    }
    return false;
}
