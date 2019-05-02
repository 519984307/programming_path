/*
 * MESCheckItemCfg.h
 *
 *  Created on: 2015��2��2��
 *      Author: cj
 */

#ifndef MESCHECKITEMCFG_H_
#define MESCHECKITEMCFG_H_

#include <QString>
#include <QList>

/*
 *�����Ŀ�����ļ�
 *�汾�ţ�6�ֽڵ���������(HEX)
 �ͺ�������1�ֽڣ�n��
 ��1���ͺţ�
 �ͺ����ƣ�XXXX��30�ֽ�
 ��Ŀ���ͣ�0������������1�ֽ�
 �����Ŀ������1�ֽڣ�M11��
 ��1����Ŀ��ţ�1�ֽ�
 ��1����Ŀ��30�ֽ�
 ��2����Ŀ��ţ�1�ֽ�
 ��2����Ŀ��30�ֽ�
 ����
 ��M11����Ŀ��ţ�1�ֽ�
 ��M11����Ŀ��30�ֽ�
 ��Ŀ���ͣ�1����ģ�ߡ���1�ֽ�
 �����Ŀ������1�ֽڣ�M12��
 ��1����Ŀ��ţ�1�ֽ�
 ��1����Ŀ��30�ֽ�
 ��2����Ŀ��ţ�1�ֽ�
 ��2����Ŀ��30�ֽ�
 ����
 ��M12����Ŀ��ţ�1�ֽ�
 ��M12����Ŀ��30�ֽ�
 ��2���ͺţ�
 �ͺ����ƣ�XXXX��30�ֽ�
 ��Ŀ���ͣ�0������������1�ֽ�
 �����Ŀ������1�ֽڣ�M21��
 ��1����Ŀ��ţ�1�ֽ�
 ��1����Ŀ��30�ֽ�
 ��2����Ŀ��ţ�1�ֽ�
 ��2����Ŀ��30�ֽ�
 ����
 ��M21����Ŀ��ţ�1�ֽ�
 ��M21����Ŀ��30�ֽ�
 ��Ŀ���ͣ�1����ģ�ߡ���1�ֽ�
 �����Ŀ������1�ֽڣ�M22��
 ��1����Ŀ��ţ�1�ֽ�
 ��1����Ŀ��30�ֽ�
 ��2����Ŀ��ţ�1�ֽ�
 ��2����Ŀ��30�ֽ�
 ����
 ��M22����Ŀ��ţ�1�ֽ�
 ��M22����Ŀ��30�ֽ�

 ����

 ��n���ͺţ�
 �ͺ����ƣ�XXXX��30�ֽ�
 ��Ŀ���ͣ�0������������1�ֽ�
 �����Ŀ������1�ֽڣ�Mn1��
 ��1����Ŀ��ţ�1�ֽ�
 ��1����Ŀ��30�ֽ�
 ��2����Ŀ��ţ�1�ֽ�
 ��2����Ŀ��30�ֽ�
 ����
 ��Mn1����Ŀ��ţ�1�ֽ�
 ��Mn1����Ŀ��30�ֽ�
 ��Ŀ���ͣ�1����ģ�ߡ���1�ֽ�
 �����Ŀ������1�ֽڣ�Mn2��
 ��1����Ŀ��ţ�1�ֽ�
 ��1����Ŀ��30�ֽ�
 ��2����Ŀ��ţ�1�ֽ�
 ��2����Ŀ��30�ֽ�
 ����
 ��Mn2����Ŀ��ţ�1�ֽ�
 ��Mn2����Ŀ��30�ֽ�

 */
class MESCheckItemCfg {
public:
	class CheckItem {
		int index; //��1����Ŀ��ţ�1�ֽ�
		QString name; //��1����Ŀ��30�ֽ�
	public:
		CheckItem() {
			index = 0;
			name = "";
		}
		CheckItem(int index, const char *n) {
			this->index = index;
			this->name = QString(n);
		}

		int getIndex() const {
			return index;
		}

		void setIndex(int index) {
			this->index = index;
		}

		const QString& getName() const {
			return name;
		}

		void setName(const QString& name) {
			this->name = name;
		}
	};

	//����ͺ�
	class CheckModel {
	public:
		QString ModelName;		//�ͺ����ƣ�XXXX��30�ֽ�
		//��Ŀ���ͣ�0������������1�ֽ�
		QList<CheckItem> MachineList;
		//��Ŀ���ͣ�1����ģ�ߡ���1�ֽ�
		QList<CheckItem> ModeList;
	};

	char version[6]; //�汾�ţ�6�ֽڵ���������(HEX)
	int sumSize; //�ͺ�������1�ֽڣ�M��

	QList<CheckModel> List;

    virtual bool write(const QString &path); //����д�뵽�ļ�
    virtual bool read(const QString &path); //���ô��ļ���ȡ
};

#endif /* MESCHECKITEMCFG_H_ */
