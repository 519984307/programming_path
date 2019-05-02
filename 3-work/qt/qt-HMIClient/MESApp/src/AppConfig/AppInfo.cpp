/*
 * AppInfo.cpp
 *
 *  Created on: 2015��1��30��
 *      Author: cj
 */

#include <string.h>
#include "AppInfo.h"
#include "QFile"
#include "../Sqlite/Entity/Employee.h"
#include "../Public/public.h"

//60��	��Ʒԭ�������ļ���bad_cfg.bin
MESBadCfg AppInfo::bad_cfg;
//61��	��������Ϣ�����ļ���mng_info_cfg.bin
//62��	�����������������������ļ���pd_func_cfg.bin
MESPDFuncCfg AppInfo::pd_func_cfg;
//63��	�豸�����ļ���dev_cfg.bin
MESDevCfg AppInfo::dev_cfg;
//64��	ϵͳ���������ļ���sys_func_cfg.bin
MESSysFuncCfg AppInfo::sys_func_cfg;
//65��	������λ�����ļ���mac_ton_cfg.bin
MESMACTonCfg AppInfo::mac_ton_cfg;
//66��	���������ļ���fault_type_cfg.bin
MESFaultTypeCfg AppInfo::fault_type_cfg;
//67��	�����Ŀ�ļ���check_item_cfg.bin
MESCheckItemCfg AppInfo::check_item_cfg;
//68��	ά�޽�������ļ���repair_result_cfg.bin
MESRepairResultCfg AppInfo::repair_result_cfg;
//69��	��Ʒ����ԭ�������ļ���cause_bad_cfg.bin
MESCauseBadCfg AppInfo::cause_bad_cfg;
//���������Ϣ
MESClassCfg AppInfo::cfg_class;

AppInfo AppInfo::instance;


//��ȡconfig_fileĿ¼���ļ��б�
QStringList AppInfo::getConfigNameList()
{
	QStringList NameList;
	NameList.append("bad_cfg.bin"); //��Ʒ��
	NameList.append("mng_info_cfg.bin"); //��������Ϣ�����ļ�
	NameList.append("pd_func_cfg.bin"); //�����������������������ļ�
	NameList.append("dev_cfg.bin");     //�豸�����ļ�
	NameList.append("sys_func_cfg.bin");//ϵͳ���������ļ�
	NameList.append("mac_ton_cfg.bin"); //������λ�����ļ�
	NameList.append("fault_type_cfg.bin"); //���������ļ�
	NameList.append("check_item_cfg.bin"); //�����Ŀ�ļ�
	NameList.append("repair_result_cfg.bin"); //ά�޽�������ļ�
	NameList.append("cause_bad_cfg.bin");    //��Ʒ����ԭ�������ļ�

	return NameList;
}

//b)	����������Ϣ�����ļ����ļ���ʽ	mng_info_cfg.bin
static void Init_mng_info_cfg_bin()
{
	char version[6];	//	�汾�ţ�6�ֽڵ���������(HEX)
	quint16 sumSize = 0;	//	����Ա������2�ֽڣ�n��

    Employee user;
    user.setIDCardNo("00000001");
    user.setIcCardRight("FFFFFFFF");
    user.setEmpID("00000001");
    user.setEmpNameCN("ϵͳ����Ա");
    user.setPost("ϵͳ����Ա");
    user.subimt();

	QFile fileRead(AppInfo::getPath_Config() + "mng_info_cfg.bin");
	if (fileRead.open(QIODevice::ReadOnly))
	{
		QDataStream readDataStream(&fileRead);
		readDataStream.readRawData(version, sizeof(version));
		readDataStream.readRawData((char*) &sumSize, 2);
		QString versionStr;
		versionStr.sprintf("%02d%02d%02d%02d%02d%02d", version[0], version[1],
				version[2], version[3], version[4], version[5]);

        if( sqlite::SQLiteBaseHelper::getInstance().versionCheck("mng_info_cfg.bin",versionStr) )
        {
            fileRead.close();
            return ;
        }
        sqlite::SQLiteBaseHelper::getInstance().versionSave("mng_info_cfg.bin",versionStr);

		for (int i = 0; i < sumSize; i++)
		{
			quint8 status;
			char card[10];
			char q[16];
			char workNo[20];
			char name[12];
			char point[20];
			int picPoint;
			int picLen;
			readDataStream >> status;	//	״̬��1�ֽ�    ע�ͣ�1��������2���޸ģ�3��ɾ��
			readDataStream.readRawData(card, sizeof(card));	//	����:   10�ֽ�
			readDataStream.readRawData(q, sizeof(q));	//	��Ȩ��:   16�ֽ�
			readDataStream.readRawData(workNo, sizeof(workNo));	//	���ţ�20�ֽ�
			readDataStream.readRawData(name, sizeof(name));	//	������12�ֽ�
			readDataStream.readRawData(point, sizeof(point));	//	ְλ��20�ֽ�
			readDataStream >> picPoint;	//	ͼƬ��ʼ��ַ��4�ֽ�
			readDataStream >> picLen;	//	ͼƬ���ȣ� 4�ֽ�
			Employee user;
			user.setIDCardNo(card);
			user.setIcCardRight(q);
			user.setEmpID(workNo);
			user.setEmpNameCN(name);
			user.setPost(point);

			switch (status)
			{
			case 1: //����
			case 2: //�޸�
				user.subimt();
				break;
			case 3: //ɾ��
				user.remove();
				break;
			}
		}
		fileRead.close();
	}
}

