/*
 * MESOrderFunButton.h
 *
 *  Created on: 2015��1��30��
 *      Author: cj
 */

#ifndef MESORDERFUNBUTTON_H_
#define MESORDERFUNBUTTON_H_

#include <QtCore>
#include <QString>
#include <QList>

/*
 *�����������������������ļ�
 //	pd_func_cfg.bin
 //	�������ð���������1�ֽڣ�m����mĿǰ�̶�Ϊ2��
 //	��1������
 //	������ţ�2�ֽ� ��0������Ա��
 //	����Ȩ�ޣ�4�ֽ�
 //	״̬��1�ֽ�        ��ʾ���Ƿ�ͣ����ע�ͣ�0����ͣ����1��ͣ��
 //	��2������
 //	������ţ�2�ֽڣ�1��QCѲ�죩
 //	����Ȩ�ޣ�4�ֽ�
 //	״̬��1�ֽ�        ��ʾ���Ƿ�ͣ����ע�ͣ�0����ͣ����1��ͣ��
 //
 //	�����ð���������1�ֽڣ�n��
 //	��1������
 //	�������ƣ�8�ֽ�
 //	������ţ�2�ֽ�
 //	����Ȩ�ޣ�4�ֽ�
 //	״̬��1�ֽ�        ��ʾ���Ƿ�ͣ����ע�ͣ�0����ͣ����1��ͣ��
 //	��2������
 //	�������ƣ�8�ֽ�
 //	������ţ�2�ֽ�
 //	����Ȩ�ޣ�4�ֽ�
 //	״̬��1�ֽ�        ��ʾ���Ƿ�ͣ����ע�ͣ�0����ͣ����1��ͣ��
 //
 //	����
 //
 //	��n������
 //	�������ƣ�8�ֽ�
 //	������ţ�2�ֽ�
 //	����Ȩ�ޣ�4�ֽ�
 //	״̬��1�ֽ�        ��ʾ���Ƿ�ͣ����ע�ͣ�0����ͣ����1��ͣ��
 //
 */
class MESPDFuncCfg {
public:
	enum KeyStatus {
		//0����ͣ����1��ͣ��
        NOT_STOP=0,
        STOP=1,
        NONE,
	};

	//���ð�����Ϣ
	class SetKeyInfo {
	public:
		QString name; //�������ƣ�8�ֽ�,(�������ð���û�����)
		quint16 funIndex; //������ţ�2�ֽ�
		quint32 funP; //����Ȩ�ޣ�4�ֽ�
		KeyStatus status;	//״̬��1�ֽ� ��ʾ���Ƿ�ͣ����ע�ͣ�0����ͣ����1��ͣ��

		SetKeyInfo(const char *name, quint16 funIndex, quint32 funP,
				KeyStatus status) {
			this->name = QString(name);
			this->funIndex = funIndex;
			this->funP = funP;
			this->status = status;
		}
		SetKeyInfo(){
			name="";
			funIndex=0;
			funP=0;
			status=NONE;
		}
        //�Ƿ���ͣ������
        bool isStopMachineFun(){
            return status==1;
        }
	};

	char version[6]; //�汾�ţ�6�ֽڵ���������(HEX)
	quint8 notSetSize; //�������ð���������1�ֽڣ�m����mĿǰ�̶�Ϊ2��
	QList<SetKeyInfo> NotSetKeyList; //�������ð�����Ϣ
	quint8 setSize; //�����ð���������1�ֽڣ�n��
	QList<SetKeyInfo> SetKeyList; //�����ð�����Ϣ

	MESPDFuncCfg::SetKeyInfo *getSetKeyInfo(quint16 funIndex);

    void Debug();

    virtual bool write(const QString &path); //����д�뵽�ļ�
    virtual bool read(const QString &path); //���ô��ļ���ȡ
};

#endif /* MESORDERFUNBUTTON_H_ */
