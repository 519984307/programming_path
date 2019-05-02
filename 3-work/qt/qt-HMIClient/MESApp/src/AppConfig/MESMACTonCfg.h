/*
 * MESMachineTon.h
 *
 *  Created on: 2015��2��2��
 *      Author: cj
 */

#ifndef MESMACHINETON_H_
#define MESMACHINETON_H_

#include <QtCore>
#include <QString>
#include <QList>
/*
 *������λ�����ļ�
 //	�汾�ţ�6�ֽڵ���������(HEX)
 //	����������2�ֽڣ�n��
 //	��1������
 //	��ţ�2�ֽ�
 //	������ţ�20�ֽ�
 //	��λֵ��4�ֽ�
 //	��2������
 //	��ţ�2�ֽ�	�������ƣ�30�ֽ�
 //	������ţ�20�ֽ�	������ţ�2�ֽ�
 //	��λֵ��4�ֽ�	����ֵ��2�ֽ�
 //
 //	����
 //	��n������
 //	��ţ�2�ֽ�	�������ƣ�30�ֽ�
 //	������ţ�20�ֽ�	������ţ�2�ֽ�
 //	��λֵ��4�ֽ�	����ֵ��2�ֽ�
 //
 */
class MESMACTonCfg
{
public:
	class TonInfo
	{
		int index; //	��ţ�2�ֽ�
		QString machine; //	������ţ�20�ֽ�
		int ton; //	��λֵ��4�ֽ�
		QString ipaddress;	//����IP
		QString machinoBrand; //Ʒ��

	public:
		TonInfo()
		{
			index = 0;
			ton = 0;
		}
		TonInfo(int index, const char *machine,
				const char *ipaddr,
				const char *machinoBrand,
				int ton)
		{
			this->index = index;
			this->machine = QString(machine);
			this->ton = ton;
			this->ipaddress =QString(ipaddr);
			this->machinoBrand = QString(machinoBrand);
		}

		int getIndex() const
		{
			return index;
		}

		void setIndex(int index)
		{
			this->index = index;
		}

		const QString& getMachine() const
		{
			return machine;
		}

		void setMachine(const QString& machine)
		{
			this->machine = machine;
		}

		int getTon() const
		{
			return ton;
		}

		void setTon(int ton)
		{
			this->ton = ton;
		}

		const QString& getIpaddress() const
		{
			return ipaddress;
		}

		void setIpaddress(const QString& ipaddress)
		{
			this->ipaddress = ipaddress;
		}

		const QString& getMachinoBrand() const
		{
			return machinoBrand;
		}

		void setMachinoBrand(const QString& machinoBrand)
		{
			this->machinoBrand = machinoBrand;
		}
	};

	char version[6]; //�汾�ţ�6�ֽڵ���������(HEX)
	int macSize; //����������2�ֽڣ�n��
	QList<TonInfo> TonList; //������λ�б�

    virtual bool write(const QString &path); //����д�뵽�ļ�
    virtual bool read(const QString &path); //���ô��ļ���ȡ
};

#endif /* MESMACHINETON_H_ */
