/*
 * MESClass.h
 *
 *  Created on: 2015��4��15��
 *      Author: cj
 */

#ifndef MESCLASSCFG_H_
#define MESCLASSCFG_H_

#include <QtCore>

/*
 * ���������Ϣ
 */
class ClassTime {
public:
	int wHour;
	int wMinute;
};

class MESClassCfg {
public:
	MESClassCfg();
	QList<ClassTime> ClassTimeList; //���ʱ���
	QString ChangeTime; //��ǰ����ʱ��

	//��ʼ��
	void init();

	//���湦��
	void save();

//    virtual bool write(const QString &path); //����д�뵽�ļ�
//    virtual bool read(const QString &path); //���ô��ļ���ȡ
};

#endif /* MESCLASS_H_ */
