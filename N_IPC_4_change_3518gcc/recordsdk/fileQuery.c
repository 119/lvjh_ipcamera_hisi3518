/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�fileQuery.c
* �ļ�˵�������ļ�������¼���ļ���ѯ�ĺ�������
* �����б�
*    ����1��
*        �������ƣ�add_file_node_to_list
*        �������ܣ����ڵ�����б�����
*    ����2��
*        �������ƣ�open_file_search
*        �������ܣ����ݸ�����·��,��ʱ�����¼���ļ�
*    ����3��
*        �������ƣ�open_file_search_by_type
*        �������ܣ������Ͳ����ļ�(���������ļ����в���)
*    ����4��
*        �������ƣ�open_bak_file_search
*        �������ܣ���Ӳ�̺������ұ����ļ�
*    ����5��
*        �������ƣ�open_all_file_search
*        �������ܣ����ݸ�����·��,���ҵõ������ļ�
*    ����6��
*        �������ƣ�get_file_count
*        �������ܣ���ȡ�ļ���
*    ����7��
*        �������ƣ�get_file_size_by_index
*        �������ܣ�ͨ��������ȡ�ļ��Ĵ�С
*    ����8��
*        �������ƣ�get_file_name_by_index
*        �������ܣ�ͨ��������ȡ�ļ���
*    ����9��
*        �������ƣ�get_first_file_name
*        �������ܣ�ͨ����һ���ļ���
*    ����10��
*        �������ƣ�get_last_file_name
*        �������ܣ�ͨ�����һ���ļ���
*    ����11��
*        �������ƣ�get_prev_file_name
*        �������ܣ�ͨ��ǰһ���ļ���
*    ����12��
*        �������ƣ�get_next_file_name
*        �������ܣ�ͨ����һ���ļ���
*    ����13��
*        �������ƣ�close_file_search
*        �������ܣ��ر��ļ�����
*    ����14��
*        �������ƣ�find_record_file_by_type
*        �������ܣ����ַ����ļ�����
*    ����15��
*        �������ƣ�find_back_file_by_type
*        �������ܣ����������ļ�
* ��    �ߣ�ׯ�ݱ�
* �汾��Ϣ��V1.0
* �������: 2007-02-07
* �޸ļ�¼:
*   �޸�1     ��    ��:
*              ��    ��:
*              �޸�����:
*   �޸�:2     ��    ��:
*              ��    ��:
*              �޸�����:
* ����˵��: ��
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "fdisk.h"
#include "fileBak.h"
#include "fileIndex.h"
#include "indexFile.h"
#include "indexFileExt.h"
#include "util.h"
#include "mp4File.h"

#include "fileQuery.h"

#ifdef SD_STORAGE
#include "sdCard.h"
#endif

#define MAX_FILE_NAME	256


