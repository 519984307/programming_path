/*
 * MESSysFunc.cpp
 *
 *  Created on: 2015��2��2��
 *      Author: cj
 */

#include "MESSysFuncCfg.h"

void MESSysFuncCfg::SetFun(const char funName[30], quint16 funIndex,
		quint16 funValue) {
	this->funArrays[funIndex].name.sprintf("%s", funName);
	this->funArrays[funIndex].index = funIndex;
    this->funArrays[funIndex].value = funValue;
}

bool MESSysFuncCfg::write(const QString &path)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    version[0] = dateTime.date().year() % 100;
    version[1] = dateTime.date().month();
    version[2] = dateTime.date().day();
    version[3] = dateTime.time().hour();
    version[4] = dateTime.time().minute();

    quint8 funSize = 8; //����������1�ֽڣ�n��
    QFile fileRead(path);

    if (fileRead.open(QIODevice::WriteOnly))
    {
        QDataStream readDataStream(&fileRead);
        readDataStream.writeRawData(this->version, sizeof(this->version));

        readDataStream << funSize;
        this->funSize = funSize;

        for (int i = 0; i < funSize; i++)
        {
            char funName[30]; //�������ƣ�30�ֽ�
            quint16 funIndex; //������ţ�2�ֽ�
            quint16 funValue; //����ֵ��2�ֽ�

            memset(funName,0,sizeof(funName));
            strcpy(funName,funArrays[i].name.toAscii().data());
            funIndex=funArrays[i].index;
            funValue=funArrays[i].value;

            readDataStream.writeRawData(funName, sizeof(funName));
            readDataStream.writeRawData((char*) &funIndex, sizeof(funIndex));
            readDataStream.writeRawData((char*) &funValue, sizeof(funValue));
        }
        fileRead.close();
        return true;
    }
    return false;
}

bool MESSysFuncCfg::read(const QString &path)
{
    //char version[6];	//�汾�ţ�6�ֽڵ���������(HEX)
    quint8 funSize = 0; //����������1�ֽڣ�n��
    QFile fileRead(path);

    if (fileRead.open(QIODevice::ReadOnly))
    {
        QDataStream readDataStream(&fileRead);
        readDataStream.readRawData(this->version, sizeof(this->version));

        readDataStream >> funSize;
        this->funSize = funSize;

        for (int i = 0; i < funSize; i++)
        {
            char funName[30]; //�������ƣ�30�ֽ�
            quint16 funIndex; //������ţ�2�ֽ�
            quint16 funValue; //����ֵ��2�ֽ�
            readDataStream.readRawData(funName, sizeof(funName));
            readDataStream.readRawData((char*) &funIndex, sizeof(funIndex));
            readDataStream.readRawData((char*) &funValue, sizeof(funValue));
            this->SetFun(funName, funIndex, funValue);
        }
        fileRead.close();
        return true;
    }
    return false;
}
