/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�mp4File.h
* �ļ�˵�������ļ�������¼���ļ����ݲ���
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
 

#ifndef __FILE_BAK_H_
#define __FILE_BAK_H_

typedef struct
{
	int channel;
	int year;
	int month;
	int day;
	int start_hour;
	int start_minute;
	int end_hour;
	int end_minute;
	int disk_type;	
	int dst_disk_no;
	
}FILE_BACKUP_INFO;

int start_backup_proc(int channel,int year,int month,int day,
					  int start_hour,int start_minute,
					  int end_hour,int end_minute,
					  int disk_type,int dst_disk_no);
int get_total_file_num();
int get_cur_file_no();
int end_backup_proc();
char *get_iso_back_dir();

#endif




 