/******************************************************************************
* �������ƣ�add_file_node_to_list
* �������������ڵ�����б�����(�����ļ���)
* ���������FILE_NODE *file_node	�ļ��ڵ�	
* ���������FILE_LIST *file_list	�ļ�����
* �� �� ֵ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int add_file_node_to_list(FILE_LIST *file_list, FILE_NODE *file_node)
{
	int i;
	int item_count;
	int found = 0;
	FILE_NODE *prev_node,*cur_node;

	if (file_list==NULL || file_node==NULL)
	{
		return -1;
	}

	pthread_mutex_lock(&file_list->mutex);

	item_count = file_list->item_count;
	if (item_count == 0)	//��һ���ڵ�
	{
		file_list->file_node = file_node;
		file_list->item_count++;
	}
	else if (item_count > 0)
	{
		prev_node = cur_node = file_list->file_node;
		
		//ͷ����
		for (i=0; i<item_count; i++)
		{
			//��һ���ڵ�
			if (strcmp(cur_node->fullname, file_node->fullname) >= 0 && i == 0)
			{
				file_node->next = cur_node;
				cur_node->prev = file_node;
				file_list->file_node = file_node;
				file_list->item_count++;
				found = 1;
				break;
			}
			
			//ͷ����
			else if (strcmp(cur_node->fullname,file_node->fullname) >= 0)
			{
				file_node->next = cur_node;
				cur_node->prev = file_node;
				prev_node->next = file_node;
				file_node->prev = prev_node;
				file_list->item_count++;
				found = 1;
				break;
			}
			prev_node = cur_node;
			cur_node = cur_node->next;
		}
		
		//β����
		if (found == 0)
		{
			file_node->prev = prev_node;
			prev_node->next = file_node;
			file_list->item_count++;
		}
	}
	pthread_mutex_unlock(&file_list->mutex);

	return 0;
}

/******************************************************************************
* �������ƣ�open_file_search
* �������������������ļ�����,���ݸ�����·��,��ʱ�����¼���ļ�
* ���������int channel			ָ��ͨ����
*			int play_backup_files	�ļ����ͣ����ݡ������ļ�
*			int year			ָ�����	
*			int month			ָ���·�
*			int day				ָ������
*			int start_hour		��ʼʱ��-Сʱ	
*			int start_minute	��ʼʱ��-����
*			int start_second	��ʼʱ��-��
*			int end_hour		����ʱ��-Сʱ	
*			int end_minute		����ʱ��-����
*			int end_second		����ʱ��-��
* �����������
* �� �� ֵ��FILE_LIST * �����ļ�����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
FILE_LIST *open_file_search(int channel, int play_backup_files, 
							int year, int month, int day, 
							int start_hour,int start_minute,int start_second,
							int end_hour,int end_minute,int end_second)
{
	int i = 0;
	int hd = 0;				//Ӳ��
	int disk_num = 0;
	int partition_no = 0;   //������: 0: <¼���ļ�> 1: <�����ļ�>

	int file_start_hour,file_start_minute,file_start_second;
	int file_end_hour,file_end_minute,file_end_second;
	int play_time;
	int second_num;
	char temp[16];
	char path[MAX_PATH];
	char file_path[MAX_PATH];
	char file_name[MAX_FILE_NAME];
	char file_name1[MAX_FILE_NAME];
	char file_full_name[MAX_PATH];
	char file_name_start[MAX_FILE_NAME];
	char file_name_end[MAX_FILE_NAME];
	DIR *dir = NULL;
	FILE_NODE *file_node = NULL;
	FILE_LIST *file_list = NULL;
	FILE_INFO *file_info = NULL;
	struct dirent *ptr = NULL;
	pthread_mutexattr_t attr;

	partition_no = play_backup_files;
	if (partition_no !=0 && partition_no != 1)
	{
		partition_no = 0;
	}

#ifdef SD_STORAGE
	disk_num = sd_check();
	if (disk_num < 0)
	{
		return NULL;
	}
	disk_num = 1;
#else
	disk_num = get_hard_disk_num();
	if (disk_num <= 0)
	{
		return NULL;
	}
#endif

	sprintf(file_name_start, "%02d-%02d-%02d.tds", start_hour, start_minute, start_second);
	sprintf(file_name_end, "%02d-%02d-%02d.tds", end_hour, end_minute, end_second);

	if (strncmp(file_name_start, file_name_end, 20) > 0)
	{
		return NULL;
	}

	file_list = (FILE_LIST *)malloc(sizeof(FILE_LIST));
	if (file_list == NULL)
	{
		return NULL;
	}
	memset(file_list, 0, sizeof(file_list));

	//�����������
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_FAST_NP);	//PTHREAD_MUTEX_FAST_NP PTHREAD_MUTEX_RECURSIVE_NP �ݹ黥�� 2005.4.4 [ZHB]
	pthread_mutex_init(&file_list->mutex, &attr);
	pthread_mutexattr_destroy(&attr);
	
	//��������Ӳ��
	for (hd=0; hd<disk_num; hd++)
	{
#ifdef SD_STORAGE
		if (!sd_get_mount_flag())
		{
			continue;
		}
#else
		if (!hd_get_mount_flag(hd, partition_no))
		{
			continue;
		}
#endif

		memset(path, 0, MAX_PATH);
		sprintf(path, "/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d", hd, partition_no, channel, year, month, day);
		for (i=start_hour; i<=end_hour; i++)
		{
			sprintf(file_path, "%s/%02d" ,path, i);
			
			//Ŀ¼������
			dir = opendir(file_path);
			if (dir == NULL)
			{
				continue;
			}

			while ((ptr = readdir(dir)) != NULL)
			{
#if 1
				//��Ŀ¼��û��¼���ļ�
				if (strcmp(ptr->d_name, ".") == 0 
					|| strcmp(ptr->d_name, "..") == 0
					|| strcmp(ptr->d_name, "index.tds") == 0)
				{
					continue;
				}
#endif
				strcpy(file_name, ptr->d_name);
				sprintf(file_full_name, "%s/%s", file_path, file_name);

				// [zhb][add][2006-04-13]
				//sscanf(file_name, "%02d-%02d-%02d.tds", &file_start_hour, &file_start_minute, &file_start_second);
				//printf("search file1 =%s %s %s %s\n", ptr->d_name, file_full_name, file_path, file_name);
				
				//Сʱ
				memcpy(temp, &file_name[0], 2);
				file_start_hour = atoi(temp);
				//����
				memcpy(temp, &file_name[3], 2);
				file_start_minute = atoi(temp);
				//��
				memcpy(temp, &file_name[6], 2);
				file_start_second = atoi(temp);
				
				//��ȡ�ļ��ڵ���Ϣ
				file_info = mp4_record_open_file(file_full_name);
				if (file_info == NULL)
				{
					printf("mp4_record_open_file() Failed!\n");
					continue;
				}
              
				play_time = mp4_record_get_play_time(file_info) / 1000;
				mp4_record_close_file(file_info);
				if (play_time <= 0)
				{
					printf("mp4_record_get_play_time() Failed!\n");
					continue;
				}

				file_end_hour = file_start_hour + play_time/3600;
				file_end_minute = file_start_hour + (play_time%3600)/60;
				file_end_second = file_start_hour + play_time%60;

				/*
				second_num = file_start_hour*60*60 + file_start_minute*60 + file_start_second + play_time;
				//printf("search file2 =%d %d %d %d %d\n", file_start_hour, file_start_minute, file_start_second, play_time, second_num);
				file_end_hour = second_num / (60*60);
				file_end_minute = (second_num - file_end_hour*60*60) / 60;
				file_end_second = second_num - file_end_hour*60*60 - file_end_minute*60;
				
				sprintf(file_name1,"%02d-%02d-%02d.tds",file_end_hour,file_end_minute,file_end_second);
				if (strcmp(file_name1,file_name_start) < 0 
					|| strcmp(file_name, file_name_start) < 0
					|| strcmp(file_name, file_name_end) > 0)
				{
					//printf("search file4 =%s %s %s\n", file_name1, file_name_start, file_name_end);
					continue;
				}
				
				//printf("search file3 =%s %d %d %d\n", file_name1, file_end_hour, file_end_minute, file_end_second);
				*/
				
				file_node = (FILE_NODE *)malloc(sizeof(FILE_NODE));
				if (file_node == NULL)
				{
					printf("Can not allocate memory for FILE_NODE!\n");
					continue;
				}
				
				strcpy(file_node->name,file_name);
				sprintf(file_node->fullname,"%s/%s",file_path,file_name);
				file_node->prev = NULL;
				file_node->next = NULL;
				add_file_node_to_list(file_list,file_node);
			}

			closedir(dir);
		}
	}

	return file_list;
}

