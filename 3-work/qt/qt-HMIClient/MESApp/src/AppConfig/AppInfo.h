/*
 * AppInfo.h
 *
 *  Created on: 2015��1��30��
 *      Author: cj
 */

#ifndef APPINFO_H_
#define APPINFO_H_

#include "../Sqlite/Entity/absentity.h"
#include "MESBadCfg.h"
#include "MESPDFuncCfg.h"
#include "MESMACTonCfg.h"
#include "MESSysFuncCfg.h"
#include "MESFaultTypeCfg.h"
#include "MESCheckItemCfg.h"
#include "MESRepairResultCfg.h"
#include "MESCauseBadCfg.h"
#include "MESDevCfg.h"
#include "MESClassCfg.h"

using namespace entity;

/*
 * Ӧ��������Ϣ
 */
class AppInfo: public AbsPropertyBase
{
	//<XML>
	QString version; //ϵͳ�汾
	QString devID; //�豸ID
	QString machineID; //����ID

	QString serverIP; //������IP
	int serverPortA; //�������˿ں�A
	int serverPortB; //�������˿ں�B
	QString devIP;	//�豸IP

	QString netDeviceName; //��ǰ����������
	QString brushCardPassword; //ˢ������
	QString BrushCardID; //ˢ���˿���

	int backLightTime; //����ʱ��
	int tbProductedBakCount; //�������ݱ��¼�Ĵ�С

	int haveStopCard;	//�Ƿ���ͣ����  0 û�У� 1 ��
	int ButtonBuzzer;	//�Ƿ�ť����
	int UsbPower;       //�Ƿ��USB��Դ
	//</XML>


public:
	AppInfo();
	//////////////////////////////////////////////////////////////////
    void init(); //��ʼ��
	void saveConfig();
	void initNetCSCP(); //��ʼ������ӿ�
	void OnUpdateNet(const QString &ip, int portA, int portB); //���µ��µ����������

	static QStringList getConfigNameList(); //��ȡ�����ļ����б�


	/**
	 * Ӧ����Ϣ
	 */
	static const QString getPathFile_appInfo()
	{
		return QCoreApplication::applicationDirPath() + "/appInfo.xml";
	}

    //��ȡ���ܰ�ť״̬�ļ�
    static const QString getPathFile_FunButtonStatus()
	{
		return QCoreApplication::applicationDirPath() + "/ui_btstatus.xml";
	}

	//���ػ���Ŀ¼
	static const QString getPath_DownCache()
	{
		QString path = QCoreApplication::applicationDirPath() + "/caches/";
		if (!QDir().exists(path))
		{
			QDir().mkdir(path);
		}
		return path;
	}

	//��ʱ�ļ�Ŀ¼
	static const QString getPath_Tmp()
	{
		QString path = QCoreApplication::applicationDirPath() + "/tmp/";
		if (!QDir().exists(path))
		{
			QDir().mkdir(path);
		}
		return path;
	}

	//�����ļ�Ŀ¼
	static const QString getPath_Config()
	{
		QString path = QCoreApplication::applicationDirPath() + "/config_file/";
		if (!QDir().exists(path))
		{
			QDir().mkdir(path);
		}
		return path;
	}

	//��־�ļ�Ŀ¼
	static const QString getPath_Logs()
	{
		QString path = QCoreApplication::applicationDirPath() + "/logs/";
		if (!QDir().exists(path))
		{
			QDir().mkdir(path);
		}
		return path;
	}

	//���ݿ�Ŀ¼
	static const QString getPath_Database()
	{
		QString path = QCoreApplication::applicationDirPath() + "/databases/";
		if (!QDir().exists(path))
		{
			QDir().mkdir(path);
		}
		return path;
	}

