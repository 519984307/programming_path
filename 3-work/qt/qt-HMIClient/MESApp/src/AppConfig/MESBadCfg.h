/*
 * MESDefectiveReason.h
 *
 *  Created on: 2015��1��30��
 *      Author: cj
 */

#ifndef MESDEFECTIVEREASON_H_
#define MESDEFECTIVEREASON_H_

#include <QString>
#include <QList>
#include <QStringList>
#include <QMap>

#include "../Unit/MESLog.h"

/*
 * ��Ʒԭ������
 */
class MESBadCfg {
public:
	char version[6]; //�汾�ţ�6�ֽڵ���������(HEX)
	int sumSize;  //��Ʒ����������1�ֽڣ�n��
	int defaultType; //Ĭ�����ͣ�1�ֽ�(<n)
	QMap<int, QStringList> name;

    virtual bool write(const QString &path); //����д�뵽�ļ�
    virtual bool read(const QString &path); //���ô��ļ���ȡ
};

#endif /* MESDEFECTIVEREASON_H_ */