/******************************************************************************
* �������ƣ�open_file_search_by_type
* ���������������Ͳ����ļ�(���������ļ����в���)
* ���������int channel				ָ��ͨ����
*			int play_backup_files	�ļ����ͣ����ݡ������ļ�
*			int year				ָ�����	
*			int month				ָ���·�
*			int day					ָ������
* �����������
* �� �� ֵ��FILE_LIST * �����ļ�����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
#ifndef FILESIZE_COMPUTE
FILE_LIST *open_file_search_by_type(int channel,int year,int month,int day,unsigned int record_type)
{
	int i,j,k;
	int flag = 0;
	int start_minute,end_minute;
	char index_file_name[MAX_PATH];
	FILE_NODE *file_node = NULL;
	FILE_LIST *file_list = NULL;
	RECORD_FILE_INDEX index;
	pthread_mutexattr_t attr;
	int disk_num;
	int temp_end=0;

	char file_name[MAX_PATH];
	struct stat st;
	int size = 0;
	int m,s;

	start_minute = end_minute = 0;

	file_list = (FILE_LIST *)malloc(sizeof(FILE_LIST));
	if(file_list == NULL)
		return NULL;
	memset(file_list,0,sizeof(file_list));

	//�����������
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_FAST_NP);		//PTHREAD_MUTEX_FAST_NP PTHREAD_MUTEX_RECURSIVE_NP �ݹ黥�� 2005.4.4 [ZHB]
	pthread_mutex_init(&file_list->mutex,&attr);
	pthread_mutexattr_destroy(&attr);

#ifdef SD_STORAGE
	disk_num = sd_check();
	if (disk_num < 0)
	{
		return NULL;
	}
	disk_num = 1;
#else
	disk_num = get_hard_disk_num();
#endif

	for (i=0; i<disk_num; i++)
	{
#ifdef SD_STORAGE
		if (!sd_get_mount_flag())
		{
			continue;
		}
#else
		if (!hd_get_mount_flag(i, 0))
		{
			continue;
		}
#endif

		// �ļ��б���ֻ����ʱ���(���嵽����ʱ�ٸ���ʱ������ҵ���Ӧ��¼���ļ�) 
		for (j=0; j<24; j++) 
		{
#if 1
			sprintf(index_file_name,"/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d/%02d/index.db",
						i,0,channel,year,month,day,j);

			if (read_record_index_file(index_file_name,&index) == -1)
			{
				continue ;
			}

			// ����һ��Сʱ��ͬһ�����ж��ٸ�����¼���ļ��� 
			flag = 0;
			
			for (k=0; k<60; k++)
			{
				if ((index.file_type[k] & record_type) != 0)
				{
					temp_end = k;
					if (flag == 0)		// �ҵ���һ���ļ�
					{
						start_minute = k;
						flag = 1;
						size = 0;
					}

					if (flag == 1 && k == 59)
					{
						flag = 0;
						end_minute = k;

						file_node = (FILE_NODE *)malloc(sizeof(FILE_NODE));
						if (file_node == NULL)
						{
							continue;
						}
						
#ifdef FILESIZE_COMPUTE
						for (m=start_minute; m<=end_minute; m++)
						{
							for (s=0; s<60; s++)
							{
								sprintf(file_name, 
									"/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d/%02d/%02d-%2d-%02d.tds",i,0,channel,year,month,day,j,j,m,s);

								#if 0
								if(stat(file_name, &st) < 0)
									printf("LIB: stat error; may the file not exit \n");
								else
								#endif

								if (stat(file_name, &st) == 0)
								{
									size += st.st_size;
									break;
								}
							}
						}

						file_node->size = size /1024; // size k
						size = 0;
#endif
						sprintf(file_node->fullname,"%02d:%02d --- %02d:%02d",
									j,start_minute,j,end_minute);
						file_node->start_hour = j;
						file_node->end_hour = j;
						file_node->start_minute = start_minute;
						file_node->end_minute = end_minute;
						file_node->prev = NULL;
						file_node->next = NULL;
						add_file_node_to_list(file_list,file_node);
					}
				}
				else
				{
					if (flag == 1)
					{
						flag = 0;
						end_minute = temp_end;

						file_node = (FILE_NODE *)malloc(sizeof(FILE_NODE));
						if (file_node == NULL)
						{
							continue;
						}

#ifdef FILESIZE_COMPUTE
						for (m=start_minute; m<=end_minute; m++)
						{
							for (s=0; s<60; s++)
							{
								sprintf(file_name, "/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d/%02d/%02d-%2d-%02d.tds",i,0,channel,year,month,day,j,j,m,s);

								#if 0
                                                                if(stat(file_name, &st) < 0)
                                                                        printf("LIB: stat error; may the file not exit \n");
                                                                else
								#endif

								if(stat(file_name, &st) == 0)
								{
									size += st.st_size;
									break;
								}
							}
						}

						file_node->size = size /1024; // size k
						size = 0;
#endif

						sprintf(file_node->fullname,"%02d:%02d --- %02d:%02d",j,start_minute,j,end_minute);
						file_node->start_hour = j;
						file_node->end_hour = j;
						file_node->start_minute = start_minute;
						file_node->end_minute = end_minute;
						file_node->prev = NULL;
						file_node->next = NULL;
						add_file_node_to_list(file_list,file_node);
					}
				}
			}
#endif
		}
	}

	return file_list;
}
#endif

/******************************************************************************
* �������ƣ�open_file_search_by_type
* ���������������Ͳ����ļ�(���������ļ����в���) ,ͬʱ����ʱ������ļ���С
* ���������int channel				ָ��ͨ����
*			int year				ָ�����	
*			int month				ָ���·�
*			int day					ָ������
*			unsigned int record_type¼������
* �����������
* �� �� ֵ��FILE_LIST * �����ļ�����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
#ifdef FILESIZE_COMPUTE
FILE_LIST *open_file_search_by_type(int channel,int year,int month,int day,unsigned int record_type)
{
	int i,j,k;
	int flag = 0;
	int start_minute,end_minute;
	char index_file_name[MAX_PATH];
	FILE_NODE *file_node = NULL;
	FILE_LIST *file_list = NULL;
	RECORD_FILE_INDEX index;
	pthread_mutexattr_t attr;
	int disk_num;
	int temp_end=0;

	struct stat st;
	int size = 0;
	//int m,s;
        char path[MAX_PATH];
        char file_path[MAX_PATH];
        char file_full_name[MAX_PATH];
        char file_name[MAX_FILE_NAME];
        //char file_name_end[MAX_FILE_NAME];
        DIR *dir = NULL;
        struct dirent *ptr;
	char temp[12];
        //int file_start_hour,file_start_minute,file_start_second;
	//int file_end_hour,file_end_minute,file_end_second;
	int file_start_minute;


	start_minute = end_minute = 0;

	file_list = (FILE_LIST *)malloc(sizeof(FILE_LIST));
	if(file_list == NULL)
		return NULL;
	memset(file_list,0,sizeof(file_list));

	//�����������
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_FAST_NP);		//PTHREAD_MUTEX_FAST_NP PTHREAD_MUTEX_RECURSIVE_NP �ݹ黥�� 2005.4.4 [ZHB]
	pthread_mutex_init(&file_list->mutex,&attr);
	pthread_mutexattr_destroy(&attr);

#ifdef SD_STORAGE
	disk_num = sd_check();
	if (disk_num < 0)
	{
		return NULL;
	}
	disk_num = 1;
#else
	disk_num = get_hard_disk_num();
#endif

	for(i=0; i<disk_num; i++)
	{
#ifdef SD_STORAGE
		if (!sd_get_mount_flag())
		{
			continue;
		}
#else
		if (!hd_get_mount_flag(i,0))
		{
			continue;
		}
#endif

		sprintf(path,"/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d",i,0,channel,year,month,day);

		for(j=0; j<24; j++) /* �ļ��б���ֻ����ʱ���(���嵽����ʱ�ٸ���ʱ������ҵ���Ӧ��¼���ļ�) */
		{
#if 1
			sprintf(index_file_name,"/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d/%02d/index.db",
						i,0,channel,year,month,day,j);
			if(read_record_index_file(index_file_name,&index) == -1)
				continue ;
			sprintf(file_path,"%s/%02d",path,j);
			dir = opendir(file_path);
			if(dir == NULL)
			{
				continue;
			}

			// ����һ��Сʱ��ͬһ�����ж��ٸ�����¼���ļ��� 
			flag = 0;
			for(k=0; k<60; k++)
			{
				if((index.file_type[k] & record_type) != 0)
				{
					temp_end = k;
					if(flag == 0)
					{
						start_minute = k;
						flag = 1;
						//size = 0;
					}

					if(flag == 1 && k == 59)
					{
						flag = 0;
						end_minute = k;

						file_node = (FILE_NODE *)malloc(sizeof(FILE_NODE));
						if(file_node == NULL)
							continue;

#ifdef FILESIZE_COMPUTE
						printf("LIB: hour: %d; start_minute: %d;  end_minute: %d\n", j,start_minute, end_minute);
						size = 0;
						rewinddir(dir);
						while((ptr = readdir(dir)) != NULL)
						{
							if(strcmp(ptr->d_name,".") == 0 || strcmp(ptr->d_name,"..") == 0
								|| strcmp(ptr->d_name,"index.db") == 0) // zym 0602
							{
								continue;
							} 
							strcpy(file_name,ptr->d_name);
							memcpy(temp,&file_name[3],2);
							file_start_minute = atoi(temp);

							if(file_start_minute >= start_minute &&
								file_start_minute <= end_minute)
							{
                            	//int ret;
								//printf("LIB: file_start_minute == %d\n", file_start_minute);
								sprintf(file_full_name,"%s/%s",file_path,file_name);

								if(stat(file_full_name, &st) == 0)
								{
									size += st.st_size;
								}
							}
							//if(file_start_minute >= end_minute)
							//      break;
						}

						printf("LIB: finish: size == %d\n", size);
								file_node->size = size /1024; // size k
#endif

						sprintf(file_node->fullname,"%02d:%02d --- %02d:%02d",
									j,start_minute,j,end_minute);
						file_node->start_hour = j;
						file_node->end_hour = j;
						file_node->start_minute = start_minute;
						file_node->end_minute = end_minute;
						file_node->prev = NULL;
						file_node->next = NULL;
						add_file_node_to_list(file_list,file_node);
					}
				}
				else
				{
					if(flag == 1)
					{
						flag = 0;
						end_minute = temp_end;	//k;		//cyg 2005.4.15
						//printf("open_file_search_by_type == %d  %d\n", k, end_minute);
						file_node = (FILE_NODE *)malloc(sizeof(FILE_NODE));
						if(file_node == NULL)
							continue;
#ifdef FILESIZE_COMPUTE
						printf("LIB: hour: %d; start_minute: %d;  end_minute: %d\n", j,start_minute,end_minute);
						size = 0;
						rewinddir(dir);
						while((ptr = readdir(dir)) != NULL)
						{
							if(strcmp(ptr->d_name,".") == 0 || strcmp(ptr->d_name,"..") == 0
								|| strcmp(ptr->d_name,"index.db") == 0) // zym 0602
							{
								continue;
							}
							strcpy(file_name,ptr->d_name);
							memcpy(temp,&file_name[3],2);
							file_start_minute = atoi(temp);

							if(file_start_minute >= start_minute &&
								file_start_minute <= end_minute)
							{
								//int ret;
								//printf("LIB: file_start_minute == %d\n", file_start_minute);
								sprintf(file_full_name,"%s/%s",file_path,file_name);
								
								if(stat(file_full_name, &st) == 0)
								{
									size += st.st_size;
								}
							}
							//if(file_start_minute >= end_minute)
							//	break;
						}

						printf("LIB: finish: size == %d\n", size);
								file_node->size = size /1024; // size k
#endif

						sprintf(file_node->fullname,"%02d:%02d --- %02d:%02d",j,start_minute,j,end_minute);
						file_node->start_hour = j;
						file_node->end_hour = j;
						file_node->start_minute = start_minute;
						file_node->end_minute = end_minute;
						file_node->prev = NULL;
						file_node->next = NULL;
						add_file_node_to_list(file_list,file_node);
					}
				}
			}
		closedir(dir);
	#endif
		}
	}

	return file_list;
}
#endif

