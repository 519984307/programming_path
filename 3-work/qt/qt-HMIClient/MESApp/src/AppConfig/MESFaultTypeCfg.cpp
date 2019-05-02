/*
 * MESFaultTypeCfg.cpp
 *
 *  Created on: 2015��2��2��
 *      Author: cj
 */

#include "MESFaultTypeCfg.h"

void MESFaultTypeCfg::Debug() {
//	logDebug("��������:%d", sumSize);
//	for (int i = 0; i < FaultInfoList.size(); i++) {
//		logDebug("��������:%s", FaultInfoList[i].getTypeName().toAscii().data());
//		logDebug("������������:%d", FaultInfoList[i].getTypeSize());
//		for (int j = 0; j < FaultInfoList[i].getTypeList().size(); j++) {
//			logDebug("%d:%s", FaultInfoList[i].getTypeList()[j].Index,
//					FaultInfoList[i].getTypeList()[j].Type.toAscii().data());
//		}
    //	}
}

bool MESFaultTypeCfg::write(const QString &path)
{
    return false;
}


bool MESFaultTypeCfg::read(const QString &path)
{
    //char version[6]; //	�汾�ţ�6�ֽڵ���������(HEX)
    quint8 faultSize; //����������1�ֽڣ�n��

    this->FaultInfoList.clear();

    QFile fileRead(path);
    if (fileRead.open(QIODevice::ReadOnly))
    {
        QDataStream readDataStream(&fileRead);
        readDataStream.readRawData(this->version, sizeof(this->version));
        readDataStream.readRawData((char*) &faultSize, sizeof(faultSize));

        this->sumSize = faultSize;

        for (int i = 0; i < faultSize; i++)
        {
            char faultName[30]; //�������ƣ��������ϣ�30�ֽ�
            quint8 faultTypeSize; //��������������1�ֽڣ�M1��
            readDataStream.readRawData(faultName, sizeof(faultName));
            readDataStream.readRawData((char*) &faultTypeSize,
                    sizeof(faultTypeSize));

            MESFaultTypeCfg::TypeInfo faultItem;
            faultItem.setTypeName(faultName);
            faultItem.setTypeSize(faultSize);
            for (int j = 0; j < faultTypeSize; j++)
            {
                quint8 faultIndex; //��1��������ţ�1�ֽ�
                char faultType[30]; //��1���������ͣ�30�ֽ�
                readDataStream.readRawData((char*) &faultIndex,
                        sizeof(faultIndex));
                readDataStream.readRawData(faultType, sizeof(faultType));

                faultItem.getTypeList().append(
                        MESFaultTypeCfg::TypeItem(faultIndex, faultType));
            }
            this->FaultInfoList.append(faultItem);
        }
        fileRead.close();
        return true;
    }
    return false;
}
