/*
 * MESRepairResultCfg.cpp
 *
 *  Created on: 2015��2��2��
 *      Author: cj
 */

#include "MESRepairResultCfg.h"



bool MESRepairResultCfg::write(const QString &path)
{
    return false;
}

bool MESRepairResultCfg::read(const QString &path)
{
    //char version[6]; //�汾�ţ�6�ֽڵ���������(HEX)
    quint8 repairSize; //����������1�ֽڣ�n��

    this->FaultInfoList.clear();

    QFile fileRead(path);
    if (fileRead.open(QIODevice::ReadOnly))
    {
        QDataStream readDataStream(&fileRead);
        readDataStream.readRawData(this->version, sizeof(this->version));
        readDataStream.readRawData((char*) &repairSize, sizeof(repairSize));

        this->sumSize = repairSize;

        for (int i = 0; i < repairSize; i++)
        {
            char faultName[30]; //ά�����ƣ��������ϣ�30�ֽ�
            quint8 repairSize; //ά�޽��������1�ֽڣ�M1��
            readDataStream.readRawData(faultName, sizeof(faultName));
            readDataStream.readRawData((char*) &repairSize, sizeof(repairSize));

            MESFaultTypeCfg::TypeInfo faultItem;
            faultItem.setTypeName(faultName);
            faultItem.setTypeSize(repairSize);

            for (int j = 0; j < repairSize; j++)
            {
                quint8 Index; //��1�������ţ�1�ֽ�
                char Type[30]; //��1��������ͣ�30�ֽ�
                readDataStream.readRawData((char*) &Index, sizeof(Index));
                readDataStream.readRawData(Type, sizeof(Type));

                faultItem.getTypeList().append(
                        MESFaultTypeCfg::TypeItem(Index, Type));
            }
            this->FaultInfoList.append(faultItem);
        }
        fileRead.close();
        return true;
    }
    return false;
}
