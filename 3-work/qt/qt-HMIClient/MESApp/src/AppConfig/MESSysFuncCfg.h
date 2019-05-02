/*
 * MESSysFunc.h
 *
 *  Created on: 2015��2��2��
 *      Author: cj
 */

#ifndef MESSYSFUNC_H_
#define MESSYSFUNC_H_

#include <QtCore>
#include <QString>

/*
 * ϵͳ���������ļ�
 *�汾�ţ�6�ֽڵ���������(HEX)
 ����������1�ֽڣ�n��
 ��1���ܣ�
 �������ƣ�30�ֽ�
 ������ţ�2�ֽ�
 ����ֵ��2�ֽ�
 ��2���ܣ�
 �������ƣ�30�ֽ�
 ������ţ�2�ֽ�
 ����ֵ��2�ֽ�

 ����

 ��n���ܣ�
 �������ƣ�30�ֽ�
 ������ţ�2�ֽ�
 ����ֵ��2�ֽ�
 sys_func_cfg
 *
 */
class MESSysFuncCfg
{
	class FunInfo
	{
	public:
		QString name; //�������ƣ�30�ֽ�
		int index; //������ţ�2�ֽ�
		int value; //����ֵ��2�ֽ�
	};
//	���	��������	��������
//	1	ͣ���Ƿ����	��ˢ��ͣ�������Ƿ���Ҫ������0����������1������
//	2	�ɼ�ѹ����ʽ	1����ѹ��2��������
//	3	�Ƿ��������ͣ��״̬	0��������1������
//	4	�ﵽ�������Ƿ��Զ�����	0����������1���Զ�����
//	5	�źŲɼ���ʽ	0���ɼ���DI�ɼ� ��1��IOģ��DI�ɼ�
//	6	��̨�ź�����	0��ͨ�����ͣ�1���������ͣ�2�����ݿ��3���ո֣�4�������������
//	7	ѹ���Ƿ����	0��δ���ߣ�1���ѽ���
//	8	�¶�·��	2��2·�¶ȣ�4��4·�¶ȣ�8��8·�¶�
public:
	char version[6]; //	�汾�ţ�6�ֽڵ���������(HEX)
	int funSize; //	����������1�ֽڣ�n��

	void SetFun(const char funName[30], //�������ƣ�30�ֽ�
			quint16 funIndex, //������ţ�2�ֽ�
			quint16 funValue); //����ֵ��2�ֽ�
	//	1	ͣ���Ƿ����	��ˢ��ͣ�������Ƿ���Ҫ������0����������1������
	bool isStopCount()
	{
		return funArrays[1].value == 1;
	}
	//	2	�ɼ�ѹ����ʽ	1����ѹ��2��������
	bool PressType()
	{
	       return funArrays[2].value;
	}

	//	3	�Ƿ��������ͣ��״̬	0��������1������
	//			��ͣ��: A ͣ��, B Ҳ����ͣ��
	//			��ͣ��: A ͣ��     B ������ͣ��
	bool isMultipleStopStatus()
	{
		return funArrays[3].value == 1;
	}
	//	4	�ﵽ�������Ƿ��Զ�����	0����������1���Զ�����
	bool isAutoChangeOrder()
	{
		return funArrays[4].value == 1;
	}
	//	5	�źŲɼ���ʽ	0���ɼ���DI�ɼ� ��1��IOģ��DI�ɼ�
	bool isIOCollect()
	{
		return funArrays[5].value == 1;
	}

	//	6	��̨�ź�����	0��ͨ�����ͣ�1���������ͣ�2�����ݿ��3���ո֣�4�������������
	quint8 machineType()
	{
		return funArrays[6].value ;
	}
	//	7	ѹ���Ƿ����	0��δ���ߣ�1���ѽ���
	bool isLineforPress()
	{
		return funArrays[7].value ;
	}
	//	8	�¶�·��	2��2·�¶ȣ�4��4·�¶ȣ�8��8·�¶�
	quint8 temperateChannel()
	{
	      return funArrays[8].value ;
	}

    virtual bool write(const QString &path); //����д�뵽�ļ�
    virtual bool read(const QString &path); //���ô��ļ���ȡ
private:
	FunInfo funArrays[9];
};

#endif /* MESSYSFUNC_H_ */
