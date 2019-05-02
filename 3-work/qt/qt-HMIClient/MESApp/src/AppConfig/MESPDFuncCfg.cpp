/*
 * MESOrderFunButton.cpp
 *
 *  Created on: 2015��1��30��
 *      Author: cj
 */

#include "MESPDFuncCfg.h"

MESPDFuncCfg::SetKeyInfo* MESPDFuncCfg::getSetKeyInfo(quint16 funIndex) {
	for (int i = 0; i < SetKeyList.size(); i++) {
		if (SetKeyList[i].funIndex == funIndex)
			return &SetKeyList[i];
	}
	for (int i = 0; i < NotSetKeyList.size(); i++) {
        if (NotSetKeyList[i].funIndex == funIndex)
			return &NotSetKeyList[i];
	}
    return 0;
}

void MESPDFuncCfg::Debug()
{
}


bool MESPDFuncCfg::read(const QString &path)
{
    //char version[6];	//�汾�ţ�6�ֽڵ���������(HEX)
    quint8 notSetSize = 0; //�������ð���������1�ֽڣ�m����mĿǰ�̶�Ϊ2��
    quint8 setSize = 0; //�����ð���������1�ֽڣ�n��

    this->NotSetKeyList.clear();
    this->SetKeyList.clear();

    QFile fileRead(path);
    if (fileRead.open(QIODevice::ReadOnly))
    {
        QDataStream readDataStream(&fileRead);
        readDataStream.readRawData(this->version, sizeof(this->version));
        readDataStream >> notSetSize;

        this->notSetSize = notSetSize;

        for (int i = 0; i < notSetSize; i++)
        {
            quint16 funIndex = 0; //������ţ�2�ֽ� ��0������Ա��
            quint32 funP = 0; //����Ȩ�ޣ�4�ֽ�
            quint8 status = 0; //״̬��1�ֽ�   ��ʾ���Ƿ�ͣ����ע�ͣ�0����ͣ����1��ͣ��
            readDataStream.readRawData((char*) &funIndex, 2);
            readDataStream.readRawData((char*) &funP, 4);
            readDataStream >> status;

            this->NotSetKeyList.append(
                    MESPDFuncCfg::SetKeyInfo("", funIndex, funP,
                            (MESPDFuncCfg::KeyStatus) status));
        }
        readDataStream >> setSize;
        this->setSize = setSize;

        for (int i = 0; i < setSize; i++)
        {
            char name[9]; //�������ƣ�8�ֽ�
            quint16 funIndex = 0; //������ţ�2�ֽ�
            quint32 funP = 0; //����Ȩ�ޣ�4�ֽ�
            quint8 status = 0;	//״̬��1�ֽ� ��ʾ���Ƿ�ͣ����ע�ͣ�0����ͣ����1��ͣ��
            memset(name, 0, sizeof(name));
            readDataStream.readRawData((char*) &name, 8);
            readDataStream.readRawData((char*) &funIndex, sizeof(funIndex));
            readDataStream.readRawData((char*) &funP, sizeof(funP));
            readDataStream >> status;

            this->SetKeyList.append(
                    MESPDFuncCfg::SetKeyInfo(name, funIndex, funP,
                            (MESPDFuncCfg::KeyStatus) status));
        }
        fileRead.close();
        return true;
    }
    this->Debug();
    return false;
}


bool MESPDFuncCfg::write(const QString &path)
{
    return false;
}
