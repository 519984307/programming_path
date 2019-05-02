/*
 * Producted.cpp
 *
 *  Created on: 2015��1��22��
 *      Author: cj
 */

#include "Producted.h"
#include "../SQLiteProductedHelper.h"

namespace entity
{

BEGIN_COLUMNS_MAP(tb_Producted,Producted)
COLUMNS_INT	(id,&Producted::setId)
	COLUMNS_STR (DispatchNo,&Producted::setDispatchNo)
	COLUMNS_STR(DispatchPrior,&Producted::setDispatchPrior)
	COLUMNS_STR(DispatchNo ,&Producted::setDispatchNo ) //		�ɹ�����
	COLUMNS_STR(DispatchPrior ,&Producted::setDispatchPrior )//�ɹ����
	COLUMNS_STR(ProcCode ,&Producted::setProcCode )///			�������
	COLUMNS_STR(StaCode ,&Producted::setStaCode )//			վ�����
	COLUMNS_STR(EndCycle ,&Producted::setEndCycle )//			�������ݲ���ʱ��
	COLUMNS_STR(TemperValue ,&Producted::setTemperValue )//		N·�¶�ֵ List<int>
	COLUMNS_INT(MachineCycle ,&Producted::setMachineCycle )//		��������
	COLUMNS_INT(FillTime ,&Producted::setFillTime )//			���ʱ��
	COLUMNS_INT(CycleTime ,&Producted::setCycleTime )//��������
	COLUMNS_INT(TotalNum ,&Producted::setTotalNum )//ģ��
	COLUMNS_STR(KeepPress ,&Producted::setKeepPress )//100��ѹ���� List<int>
	COLUMNS_STR(StartCycle ,&Producted::setStartCycle )//�������ݿ�ʼʱ
	END_COLUMNS_MAP ()

bool Producted::subimt()
{
	QSqlQuery query(sqlite::SQLiteProductedHelper::getInstance().getDB());
	query.prepare("REPLACE INTO tb_Producted ("
	//"id	," //Y KEY autoincrement," //	id pk auto
					"DispatchNo	,"//�ɹ�����
					"DispatchPrior,"//�ɹ����
					"ProcCode,"//		�������
					"StaCode,"//			վ�����
					"EndCycle,"//	�������ݲ���ʱ��
					"TemperValue,"//		N·�¶�ֵ List<int>
					"MachineCycle,"//��������
					"FillTime,"//	���ʱ��
					"CycleTime,"//	��������
					"TotalNum,"//ģ��
					"KeepPress,"//	100��ѹ���� List<int>
					"StartCycle"//����
					") values "
					"(?,?,?,?,?,?,?,?,?,?,?,?)");
	//query.addBindValue(id); //Y KEY autoincrement," //	id pk auto
	query.addBindValue(DispatchNo); //�ɹ�����
	query.addBindValue(DispatchPrior); //�ɹ����
	query.addBindValue(ProcCode); //		�������
	query.addBindValue(StaCode); //			վ�����
	query.addBindValue(EndCycle); //	�������ݲ���ʱ��
	QString _TemperValue;
	for (int i = 0; i < TemperValue.length(); i++)
	{
		QString v;
		_TemperValue += v.sprintf("%d", TemperValue[i]);
		if (i < TemperValue.length() - 1)
			_TemperValue += ",";
	}
	query.addBindValue(_TemperValue); //		N·�¶�ֵ List<int>
	query.addBindValue(MachineCycle); //��������
	query.addBindValue(FillTime); //	���ʱ��
	query.addBindValue(CycleTime); //	��������
	query.addBindValue(TotalNum); //ģ��
	QString _KeepPress;
	for (int i = 0; i < KeepPress.length(); i++)
	{
		QString v;
		_KeepPress += v.sprintf("%d", KeepPress[i]);
		if (i < KeepPress.length() - 1)
			_KeepPress += ",";
	}
	query.addBindValue(_KeepPress); //	100��ѹ���� List<int>
	query.addBindValue(StartCycle); //��
	if (sqlite::SQLiteProductedHelper::getInstance().exec(query, __FILE__,
			__FUNCTION__, __LINE__))
		return true;
	return false;
}

bool Producted::subimt_bak()
{
	QSqlQuery query(sqlite::SQLiteProductedHelper::getInstance().getDB());
	query.prepare("REPLACE INTO tb_Producted_bak ("
	//"id	," //Y KEY autoincrement," //	id pk auto
					"DispatchNo	,"//�ɹ�����
					"DispatchPrior,"//�ɹ����
					"ProcCode,"//		�������
					"StaCode,"//			վ�����
					"EndCycle,"//	�������ݲ���ʱ��
					"TemperValue,"//		N·�¶�ֵ List<int>
					"MachineCycle,"//��������
					"FillTime,"//	���ʱ��
					"CycleTime,"//	��������
					"TotalNum,"//ģ��
					"KeepPress,"//	100��ѹ���� List<int>
					"StartCycle"//����
					") values "
					"(?,?,?,?,?,?,?,?,?,?,?,?)");
	//query.addBindValue(id); //Y KEY autoincrement," //	id pk auto
	query.addBindValue(DispatchNo); //�ɹ�����
	query.addBindValue(DispatchPrior); //�ɹ����
	query.addBindValue(ProcCode); //		�������
	query.addBindValue(StaCode); //			վ�����
	query.addBindValue(EndCycle); //	�������ݲ���ʱ��
	QString _TemperValue;
	for (int i = 0; i < TemperValue.length(); i++)
	{
		QString v;
		_TemperValue += v.sprintf("%d", TemperValue[i]);
		if (i < TemperValue.length() - 1)
			_TemperValue += ",";
	}
	query.addBindValue(_TemperValue); //		N·�¶�ֵ List<int>
	query.addBindValue(MachineCycle); //��������
	query.addBindValue(FillTime); //	���ʱ��
	query.addBindValue(CycleTime); //	��������
	query.addBindValue(TotalNum); //ģ��
	QString _KeepPress;
	for (int i = 0; i < KeepPress.length(); i++)
	{
		QString v;
		_KeepPress += v.sprintf("%d", KeepPress[i]);
		if (i < KeepPress.length() - 1)
			_KeepPress += ",";
	}
	query.addBindValue(_KeepPress); //	100��ѹ���� List<int>
	query.addBindValue(StartCycle); //��
	if (sqlite::SQLiteProductedHelper::getInstance().exec(query, __FILE__,
			__FUNCTION__, __LINE__))
		return true;
	return false;
}

bool Producted::remove()
{
	QSqlQuery query(sqlite::SQLiteProductedHelper::getInstance().getDB());
	query.prepare("delete from tb_Producted where ID=? ");
	query.bindValue(0, id);
	return sqlite::SQLiteProductedHelper::getInstance().exec(query, __FILE__,
			__FUNCTION__, __LINE__);
}

bool Producted::allList(QList<Producted> &list)
{
	return sqlite::SQLiteProductedHelper::getInstance().selectList(list,
			"tb_Producted", "*", "");
}

int Producted::count_bak()
{
	QSqlQuery query(sqlite::SQLiteProductedHelper::getInstance().getDB());
	query.prepare("SELECT count(*) from tb_Producted_bak");
	if (sqlite::SQLiteProductedHelper::getInstance().exec(query, __FILE__,
			__FUNCTION__, __LINE__))
	{
		if (query.next())
		{
			return query.value(0).toInt();
		}
	}
	return 0;
}

int Producted::count()
{
	QSqlQuery query(sqlite::SQLiteProductedHelper::getInstance().getDB());
	query.prepare("SELECT count(*) from tb_Producted");
	if (sqlite::SQLiteProductedHelper::getInstance().exec(query, __FILE__,
			__FUNCTION__, __LINE__))
	{
		if (query.next())
		{
			return query.value(0).toInt();
		}
	}
	return 0;
}

} /* namespace entity */

