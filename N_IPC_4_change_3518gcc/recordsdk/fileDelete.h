/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�fileDelete.h
* �ļ�˵�������ļ�������¼���ļ�ɾ���ĺ�������
*           ������
*           1��¼���ļ���Ϣ�����ݽṹ����
*           2��¼���ļ�ɾ���ĺ�������
* ��    �ߣ�ׯ�ݱ�
* �汾��Ϣ��V1.0
* �������: 2007-02-05
* �޸ļ�¼:
*   �޸�1      ��    ��:
*              ��    ��:
*              �޸�����:
*   �޸�:2     ��    ��:
*              ��    ��:
*              �޸�����:
* ����˵��: ��
******************************************************************************/

#ifndef __FILE_DELETE_H_
#define __FILE_DELETE_H_

#define MAX_TEXT_INFO_LEN			64
#define RECORD_FILE_NAME_LEN 		10

typedef struct __RECORD_FILE_INFO
{
	char text[MAX_TEXT_INFO_LEN];
	struct __RECORD_FILE_INFO *next;

}RECORD_FILE_INFO, *RECORD_FILE_INFO_LIST;

//����ֵ���壺
//	0:ɾ���ɹ�
//	1:����ɾ��
//�����붨��:
//  -1:get_oldest_date����
//  -2:��Ѱ���������ļ��������ճ���
//  -3:����ɾ��������ļ�
//  -4:����ɾ���̳߳���

int get_total_size(char *dir_name,unsigned int *size);	// ����Ŀ¼�������ļ��Ĵ�С 
int del_oldest_record_file();						// ����Ϊ��λɾ�����ϵ�¼���ļ� 
int del_oldest_record_file_ext();
int open_search_record_file_info();
int sort_record_file_info();						// ��¼���ļ���Ϣ�������� 
int close_search_record_file_info();
int get_record_file_info_num();
int get_record_file_info_by_index(char *text,int index);
int delete_dir(char *dir_name);
int set_del_file_status(int status);
int  get_del_file_status(void);

#endif