/******************************************************************************
* �������ƣ�open_bak_file_search
* ������������Ӳ�̺������ұ����ļ�(�������������ļ�������)
*			��ǰ�����������ļ�������
* ���������int disk_type			ָ���洢�̵�����
*			int disk_no				ָ���洢�̺�
*			int channel				ָ��ͨ����
*			int year				ָ�����	
*			int month				ָ���·�
*			int day					ָ������
*			unsigned int record_type¼������
* �����������
* �� �� ֵ��FILE_LIST * �����ļ�����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
FILE_LIST *open_bak_file_search(int disk_type, int disk_no,int channel,int year,int month,int day,unsigned int record_type)
{
	int i,j,k;
	int flag = 0;
	int start_minute,end_minute;
	char index_file_name[MAX_PATH];
	FILE_NODE *file_node = NULL;
	FILE_LIST *file_list = NULL;
	RECORD_FILE_INDEX index;
	pthread_mutexattr_t attr;
	int	disk_num;
	int 	temp_end=0;

	start_minute = end_minute = 0;

	file_list = (FILE_LIST *)malloc(sizeof(FILE_LIST));
	if(file_list == NULL)
		return NULL;

	memset(file_list,0,sizeof(file_list));

	//�����������
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_FAST_NP);		//PTHREAD_MUTEX_FAST_NP PTHREAD_MUTEX_RECURSIVE_NP �ݹ黥�� 2005.4.4 [ZHB]
	pthread_mutex_init(&file_list->mutex,&attr);
	pthread_mutexattr_destroy(&attr);

	if(disk_type == 0)  				//���ݵ�Ӳ��
	{
		disk_num = get_hard_disk_num();
	}
	else if(disk_type == 1 || disk_type == 2)	//����U�� & ���ݵ��������µ��ض�Ŀ¼;Ϊ��¼
	{
		disk_num = 1;
	}
	else
	{
		disk_num = 0;
	}

	for(i=0; i<disk_num; i++)
	{
		if(disk_type == 0)
		{
			if(i != disk_no)
				continue;

			if(!hd_get_mount_flag(i,1))
				continue;
		}

		for(j=0; j<24; j++) /* �ļ��б���ֻ����ʱ���(���嵽����ʱ�ٸ���ʱ������ҵ���Ӧ��¼���ļ�) */
		{
	#if 1
			if(disk_type == 0)	//Ӳ��
			{
				sprintf(index_file_name,"/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d/%02d/index.db", i,1,channel,year,month,day,j);
				printf("open_bak_file_search(hd%d)  path = %s\n", disk_type, index_file_name);
			}
			else if(disk_type == 1) //U��
			{
				sprintf(index_file_name,"/usb%01d/ch%02d/%04d-%02d-%02d/%02d/index.db", disk_no, channel, year, month, day, j);
				printf("open_bak_file_search(ud%d)  path = %s\n", disk_type, index_file_name);
			}
			else			//��¼�ݴ�Ŀ¼
			{
				sprintf(index_file_name,"%sch%02d/%04d-%02d-%02d/%02d/index.db",  get_iso_back_dir(), channel, year, month, day, j);
				printf("open_bak_file_search(rd%d)  path = %s\n", disk_type, index_file_name);
			}

			if(read_record_index_file(index_file_name,&index) == -1)
				continue ;
 
			/* ����һ��Сʱ��ͬһ�����ж��ٸ�����¼���ļ��� */
			flag = 0;
			for(k=0; k<60; k++)
			{
				if((index.file_type[k] & record_type) != 0)
				{
					temp_end = k;
					if(flag == 0)
					{
						start_minute = k;
						flag = 1;
					}

					if(flag == 1 && k == 59)
					{
						flag = 0;
						end_minute = k;

						file_node = (FILE_NODE *)malloc(sizeof(FILE_NODE));
						if(file_node == NULL)
							continue;

						sprintf(file_node->fullname,"%02d:%02d --- %02d:%02d",j,start_minute,j,end_minute);
						file_node->start_hour = j;
						file_node->end_hour = j;
						file_node->start_minute = start_minute;
						file_node->end_minute = end_minute;
						file_node->prev = NULL;
						file_node->next = NULL;
						add_file_node_to_list(file_list,file_node);
					}
				}
				else
				{
					if(flag == 1)
					{
						flag = 0;
						end_minute = temp_end;	//k;		//cyg 2005.4.15
						file_node = (FILE_NODE *)malloc(sizeof(FILE_NODE));
						if(file_node == NULL)
							continue;
						sprintf(file_node->fullname,"%02d:%02d --- %02d:%02d",j,start_minute,j,end_minute);
						file_node->start_hour = j;
						file_node->end_hour = j;
						file_node->start_minute = start_minute;
						file_node->end_minute = end_minute;
						file_node->prev = NULL;
						file_node->next = NULL;
						add_file_node_to_list(file_list,file_node);
					}
				}
			}
	#endif
		}
	}

	return file_list;
}

