/*
 * MESCauseBadCfg.h
 *
 *  Created on: 2015��2��2��
 *      Author: cj
 */

#ifndef MESCAUSEBADCFG_H_
#define MESCAUSEBADCFG_H_

#include <QString>
#include <QList>
#include <QStringList>
#include <QMap>

/*
 *��Ʒ����ԭ�������ļ�
 */
class MESCauseBadCfg {
public:
	char version[6];
	QMap<int, QStringList> map;

    virtual bool write(const QString &path); //����д�뵽�ļ�
    virtual bool read(const QString &path); //���ô��ļ���ȡ
};

#endif /* MESCAUSEBADCFG_H_ */
