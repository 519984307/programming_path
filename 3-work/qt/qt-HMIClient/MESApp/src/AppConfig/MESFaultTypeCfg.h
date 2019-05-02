/*
 * MESFaultTypeCfg.h
 *
 *  Created on: 2015��2��2��
 *      Author: cj
 */

#ifndef MESFAULTTYPECFG_H_
#define MESFAULTTYPECFG_H_

#include "../Unit/MESLog.h"
#include <QtCore>
#include <QString>

/*
 *�������������ļ�
 */
class MESFaultTypeCfg {
public:
	class TypeItem {
	public:
		int Index; //������ţ�1�ֽ�
		QString Type; //�������ͣ�30�ֽ�

		TypeItem(int index, const char *typeName) {
			this->Index = index;
			this->Type = QString(typeName);
		}

	};

	class TypeInfo {
		QString typeName; //�������ƣ��������ϣ�30�ֽ�
		int typeSize; //��������������1�ֽڣ�M1��
		QList<TypeItem> TypeList; //���������б�
	public:
		QList<TypeItem>& getTypeList() {
			return TypeList;
		}

		const QString& getTypeName() const {
			return typeName;
		}

		void setTypeName(const QString& typeName) {
			this->typeName = typeName;
		}

		int getTypeSize() const {
			return typeSize;
		}

		void setTypeSize(int typeSize) {
			this->typeSize = typeSize;
		}
	};

	char version[6]; //�汾�ţ�6�ֽڵ���������(HEX)
	int sumSize; //����������1�ֽڣ�n��
	QList<TypeInfo> FaultInfoList; //������Ϣ

	TypeInfo *getFault(const QString &typeName) {
		int size = FaultInfoList.size();
		for (int i = 0; i < size; i++) {
			if (FaultInfoList[i].getTypeName() == typeName) {
				return &FaultInfoList[i];
			}
		}
		return NULL;
	}

	void Debug();

    virtual bool write(const QString &path); //����д�뵽�ļ�
    virtual bool read(const QString &path); //���ô��ļ���ȡ
};

#endif /* MESFAULTTYPECFG_H_ */