/******************************************************************************
* �������ƣ�open_all_file_search
* �������������ݸ�����·��,���ҵõ������ļ�
* ���������char *path	������·��
* �����������
* �� �� ֵ��FILE_LIST * �����ļ�����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
FILE_LIST *open_all_file_search(char *path)
{
	char file_name[MAX_FILE_NAME];
	char file_full_name[MAX_PATH];
	DIR *dir = NULL;
	FILE_NODE *file_node = NULL;
	FILE_LIST *file_list = NULL;
	struct dirent *ptr;
	pthread_mutexattr_t attr;

	file_list = (FILE_LIST *)malloc(sizeof(FILE_LIST));
	if(file_list == NULL)
		return NULL;;
	memset(file_list,0,sizeof(file_list));

	//�����������
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_FAST_NP);		//PTHREAD_MUTEX_FAST_NP PTHREAD_MUTEX_RECURSIVE_NP �ݹ黥�� 2005.4.4 [ZHB]
	pthread_mutex_init(&file_list->mutex,&attr);
	pthread_mutexattr_destroy(&attr);

	//printf("## path == %s\n", path);
		dir = opendir(path);
		if(dir == NULL)
		{
			return NULL;
		}

		while((ptr = readdir(dir)) != NULL)
		{
			if(strcmp(ptr->d_name,".") == 0 || strcmp(ptr->d_name,"..") == 0 
                      || strcmp(ptr->d_name,"index.db") == 0) // zym 0602
            {
				continue;
            }

			strcpy(file_name,ptr->d_name);
			sprintf(file_full_name,"%s/%s",path,file_name);

			file_node = (FILE_NODE *)malloc(sizeof(FILE_NODE));
			if(file_node == NULL)
				continue;
			strcpy(file_node->name,file_name);
                        strcpy(file_node->fullname,file_name);
			//sprintf(file_node->fullname,"%s/%s",path,file_name);
			file_node->prev = NULL;
			file_node->next = NULL;
			//printf("## file_node->fullname == %s\n", file_node->fullname);
			add_file_node_to_list(file_list,file_node);
		}

		closedir(dir);

	return file_list;
}

/******************************************************************************
* �������ƣ�open_decoder_protocal_file_search
* �������������ҵõ�Э���ļ�
* �����������
* �����������
* �� �� ֵ��FILE_LIST * �����ļ�����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
FILE_LIST * open_decoder_protocal_file_search() // zym
{
	//int i;
	FILE_NODE *node, *cur_node;

        char path[20] = "/Protocol";
        FILE_LIST *file_list = open_all_file_search(path);
	
	if(file_list == NULL)
	{
		return NULL;
	}

//	for(i=0; i<get_file_count(file_list); i++)
	node = file_list->file_node;
	cur_node = node;
	while(cur_node)
	{
		if( (node->fullname[strlen(node->fullname)-4] == '.'
						&& node->fullname[strlen(node->fullname)-3] == 'c'
						&& node->fullname[strlen(node->fullname)-2] == 'o'
						&& node->fullname[strlen(node->fullname)-1] == 'd')
						||
						(node->fullname[strlen(node->fullname)-4] == '.'
			                        && node->fullname[strlen(node->fullname)-3] == 'C'
                        			&& node->fullname[strlen(node->fullname)-2] == 'O'
			                        && node->fullname[strlen(node->fullname)-1] == 'D'))

			node->fullname[strlen(node->fullname)-4] = '\0';
		//printf("node->fullname == %s\n", node->fullname);
		node = cur_node->next;
	        cur_node = node;
	}


        return file_list;
}

/******************************************************************************
* �������ƣ�get_file_count
* ������������ȡ�ļ���(����ϴβ�ѯ)
* �����������
* �����������
* �� �� ֵ������ֵ���ļ���
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_file_count(FILE_LIST *file_list)
{

	if (file_list == NULL)
		return 0;
		
	return file_list->item_count;
}

/******************************************************************************
* �������ƣ�get_file_size_by_index
* ����������ͨ��������ȡ�ļ��Ĵ�С
* ���������FILE_LIST *file_list	�ļ�����
*			int index				�ļ�����
* ���������int *file_size			�ļ���С
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_file_size_by_index(FILE_LIST *file_list,int *file_size,int index)
{
        int i;
        int file_count;
	//int start_hour,start_minute,end_hour,end_minute;
        //DIR *dir = NULL;
        //struct dirent *ptr;
	

        FILE_NODE *file_node = NULL;
        if(file_list == NULL)
                return -1;

        pthread_mutex_lock(&file_list->mutex);

        file_count = file_list->item_count;
        if(index < 0 || index >= file_count)
        {
                pthread_mutex_unlock(&file_list->mutex);
                return -1;
        }

        file_node = file_list->file_node;
        for(i=0; i<index; i++)
        {
                if(file_node == NULL)
                        break;
                file_node = file_node->next;
        }

        if(file_node != NULL)
        {
                file_list->cur_file_node = file_node;
                //strcpy(file_name,file_node->fullname);

		//sscanf(file_node->fullname,"%02d:%02d --- %02d:%02d",&start_hour,&start_minute,&end_hour,&end_minute);	

		
		*file_size = file_node->size;

                pthread_mutex_unlock(&file_list->mutex);
                return 0;
        }

        pthread_mutex_unlock(&file_list->mutex);
        return -1;
}

/******************************************************************************
* �������ƣ�get_file_name_by_index
* ����������ͨ��������ȡ�ļ���(����ϴβ�ѯ)
* ���������FILE_LIST *file_list	�ļ�����
*			int index				�ļ�����
* ���������char *file_name			�ļ���
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_file_name_by_index(FILE_LIST *file_list, char *file_name, int index)
{
	int i;
	int file_count;
	FILE_NODE *file_node = NULL;
	if(file_list == NULL)
		return -1;

	pthread_mutex_lock(&file_list->mutex);

	file_count = file_list->item_count;
	if(index < 0 || index >= file_count)
	{
		pthread_mutex_unlock(&file_list->mutex);
		return -1;
	}

	file_node = file_list->file_node;
	for(i=0; i<index; i++)
	{
		if(file_node == NULL)
			break;
		file_node = file_node->next;
	}

	if(file_node != NULL)
	{
		file_list->cur_file_node = file_node;
		strcpy(file_name,file_node->fullname);

		pthread_mutex_unlock(&file_list->mutex);
		return 0;
	}

	pthread_mutex_unlock(&file_list->mutex);
	return -1;
}

int get_file_attr_by_index(FILE_LIST *file_list, char *file_name, int *file_size, int *play_time, int index)
{
	int i;
	int file_count;
	struct stat st;
	FILE_NODE *file_node = NULL;

	if (file_list == NULL)
	{
		return -1;
	}

	pthread_mutex_lock(&file_list->mutex);

	file_count = file_list->item_count;
	if(index < 0 || index >= file_count)
	{
		pthread_mutex_unlock(&file_list->mutex);
		return -1;
	}

	file_node = file_list->file_node;
	for(i=0; i<index; i++)
	{
		if(file_node == NULL)
			break;
		file_node = file_node->next;
	}

	if (file_node != NULL)
	{
		FILE_INFO *file_info = NULL;
		
		file_list->cur_file_node = file_node;

		strcpy(file_name, file_node->fullname);

		if (stat(file_node->fullname, &st) == 0)
		{
			*file_size = st.st_size;
		}
		else
		{
			*file_size = 0;
		}

		file_info = mp4_record_open_file(file_node->fullname);
		if (file_info != NULL)
		{
			*play_time = mp4_record_get_play_time(file_info);	
			mp4_record_close_file(file_info);
		}
		else
		{
			printf("mp4_record_open_file(): Failed.\n");
			*play_time = 0;
		}

		pthread_mutex_unlock(&file_list->mutex);
		return 0;
	}

	pthread_mutex_unlock(&file_list->mutex);
	return -1;
}

/******************************************************************************
* �������ƣ�get_file_index_by_name
* ����������ͨ���ļ�����ȡ����ֵ(����ϴβ�ѯ)
* ���������FILE_LIST *file_list	�ļ�����
*			char *file_name			�ļ���
* �����������
* �� �� ֵ������ֵ���ļ�����ֵ
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_file_index_by_name(FILE_LIST *file_list,char *file_name)
{
        int i;
        int file_count;
        FILE_NODE *file_node = NULL;
        if(file_list == NULL)
                return -1;

	pthread_mutex_lock(&file_list->mutex);

        file_count = file_list->item_count;

        file_node = file_list->file_node;

        for(i=0; i<file_count; i++)
        {
                if(file_node == NULL)
                        break;
		if( strcmp(file_node->fullname,file_name)==0 )
		{
			pthread_mutex_unlock(&file_list->mutex);
			return i;
		}
                file_node = file_node->next;
        }

	pthread_mutex_unlock(&file_list->mutex);
        return -1;
}

/******************************************************************************
* �������ƣ�get_first_file_name
* ������������ȡ��һ���ļ���(����ϴβ�ѯ)
* ���������FILE_LIST *file_list	�ļ�����			
* ���������char *file_name			�ļ���
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_first_file_name(FILE_LIST *file_list,char *file_name)
{
	FILE_NODE *file_node;
        if(file_list == NULL)
                return -1;

	pthread_mutex_lock(&file_list->mutex);
	file_node = file_list->file_node;
	if(file_node == NULL)
	{
		pthread_mutex_unlock(&file_list->mutex);
		return -1;
	}

	file_list->cur_file_node = file_node;
	strcpy(file_name,file_node->fullname);
	pthread_mutex_unlock(&file_list->mutex);
	return 0;
}

/******************************************************************************
* �������ƣ�get_last_file_name
* ������������ȡ���һ���ļ���(����ϴβ�ѯ)
* ���������FILE_LIST *file_list	�ļ�����			
* ���������char *file_name			�ļ���
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_last_file_name(FILE_LIST *file_list,char *file_name)
{
	int i;
	int item_count;
	FILE_NODE *file_node;

	if(file_list == NULL)
                return -1;
	pthread_mutex_lock(&file_list->mutex);

	file_node = file_list->file_node;
	if(file_node == NULL)
	{
		pthread_mutex_unlock(&file_list->mutex);
		return -1;
	}

	item_count = file_list->item_count;
	for(i=0; i<item_count-1; i++)
	{
		if(file_node == NULL)
			break;
		file_node = file_node->next;
	}

	if(file_node != NULL)
	{
		strcpy(file_name,file_node->fullname);
		pthread_mutex_unlock(&file_list->mutex);
		return 0;
	}

	pthread_mutex_unlock(&file_list->mutex);
	return -1;
}

/******************************************************************************
* �������ƣ�get_prev_file_name
* ������������ȡ��һ���ļ���(����ϴβ�ѯ)
* ���������FILE_LIST *file_list	�ļ�����			
* ���������char *file_name			�ļ���
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_prev_file_name(FILE_LIST *file_list,char *file_name)
{
	if(file_list == NULL)
                return -1;
	pthread_mutex_lock(&file_list->mutex);

	file_list->cur_file_node = file_list->cur_file_node->prev;
	if(file_list->cur_file_node == NULL)
	{
		pthread_mutex_unlock(&file_list->mutex);
		return -1;
	}

	strcpy(file_name,file_list->cur_file_node->fullname);
	pthread_mutex_unlock(&file_list->mutex);
	return 0;
}

/******************************************************************************
* �������ƣ�get_next_file_name
* ������������ȡ��һ���ļ���(����ϴβ�ѯ)
* ���������FILE_LIST *file_list	�ļ�����			
* ���������char *file_name			�ļ���
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_next_file_name(FILE_LIST *file_list,char *file_name)
{
	if(file_list == NULL)
                return -1;
	pthread_mutex_lock(&file_list->mutex);

	file_list->cur_file_node = file_list->cur_file_node->next;
	if(file_list->cur_file_node == NULL)
	{
		pthread_mutex_unlock(&file_list->mutex);
		return -1;
	}

	strcpy(file_name,file_list->cur_file_node->fullname);
	pthread_mutex_unlock(&file_list->mutex);
	return 0;
}

/******************************************************************************
* �������ƣ�close_file_search
* �����������ر��ļ�����
* ���������FILE_LIST *file_list	�ļ�����			
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int close_file_search(FILE_LIST *file_list)
{
	int i;
	int file_num;
	FILE_NODE *file_node,*next_file_node;

	if(file_list == NULL)
                return -1;
	pthread_mutex_lock(&file_list->mutex);

	//printf("close_file_search1=%d\n", file_list->item_count);
	file_num = file_list->item_count;
	if(file_num <= 0)
	{
		//printf("close_file_search2=%d\n", file_list->item_count);
		pthread_mutex_unlock(&file_list->mutex);
		pthread_mutex_destroy(&file_list->mutex);
		free(file_list);
		file_list = NULL;
		return 0;
	}

	file_list->item_count = 0;
	file_node = file_list->file_node;
	for(i=0; i<file_num; i++)
	{
		//printf("close_file_search4=%d\n", file_list->item_count);
		if(file_node == NULL)
		{
			//printf("close_file_search3=%d\n", file_list->item_count);
			pthread_mutex_unlock(&file_list->mutex);
			pthread_mutex_destroy(&file_list->mutex);
			free(file_list);
			file_list = NULL;
			return -1;
		}
		next_file_node = file_node->next;
		free(file_node);
		file_node = NULL;
		file_node = next_file_node;
	}
	pthread_mutex_unlock(&file_list->mutex);
	pthread_mutex_destroy(&file_list->mutex);

	free(file_list);
	file_list = NULL;

	//printf("close_file_search5=%d\n", file_list->item_count);

	return 0;
}

/******************************************************************************
* �������ƣ�find_record_file_by_type
* �����������ļ�����
*			���ַ�ʽ: ���Ȳ��������ļ�,��������ڵ��������һ��һ���ļ��ķ�ʽ����
* ���������int channel				ָ��ͨ����
*			int year				ָ�����	
*			int month				ָ���·�
*			int day					ָ������
*			unsigned int record_type¼������			
* �����������
* �� �� ֵ���ɹ�: 1; ʧ��: 0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int find_record_file_by_type(int channel,int year,int month,int day,unsigned int record_type)
{
	int i,j;
	int ret;
	char index_file_name[MAX_FILE_NAME];
	int  disk_num;

	// ��������Ӳ�� 
#ifdef SD_STORAGE
	/*
	disk_num = sd_check();
	if (disk_num < 0)
	{
		return -1;
	}
	*/
	disk_num = 1;
