/*
 * MESCauseBadCfg.cpp
 *
 *  Created on: 2015��2��2��
 *      Author: cj
 */

#include "MESCauseBadCfg.h"
#include "QtCore"


bool MESCauseBadCfg::read(const QString &path)
{
    //char version[6]; //�汾�ţ�6�ֽڵ���������(HEX)
    quint8 sumSize; //��Ʒ����������1�ֽڣ�n��

    this->map.clear();

    QFile fileRead(path);
    if (fileRead.open(QIODevice::ReadOnly))
    {
        QDataStream readDataStream(&fileRead);

        readDataStream.readRawData(this->version, sizeof(this->version));
        readDataStream.readRawData((char*) &sumSize, sizeof(sumSize));
        for (int i = 0; i < sumSize; i++)
        {
            quint8 size = 0;	//��Ʒ����ԭ��������1�ֽ�
            readDataStream >> size;

            QStringList list;
            for (int j = 0; j < size; j++)
            {
                char name[20]; //��1����Ʒ����ԭ�����֣�20�ֽ�
                readDataStream.readRawData(name, sizeof(name));
                list.append(QString(name));
            }
            this->map.insert(i, list);
        }
        fileRead.close();
        return true;
    }
    return false;
}


bool MESCauseBadCfg::write(const QString &path)
{
    //�汾�ţ�6�ֽڵ���������(HEX)
    quint8 sumSize=this->map.size(); //��Ʒ����������1�ֽڣ�n��

    QDateTime dateTime = QDateTime::currentDateTime();
    version[0] = dateTime.date().year() % 100;
    version[1] = dateTime.date().month();
    version[2] = dateTime.date().day();
    version[3] = dateTime.time().hour();
    version[4] = dateTime.time().minute();
    version[5] = dateTime.time().second();

    QFile fileRead(path);
    if (fileRead.open(QIODevice::WriteOnly))
    {
        QDataStream dataStream(&fileRead);

        dataStream.writeBytes(this->version, sizeof(this->version));
        dataStream.writeBytes((char*) &sumSize, sizeof(sumSize));
        for (int i = 0; i < sumSize; i++)
        {
            QStringList &list=map[i];
            quint8 size = list.size();	//��Ʒ����ԭ��������1�ֽ�
            dataStream << size;

            for (int j = 0; j < size; j++)
            {
                char name[20]; //��1����Ʒ����ԭ�����֣�20�ֽ�
                strcpy(name, list[j].toAscii().data());
                dataStream.writeBytes(name, sizeof(name));
            }
        }
        fileRead.close();
        return true;
    }
    return false;
}