void AppInfo::init()
{
	//
	QString AppInfoPath = getPathFile_appInfo();
	QFile file(AppInfoPath);
	if (!QFile::exists(AppInfoPath))
	{
		AppInfo info;
		info.setServerIp("192.168.8.223");
		info.setServerPortA(4007);
		info.setServerPortB(4008);
		info.setTbProductedBakCount(2000);

		QString str = PropertyBaseToXML(info).toString();
		if (file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream out(&file);
			out << str;
			file.close();
		}
	}
	if (file.open(QIODevice::ReadOnly | QFile::Text))
	{
		XMLFileToPropertyBase(file, instance);
		file.close();
	}

	if (getVersion() != APP_VER_STR)
	{
		setVersion(APP_VER_STR);
		saveConfig();
	}

    //��ʼ�������ļ�
	Init_mng_info_cfg_bin();

    //��Ʒԭ�������ļ� bad_cfg.bin
     bad_cfg.read(AppInfo::getPath_Config() + "bad_cfg.bin");

    //�����������������������ļ����ļ���ʽ	pd_func_cfg.bin
    pd_func_cfg.read(AppInfo::getPath_Config() + "pd_func_cfg.bin");

    //a)	���豸�����ļ����ļ���ʽ	dev_cfg.bin
    dev_cfg.read(AppInfo::getPath_Config() + "dev_cfg.bin");

    //b)	��ϵͳ���������ļ����ļ���ʽsys_func_cfg.bin
    sys_func_cfg.read(AppInfo::getPath_Config() + "sys_func_cfg.bin");

    //������λ�����ļ����ļ���ʽmac_ton_cfg.bin
    mac_ton_cfg.read(AppInfo::getPath_Config() + "mac_ton_cfg.bin");

    //�������������ļ����ļ���ʽfault_type_cfg.bin
    fault_type_cfg.read(AppInfo::getPath_Config() + "fault_type_cfg.bin");

    //e)	�������Ŀ�����ļ����ļ���ʽcheck_item_cfg.bin
    check_item_cfg.read(AppInfo::getPath_Config() + "check_item_cfg.bin");

    //ά�޽�������ļ����ļ���ʽrepair_result_cfg.bin
    repair_result_cfg.read(AppInfo::getPath_Config() + "repair_result_cfg.bin");

    cause_bad_cfg.read(AppInfo::getPath_Config() + "cause_bad_cfg.bin");

	cfg_class.init(); //��γ�ʼ��
}

AppInfo &AppInfo::GetInstance()
{
	return instance;
}

AppInfo::AppInfo()
{
	version = "";
	serverPortA = 0;
	serverPortB = 0;
    BrushCardID = "00000001";
    tbProductedBakCount = 1000;
	UsbPower = 1;
	backLightTime = 30;
    haveStopCard=0;
    ButtonBuzzer=1;
}

void AppInfo::onBindProperty()
{
	addProperty("version", Property::AsQStr, &version); //ϵͳ�汾
	addProperty("devID", Property::AsQStr, &devID); //�豸ID
	addProperty("machineID", Property::AsQStr, &machineID); //����ID
	addProperty("serverIP", Property::AsQStr, &serverIP); //������IP
	addProperty("serverPortA", Property::AsInt, &serverPortA); //�������˿ں�A
	addProperty("serverPortB", Property::AsInt, &serverPortB); //�������˿ں�B
	addProperty("brushCardPassword", Property::AsQStr, &brushCardPassword);
	addProperty("BrushCardID", Property::AsQStr, &BrushCardID);	//��ǰˢ����,����ˢ���������
	addProperty("devIP", Property::AsQStr, &devIP);	//�豸IP
	addProperty("netDeviceName", Property::AsQStr, &netDeviceName);	//�����豸��
	addProperty("tbProductedBakCount", Property::AsInt, &tbProductedBakCount); //�������ݱ��¼�Ĵ�С
	addProperty("haveStopCard", Property::AsInt, &haveStopCard); //�Ƿ���ͣ����
	addProperty("ButtonBuzzer", Property::AsInt, &ButtonBuzzer); //�Ƿ��а�����
	addProperty("UsbPower", Property::AsInt, &UsbPower); //�Ƿ��usb�豸

	addProperty("backLightTime", Property::AsInt, &backLightTime); //����ʱ��
}

void AppInfo::saveConfig()
{
	QFile file(getPathFile_appInfo());

	QString str = PropertyBaseToXML(AppInfo::GetInstance()).toString();
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);
		out << str;
		file.close();
	}
}

void AppInfo::initNetCSCP()
{
	//�����������ݽ���
	CSCPNotebookTask::GetInstance().startCSCPServer(
			AppInfo::GetInstance().getServerIp(),
			AppInfo::GetInstance().getServerPortA(),
			AppInfo::GetInstance().getServerPortB());
}
//��������������Ϣ
void AppInfo::OnUpdateNet(const QString& ip, int portA, int portB)
{
	setServerIp(ip);
	setServerPortA(portA);
	setServerPortB(portB);
	saveConfig();
	CSCPNotebookTask::GetInstance().startCSCPServer(
			AppInfo::GetInstance().getServerIp(),
			AppInfo::GetInstance().getServerPortA(),
			AppInfo::GetInstance().getServerPortB());
}