#else
	disk_num = get_hard_disk_num();
#endif

	for(i=0; i<disk_num; i++)
	{
#ifdef SD_STORAGE

#else
		if (!hd_get_mount_flag(i,0))
		{
			continue;
		}
#endif

		for (j=0; j<24; j++)
		{
			sprintf(index_file_name,"/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d/%02d/index.db",
						i, 0, channel, year, month, day, j);
			//ret = search_record_index_file_by_type(index_file_name, record_type);
			//ret = search_record_index_file_by_type_ext(index_file_name, record_type);		// Add the code by lvjh, 2008-04-29
			ret = search_record_index_file_by_type_ext2(index_file_name, record_type);		// Add the code by lvjh, 2010-11-30
			if (ret == 1)
			{
				return 1;
			}
			else
			{

			}

#if 0
			dir = opendir(file_path);
			if(dir == NULL)
			{
				continue;
			}

			while((ptr = readdir(dir)) != NULL)
			{
				sprintf(file_full_name,"%s/%s",file_path,ptr->d_name);

				type = mp4_record_get_record_type(file_full_name);
				if(type != -1)
				{
					type &= record_type;
					if(type != 0)
						return 1;
				}
			}

#endif
		}
	}

	return 0;
}

int find_record_file_by_type_ext(int channel,int year,int month,int day,unsigned int record_type)
{
	int i,j;
	int ret;
	char index_file_name[MAX_FILE_NAME];
	int  disk_num;
	struct stat buf;

	// ��������Ӳ�� 
#ifdef SD_STORAGE
	/*
	disk_num = sd_check();
	if (disk_num < 0)
	{
		return -1;
	}
	*/
	disk_num = 1;
#else
	disk_num = get_hard_disk_num();
#endif

	for(i=0; i<disk_num; i++)
	{
#ifdef SD_STORAGE

#else
		if (!hd_get_mount_flag(i,0))
		{
			continue;
		}
#endif

		sprintf(index_file_name,"/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d",
						i, 0, channel, year, month, day);

		if (stat(index_file_name, &buf) != 0) // �ļв����� 
		{
			#if 0
			printf("stat: %s error!\n", index_file_name);
			#endif
			
			return 0;
		}
		else 
		{
			return 1;
		}
	}

	return 0;
}
/******************************************************************************
* �������ƣ�find_back_file_by_type
* �������������������ļ�
* ���������int channel				ָ��ͨ����
*			int year				ָ�����	
*			int month				ָ���·�
*			int day					ָ������
*			unsigned int record_type¼������		
* �����������
* �� �� ֵ���ɹ�: 1; ʧ��: 0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int find_back_file_by_type(int channel,int year,int month,int day,unsigned int record_type)
{
        int i,j;
        int ret;
        char index_file_name[MAX_FILE_NAME];
	int  disk_num;

        // ��������Ӳ�� 
#ifdef SD_STORAGE
	disk_num = sd_check();
	if (disk_num < 0)
	{
		return -1;
	}
	disk_num = 1;
#else
	disk_num = get_hard_disk_num();
#endif

	for (i=0; i<disk_num; i++)
	{
#ifdef SD_STORAGE
		if (!sd_get_mount_flag())
		{
			continue;
		}
#else
		if (!hd_get_mount_flag(i,0))
		{
			continue;
		}
#endif

		for(j=0; j<24; j++)
		{
			sprintf(index_file_name,"/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d/%02d/index.db", i,1,channel,year,month,day,j);
			//ret = search_record_index_file_by_type(index_file_name,record_type);
			ret = search_record_index_file_by_type_ext(index_file_name,record_type);	// Add the code by lvjh, 2008-04-29
			if (ret == 1)
			{
				return 1;
			}

#if 0
			dir = opendir(file_path);
			if(dir == NULL)
			{
				continue;
			}

			while((ptr = readdir(dir)) != NULL)
			{
				sprintf(file_full_name,"%s/%s",file_path,ptr->d_name);

				type = mp4_record_get_record_type(file_full_name);
				if (type != -1)
				{
					type &= record_type;
					if (type != 0)
						return 1;
				}
			}

        #endif
		}
	}

	return 0;
}

int open_file_search_by_index(int channel, int year, int month, int day,
							int start_hour,int start_minute,int start_second,
                            int end_hour,int end_minute,int end_second, 
							int record_type, char *pInfo, int *nSize)
{
	int i = 0;
	int j = 0;
	int n = 0;
	int nRet = -1;
	int  disk_num = 0;
	ACK_RECFILE *pAckRecFile = NULL;
	char *pRecFileInfo = NULL;

	char indexFile[64];
	RECORD_FILE_INDEX_EXT indexInfo;

	if (pInfo==NULL || nSize==NULL)
	{
		return -1;
	}

	pAckRecFile = (ACK_RECFILE *)pInfo;
	pRecFileInfo = (char *)(pInfo+sizeof(ACK_RECFILE));

	memset(pAckRecFile, 0, sizeof(ACK_RECFILE));
	
	pAckRecFile->nYear = year;
	pAckRecFile->nMonth = month;
	pAckRecFile->nDay = day;
	pAckRecFile->nType = record_type;

	// ��������Ӳ�� 
#ifdef SD_STORAGE
	/*
	disk_num = sd_check();
	if (disk_num < 0)
	{
		return -1;
	}
	*/
	disk_num = 1;
