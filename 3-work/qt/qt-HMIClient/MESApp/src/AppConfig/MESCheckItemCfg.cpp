/*
 * MESCheckItemCfg.cpp
 *
 *  Created on: 2015��2��2��
 *      Author: cj
 */

#include "MESCheckItemCfg.h"
#include <QtCore>


bool MESCheckItemCfg::write(const QString &path)
{
    quint8 checkSize=this->List.size(); //��Ŀ������1�ֽڣ�M��
    QDateTime dateTime = QDateTime::currentDateTime();
    version[0] = dateTime.date().year() % 100;
    version[1] = dateTime.date().month();
    version[2] = dateTime.date().day();
    version[3] = dateTime.time().hour();
    version[4] = dateTime.time().minute();
    version[5] = dateTime.time().second();
    //this->List.clear();

    QFile fileRead(path);
    if (fileRead.open(QIODevice::WriteOnly))
    {
        QDataStream readDataStream(&fileRead);
        readDataStream.writeBytes(this->version, sizeof(this->version));
        readDataStream.writeBytes((char*) &checkSize, sizeof(checkSize));

        this->sumSize = checkSize;

        for (int i = 0; i < checkSize; i++)
        {
            char modeName[30]; //�ͺ����ƣ�XXXX��30�ֽ�
            quint8 modeType; //��Ŀ���ͣ�0������������1�ֽ�
            quint8 count; //�����Ŀ������1�ֽڣ�M11��

            MESCheckItemCfg::CheckModel &model=List[i];

            strcpy(modeName,model.ModelName.toAscii().data());
            modeType=0;
            count=model.MachineList.size();

            readDataStream.writeBytes(modeName, 30);
            readDataStream.writeBytes((char*) &modeType, 1); //��Ŀ���ͣ�0������������1�ֽ�
            readDataStream.writeBytes((char*) &count, 1); //�����Ŀ������1�ֽڣ�M11��

            for (int j = 0; j < count; j++)
            {
                MESCheckItemCfg::CheckItem &item=model.MachineList[j];
                quint8 Index; //��1����Ŀ��ţ�1�ֽ�
                char name[30]; //��1����Ŀ��30�ֽ�

                Index=item.getIndex();
                strcpy(name, item.getName().toAscii().data());

                readDataStream.writeBytes((char*) &Index, sizeof(Index));
                readDataStream.writeBytes(name, sizeof(name));
            }

            modeType=1;
            count=model.ModeList.size();

            readDataStream.writeBytes((char*) &modeType, 1); //��Ŀ���ͣ�0������������1�ֽ�
            readDataStream.writeBytes((char*) &count, 1); //�����Ŀ������1�ֽڣ�M11��

            for (int j = 0; j < count; j++)
            {
                MESCheckItemCfg::CheckItem &item=model.ModeList[j];
                quint8 Index; //��1����Ŀ��ţ�1�ֽ�
                char name[30]; //��1����Ŀ��30�ֽ�

                Index=item.getIndex();
                strcpy(name, item.getName().toAscii().data());

                readDataStream.writeBytes((char*) &Index, sizeof(Index));
                readDataStream.writeBytes(name, sizeof(name));
            }

            //this->List.append(model);
        }
        fileRead.close();
        return true;
    }
    return false;
}

bool MESCheckItemCfg::read(const QString &path)
{
    //char version[6]; //�汾�ţ�6�ֽڵ���������(HEX)
    quint8 checkSize; //��Ŀ������1�ֽڣ�M��

    this->List.clear();

    QFile fileRead(path);
    if (fileRead.open(QIODevice::ReadOnly))
    {
        QDataStream readDataStream(&fileRead);
        readDataStream.readRawData(this->version, sizeof(this->version));
        readDataStream.readRawData((char*) &checkSize, sizeof(checkSize));

        this->sumSize = checkSize;

        for (int i = 0; i < checkSize; i++)
        {
            char modeName[30]; //�ͺ����ƣ�XXXX��30�ֽ�
            quint8 modeType; //��Ŀ���ͣ�0������������1�ֽ�
            quint8 count; //�����Ŀ������1�ֽڣ�M11��
            readDataStream.readRawData(modeName, 30);
            readDataStream.readRawData((char*) &modeType, 1); //��Ŀ���ͣ�0������������1�ֽ�
            readDataStream.readRawData((char*) &count, 1); //�����Ŀ������1�ֽڣ�M11��

            MESCheckItemCfg::CheckModel model;
            model.ModelName = modeName;

            for (int j = 0; j < count; j++)
            {
                quint8 Index; //��1����Ŀ��ţ�1�ֽ�
                char name[30]; //��1����Ŀ��30�ֽ�

                readDataStream.readRawData((char*) &Index, sizeof(Index));
                readDataStream.readRawData(name, sizeof(name));

                MESCheckItemCfg::CheckItem item(Index, name);
                if (modeType == 0)
                    model.MachineList.append(item);
                if (modeType == 1)
                    model.ModeList.append(item);
            }

            readDataStream.readRawData((char*) &modeType, 1); //��Ŀ���ͣ�1����ģ�ߡ���1�ֽ�
            readDataStream.readRawData((char*) &count, 1); //�����Ŀ������1�ֽڣ�M12��

            for (int j = 0; j < count; j++)
            {
                quint8 Index; //��1����Ŀ��ţ�1�ֽ�
                char name[30]; //��1����Ŀ��30�ֽ�

                readDataStream.readRawData((char*) &Index, sizeof(Index));
                readDataStream.readRawData(name, sizeof(name));

                MESCheckItemCfg::CheckItem item(Index, name);
                if (modeType == 0)
                    model.MachineList.append(item);
                if (modeType == 1)
                    model.ModeList.append(item);
            }

            this->List.append(model);
        }
        fileRead.close();
        return true;
    }
    return false;
}
