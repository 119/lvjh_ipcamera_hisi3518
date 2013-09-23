/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�fileQuery.h
* �ļ�˵�������ļ�������¼���ļ���ѯ�ĺ�������
*           ������
*           1��¼���ļ��ڵ�����ݽṹ����
*           2��¼���ļ���������ݽṹ����
*           3��¼���ļ���ѯ�ĺ�������
* ��    �ߣ�ׯ�ݱ�
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

#ifndef __FILE_QUERY_H__
#define __FILE_QUERY_H__

#ifndef MAX_PATH
#define MAX_PATH 	256
#endif

//#define PTHREAD_MUTEX_FAST_NP 0

typedef struct FILE_NODE
{
    char fullname[128];			//�ļ�ȫ��,����·��
    char name[16];				//�ļ���,������·��
    int  start_hour;			//��ʼʱ��-Сʱ
    int  end_hour;				//����ʱ��-Сʱ
    int	 start_minute;			//��ʼʱ��-����
    int	 end_minute;			//����ʱ��-����
    int	 size;					//�ļ��Ĵ�С
    struct  FILE_NODE	*prev;	//��һ��������
    struct  FILE_NODE  	*next;  //��һ��������
}FILE_NODE, *PFILE_NODE;

typedef struct FILES_LIST
{
    int item_count;		//�ļ��ĸ���
    FILE_NODE *cur_file_node;	//����ĵ�ǰ�ڵ�
    FILE_NODE *file_node;		//�����ͷ�ڵ�

    pthread_mutex_t	mutex;			//��������Ļ�����
}FILE_LIST;

typedef struct
{
 	unsigned long nYear;
	unsigned long nMonth;
	unsigned long nDay;
	unsigned long nType;
	unsigned long nNum;
	unsigned long nReserve;
	
}ACK_RECFILE;

/* export functions for file search when play record */
//FILE_LIST *open_file_search(int channel,int year, int month, int day, int start_hour,int start_minute,int start_second,
//                            int end_hour,int end_minute,int end_second);
//FILE_LIST * open_file_search(char *path,int start_hour,int start_minute,int start_second,
//			int end_hour,int end_minute,int end_second);

FILE_LIST *open_file_search(int channel, int play_backup_files, int year, int month, int day, int start_hour,int start_minute,int start_second,
                            int end_hour,int end_minute,int end_second);
FILE_LIST *open_file_search_by_type(int channel,int year,int month,int day,unsigned int record_type);
FILE_LIST *open_bak_file_search(int disk_type, int disk_no, int channel, int year, int month, int day, unsigned int record_type);
FILE_LIST * open_decoder_protocal_file_search();

int get_file_count(FILE_LIST *file_list);
int get_file_size_by_index(FILE_LIST *file_list,int *file_size,int index);
int get_file_name_by_index(FILE_LIST *file_list,char *file_name,int index);
int get_first_file_name(FILE_LIST *file_list,char *file_name);
int get_last_file_name(FILE_LIST *file_list,char *file_name);
int get_prev_file_name(FILE_LIST *file_list,char *file_name);
int get_next_file_name(FILE_LIST *file_list,char *file_name);
int get_file_index_by_name(FILE_LIST *file_list,char *file_name);
int close_file_search(FILE_LIST *file_list);
int find_record_file_by_type(int channel,int year,int month,int day,unsigned int record_type);
int get_file_attr_by_index(FILE_LIST *file_list, char *file_name, int *file_size, int *play_time, int index);

#endif /* __FILE_QUERY_H__ */