#else
	disk_num = get_hard_disk_num();
#endif

	for (i=0; i<disk_num; i++)
	{
#ifdef SD_STORAGE
		/*
		if (!sd_get_mount_flag())
		{
			continue;
		}
		*/
#else
		if (!hd_get_mount_flag(i,0))
		{
			continue;
		}
#endif

		for (j=start_hour; j<end_hour+1; j++)
		{
			memset(&indexInfo, 0, sizeof(RECORD_FILE_INDEX_EXT));

			sprintf(indexFile, "/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d/%02d/index.db", i,0,channel,year,month,day,j);
			nRet = read_record_index_file_ext(indexFile, &indexInfo);
			if (nRet < 0)
			{
				//printf("read_record_index_file_ext: %s %d\n", indexFile, nRet);
				continue;
			}

			//printf("read_record_index_file_ext: %s %d\n", indexFile, nRet);

			//for (n=start_minute; n<end_minute+1; n++)
			for (n=0; n<60; n++)
			{
				//printf("RecInfo: %d %d %x %s\n", indexInfo.recFileInfo[n].nSize, indexInfo.recFileInfo[n].nPlayTime, indexInfo.recFileInfo[n].nType, indexInfo.recFileInfo[n].szFileName);
				if (indexInfo.recFileInfo[n].nSize!=0 && indexInfo.recFileInfo[n].nPlayTime!=0 && indexInfo.recFileInfo[n].nType!=0 && strlen(indexInfo.recFileInfo[n].szFileName)>0)
				{
					REC_FILE_INFO *pRecInfo = NULL;

					//printf("RecInfo(%d %p): %d %d %x %s\n", sizeof(REC_FILE_INFO), pRecFileInfo, indexInfo.recFileInfo[n].nSize, indexInfo.recFileInfo[n].nPlayTime, indexInfo.recFileInfo[n].nType, indexInfo.recFileInfo[n].szFileName);

					pAckRecFile->nNum++;
					memcpy(pRecFileInfo, (char *)&indexInfo.recFileInfo[n], sizeof(REC_FILE_INFO));

					//pRecInfo = (REC_FILE_INFO *)pRecFileInfo;
					//printf("RecInfo(%d %p): %d %d %x %s\n", sizeof(REC_FILE_INFO), pRecInfo, pRecInfo->nSize, pRecInfo->nPlayTime, pRecInfo->nType, pRecInfo->szFileName);

					pRecFileInfo += sizeof(REC_FILE_INFO);
				}
			}
		}
	}

	if (pAckRecFile->nNum <= 0)
	{
		*nSize = sizeof(ACK_RECFILE);
		return -1;
	}

	*nSize = sizeof(ACK_RECFILE)+sizeof(REC_FILE_INFO)*pAckRecFile->nNum;

	return 0;
}