	//60��	��Ʒԭ�������ļ���bad_cfg.bin
	static MESBadCfg bad_cfg;
	//61��	��������Ϣ�����ļ���mng_info_cfg.bin
	//62��	�����������������������ļ���pd_func_cfg.bin
	static MESPDFuncCfg pd_func_cfg;
	//63��	�豸�����ļ���dev_cfg.bin
	static MESDevCfg dev_cfg;
	//64��	ϵͳ���������ļ���sys_func_cfg.bin
	static MESSysFuncCfg sys_func_cfg;
	//65��	������λ�����ļ���mac_ton_cfg.bin
	static MESMACTonCfg mac_ton_cfg;
	//66��	���������ļ���fault_type_cfg.bin
	static MESFaultTypeCfg fault_type_cfg;
	//67��	�����Ŀ�ļ���check_item_cfg.bin
	static MESCheckItemCfg check_item_cfg;
	//68��	ά�޽�������ļ���repair_result_cfg.bin
	static MESRepairResultCfg repair_result_cfg;
	//69��	��Ʒ����ԭ�������ļ���cause_bad_cfg.bin
	static MESCauseBadCfg cause_bad_cfg;

	//���������Ϣ
	static MESClassCfg cfg_class;

	static AppInfo instance;
	static AppInfo &GetInstance();

	////////////////////////////////////////////////////////////////////////
	// ����Ϊ���ݵ�get set�Ȳ���
	////////////////////////////////////////////////////////////////////////
	virtual const char *getClassName() const
	{
		return GetThisClassName();
	}

	static const char *GetThisClassName()
	{
		return "AppInfo";
	}

	virtual void onBindProperty();

	const QString& getBrushCardPassword() const
	{
		return brushCardPassword;
	}

	void setBrushCardPassword(const QString& brushCardPassword)
	{
		this->brushCardPassword = brushCardPassword;
	}

	const QString& getDevId() const
	{
		return devID;
	}

	void setDevId(const QString& devId)
	{
		devID = devId;
	}

	const QString& getMachineId() const
	{
		return machineID;
	}

	void setMachineId(const QString& machineId)
	{
		machineID = machineId;
	}

	const QString& getServerIp() const
	{
		return serverIP;
	}

	void setServerIp(const QString& serverIp)
	{
		serverIP = serverIp;
	}

	const int& getServerPortA() const
	{
		return serverPortA;
	}

	void setServerPortA(const int& serverPortA)
	{
		this->serverPortA = serverPortA;
	}

	const int& getServerPortB() const
	{
		return serverPortB;
	}

	void setServerPortB(const int& serverPortB)
	{
		this->serverPortB = serverPortB;
	}

	const QString& getVersion() const
	{
		return version;
	}

	void setVersion(const QString& version)
	{
		this->version = version;
	}

	const QString& getBrushCardId() const
	{
		return BrushCardID;
	}

	void setBrushCardId(const QString& brushCardId)
	{
		BrushCardID = brushCardId;
	}
	int getTbProductedBakCount() const
	{
		return tbProductedBakCount;
	}

	void setTbProductedBakCount(int tbProductedBakCount)
	{
		this->tbProductedBakCount = tbProductedBakCount;
	}

	//ͣ�����Ƿ���
	bool getHaveStopCard() const
	{
		return haveStopCard == 1;
	}

	//�趨ͣ����
	void setHaveStopCard(bool flag)
	{
		if (flag)
			this->haveStopCard = 1;
		else
			haveStopCard = 0;
	}

	//��ť����
	bool isButtonBuzzer() const
	{
		return ButtonBuzzer == 1;
	}

	//��ť����
	void setButtonBuzzer(bool flag)
	{
		if (flag)
			ButtonBuzzer = 1;
		else
			ButtonBuzzer = 0;
	}

	//usb��Դ
	bool isUsbPower() const
	{
		return UsbPower == 1;
	}

	//usb��Դ
	void setUsbPower(bool flag)
	{
		if (flag)
			UsbPower = 1;
		else
			UsbPower = 0;
	}
	//�豸IP
	const QString& getDevIp() const
	{
		return devIP;
	}
	//�豸IP
	void setDevIp(const QString& devIp)
	{
		devIP = devIp;
	}
	//�����豸��
	const QString& getNetDeviceName() const
	{
		return netDeviceName;
	}
	//�����豸��
	void setNetDeviceName(const QString& netDeviceName)
	{
		this->netDeviceName = netDeviceName;
	}
	//����ʱ��
	int getBackLightTime() const
	{
		return backLightTime;
	}
	//����ʱ��
	void setBackLightTime(int backLightTime)
	{
		this->backLightTime = backLightTime;
	}
};

#endif /* APPINFO_H_ */
