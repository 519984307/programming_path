/*
 * MESRepairResultCfg.h
 *
 *  Created on: 2015��2��2��
 *      Author: cj
 */

#ifndef MESREPAIRRESULTCFG_H_
#define MESREPAIRRESULTCFG_H_

#include "MESFaultTypeCfg.h"
/*
 *�������������ļ�
 */
class MESRepairResultCfg: public MESFaultTypeCfg {
public:
    virtual bool write(const QString &path); //����д�뵽�ļ�
    virtual bool read(const QString &path); //���ô��ļ���ȡ
};

#endif /* MESREPAIRRESULTCFG_H_ */