int find_jpeg_file_by_type(int channel,int year,int month,int day, unsigned int record_type)
{
	int i,j;
	int ret;
	char index_file_name[MAX_FILE_NAME];
	int  disk_num;

	// ��������Ӳ�� 
#ifdef SD_STORAGE
	/*
	disk_num = sd_check();
	if (disk_num < 0)
	{
		return -1;
	}
	*/
	disk_num = 1;
#else
	disk_num = get_hard_disk_num();
#endif

	for(i=0; i<disk_num; i++)
	{
#ifdef SD_STORAGE

#else
		if (!hd_get_mount_flag(i,0))
		{
			continue;
		}
#endif

		for (j=0; j<24; j++)
		{
			sprintf(index_file_name,"/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d/%02d/snapshot/index.db",
						i, 0, channel, year, month, day, j);

			ret = search_jpeg_index_file_by_type(index_file_name, record_type);		// Add the code by lvjh, 2008-04-29
			if (ret == 1)
			{
				return 1;
			}
			else
			{

			}
		}
	}

	return 0;
}

int open_jpeg_search_by_index(int channel, int year, int month, int day,
							int start_hour,int start_minute,int start_second,
                            int end_hour,int end_minute,int end_second, 
							int record_type, char *pInfo, int *nSize)
{
	int i = 0;
	int j = 0;
	int n = 0;
	int nRet = -1;
	int  disk_num = 0;
	ACK_RECFILE *pAckRecFile = NULL;
	char *pRecFileInfo = NULL;

	char indexFile[64];
	RECORD_FILE_INDEX_EXT indexInfo;

	if (pInfo==NULL || nSize==NULL)
	{
		return -1;
	}

	pAckRecFile = (ACK_RECFILE *)pInfo;
	pRecFileInfo = (char *)(pInfo+sizeof(ACK_RECFILE));

	memset(pAckRecFile, 0, sizeof(ACK_RECFILE));
	pAckRecFile->nYear = year;
	pAckRecFile->nMonth = month;
	pAckRecFile->nDay = day;
	pAckRecFile->nType = record_type;

	// ��������Ӳ�� 
#ifdef SD_STORAGE
	/*
	disk_num = sd_check();
	if (disk_num < 0)
	{
		return -1;
	}
	*/
	disk_num = 1;
#else
	disk_num = get_hard_disk_num();
#endif

	for (i=0; i<disk_num; i++)
	{
#ifdef SD_STORAGE
		/*
		if (!sd_get_mount_flag())
		{
			continue;
		}
		*/
#else
		if (!hd_get_mount_flag(i,0))
		{
			continue;
		}
#endif

		nRet = 0;
		for (j=start_hour; j<end_hour+1; j++)
		{
			pRecFileInfo += (nRet*sizeof(REC_FILE_INFO));
			sprintf(indexFile, "/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d/%02d/snapshot/index.db", i,0,channel,year,month,day,j);
			nRet = read_jpeg_index_file(indexFile, pRecFileInfo);
			if (nRet > 0)
			{
				pAckRecFile->nNum += nRet;
			}
		}
	}

	if (pAckRecFile->nNum <= 0)
	{
		*nSize = sizeof(ACK_RECFILE);
		return -1;
	}

	*nSize = sizeof(ACK_RECFILE)+sizeof(REC_FILE_INFO)*pAckRecFile->nNum;

	printf("open_jpeg_search_by_index: %d\n", pAckRecFile->nNum);

	return 0;
}

