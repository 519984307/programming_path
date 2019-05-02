/*
 * MESDefectiveReason.cpp
 *
 *  Created on: 2015��1��30��
 *      Author: cj
 */

#include "MESBadCfg.h"

bool MESBadCfg::write(const QString &path)
{
	QFile fileRead(path);

	if (fileRead.open(QIODevice::WriteOnly))
	{
		QDataStream dataStream(&fileRead);
		//char version[6];	//�汾�ţ�6�ֽڵ���������(HEX)
        QDateTime dateTime = QDateTime::currentDateTime();
        version[0] = dateTime.date().year() % 100;
        version[1] = dateTime.date().month();
        version[2] = dateTime.date().day();
        version[3] = dateTime.time().hour();
        version[4] = dateTime.time().minute();
        version[5] = dateTime.time().second();

		quint8 sumSize = name.size(); //��Ʒ����������1�ֽڣ�n��
		quint8 defaultType = this->defaultType; //Ĭ�����ͣ�1�ֽ�(<n)
		dataStream.writeBytes(this->version, sizeof(this->version));
		dataStream << sumSize << defaultType;

		this->sumSize = sumSize;

		for (int i = 0; i < sumSize; i++)
		{
			quint8 sum = i;	//��1�����ʹ�Ʒ��//��Ʒ������1�ֽڣ�M1 < = 30��
			dataStream << sum;
			QStringList &list = name[i];

			for (int j = 0; j < sum; j++)
			{
				char name[13];	//��1����Ʒ���֣�12�ֽ�
				strcpy(name, list[j].toAscii().data());
				dataStream.writeRawData(name, 12);
				list.append(QString(name));
			}
		}
		fileRead.close();
		return true;
	}
	return false;
}

bool MESBadCfg::read(const QString &path)
{
	QFile fileRead(path);

	name.clear();

	if (fileRead.open(QIODevice::ReadOnly))
	{
		QDataStream readDataStream(&fileRead);
		//char version[6];	//�汾�ţ�6�ֽڵ���������(HEX)
		quint8 sumSize = 0; //��Ʒ����������1�ֽڣ�n��
		quint8 defaultType = 0; //Ĭ�����ͣ�1�ֽ�(<n)
		readDataStream.readRawData(this->version, sizeof(this->version));
		readDataStream >> sumSize >> defaultType;

		sumSize = sumSize;
		defaultType = defaultType;

		for (int i = 0; i < sumSize; i++)
		{
			quint8 sum;	//��1�����ʹ�Ʒ��//��Ʒ������1�ֽڣ�M1 < = 30��
			readDataStream >> sum;
			QStringList list;
			for (int j = 0; j < sum; j++)
			{
				char name[13];	//��1����Ʒ���֣�12�ֽ�
				memset(name, 0, 13);
				readDataStream.readRawData(name, 12);
				list.append(QString(name));
			}
			name.insert(i, list);
		}
		fileRead.close();
		return true;
	}
	return false;
}
