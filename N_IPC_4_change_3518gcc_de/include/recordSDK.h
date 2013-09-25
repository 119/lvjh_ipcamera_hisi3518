/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�recordSDK.h
* �ļ�˵�������ļ�������¼��SDK��API
* ��    �ߣ�ׯ�ݱ�
* �汾��Ϣ��V1.0
* �������: 2007-01-29
* �޸ļ�¼:
*   �޸�1      ��    ��:
*              ��    ��:
*              �޸�����:
*   �޸�:2     ��    ��:
*              ��    ��:
*              �޸�����:
* ����˵��: ��
******************************************************************************/

#ifndef	__RECORD_SDK_H_
#define	__RECORD_SDK_H_

#include "recordStruct.h"

// ¼��SDK��
int RECORDSDK_Open(int nChnNum);

// ¼��SDK�ر�
int RECORDSDK_Close();

// ��ȡ¼��SDK����
int RECORDSDK_Setup(RECORD_SETUP setup);

// ����SDK����
int RECORDSDK_GetSetup(RECORD_SETUP *setup);

// ����¼��SDK
int RECORDSDK_Start();

// ֹͣ¼��SDK
int RECORDSDK_Stop();

// ��ͣ¼��SDK
int RECORDSDK_Pause();

// �ָ�¼��SDK
int RECORDSDK_Resume();

// ����֡���ݵ�¼��SDK
int RECORDSDK_SendAVData(int channel, void *buffer, int size);

// ¼��SDK���Ʋ���
int RECORDSDK_Operate(void *inParam, void *outParam, int *outSize);

#endif

