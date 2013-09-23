/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�indexFile.h
* �ļ�˵�������ļ������˲���¼�������ļ��ĺ�������
* ��    �ߣ�ׯ�ݱ�
*           ������
*		1������¼���ļ����ĺ궨��
*		2.	RECORD_FILE_INDEX���ݽṹ�Ķ���
*		3������¼�������ļ��ĺ�������
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
 
#ifndef __FILE_INDEX_H_
#define __FILE_INDEX_H_

#include <pthread.h>
#include "indexFile.h"

typedef struct _fileIndexInfo
{
	char cur_disk[32];				//��������ļ��ĵ�ǰ����
	char next_disk[32];			//��������ļ�����һ������
	char index_name[32];			//��ּ�ļ���	
	char disk_mount_flag[8];		//Ӳ�̷���MOUNT��־
	pthread_mutex_t index_mutex;	//�����ļ������Ļ����ź�
}fileIndexInfo;

int write_record_index_file(char *file_name, int minute, int file_type);
int search_record_index_file_by_type(char *file_name, int file_type);
int read_record_index_file(char *file_name, RECORD_FILE_INDEX *index);

#endif

