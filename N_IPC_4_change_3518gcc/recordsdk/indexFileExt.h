/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�indexFileExt.h
* �ļ�˵�������ļ������˲���¼�������ļ��ĺ�������
* ��    �ߣ�ׯ�ݱ�
*           ������
*           1������¼���ļ����ĺ궨��
*			2. RECORD_FILE_INDEX���ݽṹ�Ķ���
*           3������¼�������ļ��ĺ�������
* �汾��Ϣ��V1.0
* �������: 2008-04-29
* �޸ļ�¼:
*   �޸�1      ��    ��:
*              ��    ��:
*              �޸�����:
*   �޸�:2     ��    ��:
*              ��    ��:
*              �޸�����:
* ����˵��: ��
******************************************************************************/
 
#ifndef __INDEX_FILE_EXT_H_
#define __INDEX_FILE_EXT_H_

#define MAX_RECORD_FILE_NUM	60

typedef struct
{
	unsigned long nSize;
	unsigned long nPlayTime;
	char szFileName[64];
	unsigned long nType;
	
}REC_FILE_INFO;

typedef struct __RECORD_FILE_INDEX_EXT
{
	REC_FILE_INFO recFileInfo[MAX_RECORD_FILE_NUM];		// �������������ʱ��ķ����� 
}RECORD_FILE_INDEX_EXT;

int write_record_index_file_ext(char *indexFile, int minute, REC_FILE_INFO recFileInfo);
int read_record_index_file_ext(char *indexFile, RECORD_FILE_INDEX_EXT *indexInfo);
int search_record_index_file_by_type_ext(char *indexFile, int file_type);

int write_jpeg_index_file(char *indexFile, REC_FILE_INFO recFileInfo);
int read_jpeg_index_file(char *indexFile, char *recFileInfo);
int search_jpeg_index_file_by_type(char *indexFile, int file_type);

#endif

