/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�prerecord.h
* �ļ�˵�������ļ������˲���¼�������ļ��ĺ�������
* ��    �ߣ�ׯ�ݱ�
*           ������
*           1������¼���ļ����ĺ궨��
*			2. RECORD_FILE_INDEX���ݽṹ�Ķ���
*           3������¼�������ļ��ĺ�������
* �汾��Ϣ��V1.0
* �������: 2007-02-07
* �޸ļ�¼:
*   �޸�1      ��    ��:
*              ��    ��:
*              �޸�����:
*   �޸�:2     ��    ��:
*              ��    ��:
*              �޸�����:
* ����˵��: ��
******************************************************************************/

#ifndef __PRERECORD_H_
#define __PRERECORD_H_

int Open_prerecord(int channel, int size);
int Close_prerecord(int channel);
int Reset_prerecord(int channel);
int GetSize_prerecord(int channel);

int SendOneFrame_prerecord(int channel, void * buf, int size);
void* GetOneFrame_prerecord(int channel, void * buf, int *size);

#endif

