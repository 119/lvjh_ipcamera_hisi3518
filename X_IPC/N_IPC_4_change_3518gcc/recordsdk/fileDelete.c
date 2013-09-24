/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�fileDelete.c
* �ļ�˵�������ļ�������¼���ļ�ɾ���ĺ�������
* �����б�
*    ����1��
*        �������ƣ�set_del_file_status
*        �������ܣ�����¼���ļ�ɾ��״̬
*    ����2��
*        �������ƣ�get_del_file_status
*        �������ܣ���ȡ¼���ļ�ɾ��״̬
*    ����3��
*        �������ƣ�filter
*        �������ܣ�¼���ļ�����
*    ����4��
*        �������ƣ�get_oldest_date
*        �������ܣ���ȡ����¼���ļ�������
*    ����5��
*        �������ƣ�get_total_size
*        �������ܣ�����Ŀ¼�������ļ��Ĵ�С
*    ����6��
*        �������ƣ�delete_dir
*        �������ܣ�ɾ������Ŀ¼
*    ����7��
*        �������ƣ�del_oldest_file_fun
*        �������ܣ�����Ϊ��λɾ�����ϵ�¼���ļ�(�߳�ʵ�ֺ���)
*    ����8��
*        �������ƣ�del_oldest_record_file
*        �������ܣ���ʼɾ�����ϵ�¼���ļ�
*    ����9��
*        �������ƣ�open_search_record_file_info
*        �������ܣ�����¼���ļ���Ϣ
*    ����10��
*        �������ƣ�sort_record_file_info
*        �������ܣ���¼���ļ���Ϣ��������
*    ����11��
*        �������ƣ�close_search_record_file_info
*        �������ܣ��ر�¼���ļ���Ϣ
*    ����12��
*        �������ƣ�get_record_file_info_num
*        �������ܣ���ȡ����ɾ���������ļ���
*    ����13��
*        �������ƣ�get_record_file_info_by_index
*        �������ܣ���ȡ¼���ļ�����Ϣ(ͨ������)
* ��    �ߣ�ׯ�ݱ�
* �汾��Ϣ��V1.0
* �������: 2007-02-05
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
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>

#include "global.h"
#include "fileDelete.h"

pthread_t  g_del_file_thread;	//¼���ļ�ɾ���߳�
int  del_file_status = 0;		//¼���ļ�ɾ��״̬
static RECORD_FILE_INFO_LIST g_list = NULL;	//¼���ļ���Ϣ��������

/******************************************************************************
* �������ƣ�set_del_file_status
* ��������������¼���ļ�ɾ��״̬
* ���������int status ¼���ļ�ɾ��״̬(0:ɾ���ɹ�1:����ɾ��)
* �����������
* �� �� ֵ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int set_del_file_status(int status)
{
	del_file_status = status;

	return 0;
}

/******************************************************************************
* �������ƣ�get_del_file_status
* ������������ȡ¼���ļ�ɾ��״̬
* �����������
* �����������
* �� �� ֵ������ֵ��¼���ļ�ɾ��״̬(0:ɾ���ɹ�1:����ɾ��)
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int  get_del_file_status()
{
	return del_file_status;
}

/******************************************************************************
* �������ƣ�filter
* ����������¼���ļ����ǣ������ļ����ĳ��ȣ�����ΪRECORD_FILE_NAME_LEN
* ���������const struct dirent *dir Ŀ¼ָ��
* �����������
* �� �� ֵ��ƥ�䣺 1; ��ƥ�䣺 0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
static int filter(const struct dirent *dir)
{
	if (strlen(dir->d_name) == RECORD_FILE_NAME_LEN)
	{
		//return 0;
		return 1;
	}
	else
	{
		//return -1;
		return 0;
	}
}

/******************************************************************************
* �������ƣ�get_oldest_date
* ������������ȡ����¼���ļ�������
* �����������
* ���������int *year	����¼���ļ�������-��
*			int *month	����¼���ļ�������-��
*			int *day	����¼���ļ�������-��
* �� �� ֵ���ɹ��� -1; ʧ�ܣ� 0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
static int get_oldest_date(int *year, int *month, int *day)
{
	int i,j,k,total;
	struct dirent **namelist;
	char path_name[MAX_PATH];
	char dir_name[24];
	int  disk_num;
	int channel_num = 0;

	memset(dir_name, 0xff, sizeof(dir_name));

#ifdef SD_STORAGE
	disk_num = 1;
#else
	// ��������Ӳ�̵����з��� 
	disk_num = get_hard_disk_num();
#endif

	channel_num = get_av_channel_num();
	
	for (i=0; i<disk_num; i++)
	{
#ifdef SD_STORAGE

#else
		if (hd_get_mount_flag(i,0) != 1) 		// ֻ��Ѱ���ݷ��� 
		{
			continue;
		}
#endif

		for (j=0; j<channel_num; j++)
		{
			sprintf(path_name,"/record/hd%02d/%02d/ch%02d", i, 0, j);

			printf("%s\n", path_name);

			// ���������������ļ���,
			total = scandir(path_name, &namelist, filter, alphasort);
			if (total > 0)
			{
				printf("%s\n", namelist[0]->d_name);

				if (strcmp(dir_name, namelist[0]->d_name) > 0)
				{
					strcpy(dir_name, namelist[0]->d_name);
				}

				for (k=0; k<total; k++)
				{
					if (namelist[k] != NULL)
					{
 						free(namelist[k]);
					}
				}

				if (namelist != NULL)
				{
					free(namelist);
				}
			}
			else
			{
				printf("scandir: Failed!\n");
			}
			
			//usleep(50*1000);
		}
	}

	if (strlen(dir_name) > 0)
	{
		sscanf(dir_name, "%d-%d-%d", year, month, day);

		return 0;
	}
	else
	{
		return -1;
	}
}

static int filter_ext(const struct dirent *dir)
{
	if (dir->d_name[0] >= 48 && dir->d_name[0] <= 50)
	{
		printf("value: %s\n", dir->d_name);
		return 1;
	}
	else
	{
		return 0;
	}
}

static int get_oldest_time(int *year, int *month, int *day, int *hour)
{
	int i,j,k,total;
	struct dirent **namelist;
	char path_name[MAX_PATH];
	char dir_name[24];
	int  disk_num;
	int channel_num = 0;

	memset(dir_name, 0xFF, sizeof(dir_name));

	// ��������Ӳ�̵����з��� 
#ifdef SD_STORAGE
	disk_num = 1;
#else
	disk_num = get_hard_disk_num();
#endif

	channel_num = get_av_channel_num();
	
	for (i=0; i<disk_num; i++)
	{
#ifndef SD_STORAGE
		if (hd_get_mount_flag(i,0) != 1) 		// ֻ��Ѱ���ݷ��� 
		{
			continue;
		}
#endif

		for (j=0; j<channel_num; j++)
		{
			sprintf(path_name,"/record/hd%02d/%02d/ch%02d", i, 0, j);
			// ���������������ļ���,
			total = scandir(path_name, &namelist, filter, alphasort);
	               //  printf(" total = %d\n", total);
			if (total > 1)
			{
				if (strcmp(dir_name,namelist[0]->d_name) > 0)
				{
					printf("dir_name = %s\n", dir_name);
					strcpy(dir_name,namelist[0]->d_name);
				}

				for (k=0; k<total; k++)
				{
					if (namelist[k] != NULL)
					{
 						free(namelist[k]);
                                                namelist[k] = NULL;
					}
				}

				if (namelist != NULL)
				{
					free(namelist);
					namelist = NULL;
					
				}
			}
			else{
				memset(dir_name, 0, 24);
				//printf("dir_name = %d\n",strlen( dir_name));

			}
			//usleep(500*1000);
		}
	}

	if (strlen(dir_name) > 0)
	{

		sscanf(dir_name, "%d-%d-%d", year, month, day);

		strcat(path_name, "/");
		strcat(path_name, dir_name);
		total = scandir(path_name, &namelist, filter_ext, alphasort);
		if (total > 0)
		{
			//printf(" namelist[0]->d_name = %s\n", namelist[0]->d_name);
			sscanf(namelist[0]->d_name, "%d", hour);

			for (k=0; k<total; k++)
			{
				if (namelist[k] != NULL)
				{
 					free(namelist[k]);
				}
			}

			if (namelist != NULL)
			{
				free(namelist);
			}
		}
		else
		{
			delete_dir(path_name);
			//printf("delete_dir scussed %s!!!\n\n", path_name);
		}
		//printf("############################total = %d path_name = %s\n dir_name= %s  \n", path_name,dir_name);
		return 0;
	}
	else
	{
		return -1;
	}
}

/******************************************************************************
* �������ƣ�get_total_size
*
* ���������char *dir_name
* ���������unsigned int *size
* �� �� ֵ���ɹ���0; ʧ�ܣ�-1
* �޸ļ�¼:
* ����˵��:
********************************************************************************/
int get_total_size(char *dir_name, unsigned int *size)
{
	int ret ;
	DIR *dir;
	struct stat buf;
	struct dirent *ptr;
	char file_name[MAX_PATH];

	ret = stat(dir_name,&buf);
	
	if (S_ISDIR(buf.st_mode))
	{
		// ����Ŀ¼�������ļ��Ĵ�С 
		dir = opendir(dir_name);
		while ((ptr = readdir(dir)) != NULL)
		{
			if (strcmp(ptr->d_name,".") == 0 
				|| strcmp(ptr->d_name,"..") == 0
				/*|| strcmp(ptr->d_name,"index.tm3k") == 0*/)
			{
				continue;
			}

			sprintf(file_name, "%s/%s", dir_name, ptr->d_name);
			stat(file_name, &buf);
			if (S_ISDIR(buf.st_mode))
			{
				get_total_size(file_name,size);
			}
			else
			{
				*size += buf.st_size;
			}
		}
		closedir(dir);
	}
	else
	{
		*size += buf.st_size;
	}

	return ret;
}

/******************************************************************************
* �������ƣ�delete_dir
*
* ���������char *dir_name
*
* �� �� ֵ���ɹ���0; ʧ�ܣ�-1
* �޸ļ�¼:
* ����˵��:
********************************************************************************/
int delete_dir(char *dir_name)
{
	int ret;
	DIR *dir;
	struct stat buf;
	struct dirent *ptr;
	char file_name[MAX_PATH];

	stat(dir_name,&buf);
	
	if (S_ISDIR(buf.st_mode))
	{
		dir = opendir(dir_name);
		while ((ptr = readdir(dir)) != NULL)
		{
			if (strcmp(ptr->d_name,".") == 0 
				|| strcmp(ptr->d_name,"..") == 0)
			{
				continue;
			}

			sprintf(file_name, "%s/%s", dir_name, ptr->d_name);
			stat(file_name, &buf);

			if (S_ISDIR(buf.st_mode))
			{
				delete_dir(file_name);
			}
			else
			{
				remove(file_name);
			}
		}
		closedir(dir);
		ret = remove(dir_name);
	        printf("starting delete director 2 %s \n", file_name);
	}
	else
	{
		ret = remove(dir_name);
	}

	return ret;
}

/******************************************************************************
* �������ƣ�del_oldest_file_fun
* ��������������Ϊ��λɾ�����ϵ�¼���ļ�(�߳�ʵ�ֺ���)
*
*
*
* �޸ļ�¼:
* ����˵��:
********************************************************************************/
int del_oldest_file_fun()
{
	int  i,j;
	int  ret;
	int  year,month,day;
	char dir_name[MAX_PATH];
	int  cur_year, cur_month, cur_day;
	int  disk_num;
	int  channel_num = get_av_channel_num();

	cur_year   = get_year();	//��ȡ��ǰʱ��-
	cur_month  = get_month();	//��ȡ��ǰʱ��-
	cur_day    = get_day();		//��ȡ��ǰʱ��-

	do
	{
		ret = get_oldest_date(&year, &month, &day);
		if (ret != 0)
		{
			printf("get_oldest_date: Failed!\n");
			del_file_status = -1;
			break;
		}

		//
		if (year < 1970 || year >2100 || month < 1 || month > 12 || day < 1 || day >31 )
		{
			del_file_status = -2;
			break;
		}

		//
		if (year == cur_year && month == cur_month && day == cur_day )
		{
			del_file_status = -3;
			break;
		}

#ifdef SD_STORAGE
		disk_num = 1;
#else
		//
		disk_num = get_hard_disk_num();
#endif
		
		for (i=0; i<disk_num; i++)
		{
#ifdef SD_STORAGE

#else
			//
			if (hd_get_mount_flag(i, 0) != 1)
			{
				continue;
			}
#endif

			//
			for (j=0; j<channel_num; j++)
			{
				sprintf(dir_name,"/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d",i,0,j,year,month,day);
				printf("Delete the oldest file: %s\n", dir_name);
				delete_dir(dir_name);
			}
		}
	}while(0);

	if (del_file_status == 1)
	{
		del_file_status = 0;
	}

	//pthread_exit(NULL);

	return 0;
}

int del_oldest_file_ext_fun()
{
	int i = 0;
	int j = 0;
	int ret = 0;
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	char dir_name[MAX_PATH];
	int  cur_year, cur_month, cur_day, cur_hour;
	int  disk_num;
	int  channel_num = get_av_channel_num();

	cur_year = get_year();		//��ȡ��ǰʱ��-
	cur_month = get_month();	//��ȡ��ǰʱ��-
	cur_day = get_day();		//��ȡ��ǰʱ��-
	cur_hour = get_hour();

	do
	{
#ifdef SD_STORAGE
		
#endif

#if 1
		ret = get_oldest_time(&year, &month, &day, &hour);
		if (ret != 0)
		{
			del_file_status = -1;
			break;
		}
#endif		
		printf("the old time: %d %d %d %d\n", year, month, day, hour);
		//
		if (year < 1970 || year >2100 || month < 1 || month > 12 || day < 1 || day >31 || hour<0 || hour>=24)
		{
			del_file_status = -2;
			break;
		}
		/*
		//
		if (year == cur_year && month == cur_month && day == cur_day && hour == cur_hour)
		{
			del_file_status = -3;
			break;
		}
		*/

#ifdef SD_STORAGE
		for (j=0; j<channel_num; j++)
		{
			sprintf(dir_name,"/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d/%02d",i,0,j,year,month,day,hour);
			//printf("Delete: %s\n", dir_name);
			delete_dir(dir_name);
		}
#else
		//
		disk_num = get_hard_disk_num();
		
		for (i=0; i<disk_num; i++)
		{
			//
			if (hd_get_mount_flag(i, 0) != 1)
			{
				continue;
			}

			//
			for (j=0; j<channel_num; j++)
			{
				sprintf(dir_name,"/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d/%02d",i,0,j,year,month,day,hour);
				delete_dir(dir_name);
			}
		}
#endif
	}while(0);

	if (del_file_status == 1)
	{
		del_file_status = 0;
	}

	return 0;
}

/******************************************************************************
* �������ƣ�del_oldest_record_file
*
*
*
* �� �� ֵ���ɹ���0; ʧ�ܣ�-1
* �޸ļ�¼:
* ����˵��:
********************************************************************************/
int del_oldest_record_file()
{
	int ret = 0;

	del_file_status = 1;

#if 1
	ret = pthread_create(&g_del_file_thread, NULL, (void *)del_oldest_file_fun, NULL);
	if (ret != 0)
	{
		del_file_status = -4;
	}
#endif
	

	return ret;
}

int del_oldest_record_file_ext()
{
	int ret = 0;

	del_file_status = 1;

	ret = pthread_create(&g_del_file_thread, NULL, (void *)del_oldest_file_ext_fun, NULL);
	if (ret != 0)
	{
		del_file_status = -4;
	}

	return ret;
}

/******************************************************************************
* �������ƣ�open_search_record_file_info
*
*
*
* �� �� ֵ��0
* �޸ļ�¼:
* ����˵��:
********************************************************************************/
int open_search_record_file_info()
{
	int i,j,k,total;
	unsigned int size;
	struct dirent **namelist;
	char path_name[MAX_PATH];
	char dir_name[MAX_PATH];
	RECORD_FILE_INFO *record = NULL;
	RECORD_FILE_INFO_LIST tmp_list = NULL;
	int  language = 0;
	int  disk_num;
	int  channel_num = 0;

	channel_num = get_av_channel_num();

	while (g_list != NULL)
	{
		tmp_list = g_list->next;
		free(g_list);
		g_list = tmp_list;
	}
	tmp_list = g_list = NULL;

	memset(dir_name, 0, sizeof(dir_name));

	// ��������Ӳ�̵����з��� 
#ifdef SD_STORAGE
	disk_num = 1;
#else
	disk_num = get_hard_disk_num();
#endif
	
	for (i=0; i<disk_num; i++)
	{
#ifdef SD_STORAGE

#else
		if (hd_get_mount_flag(i, 0) != 1 )
		{
			continue;
		}
#endif

		for (j=0; j<channel_num; j++)
		{
			sprintf(path_name,"/record/hd%02d/%02d/ch%02d", i, 0, j);
			
			total = scandir(path_name,&namelist, filter, alphasort);
			
			if (total > 0)
			{
				for (k=0; k<total; k++)
				{
					size = 0;
					sprintf(dir_name,"%s/%s", path_name, namelist[k]->d_name);
					get_total_size(dir_name, &size);

					record = (RECORD_FILE_INFO *)malloc(sizeof(RECORD_FILE_INFO));
					if (record != NULL)
					{
						sprintf(record->text, "%s CH%02d HD%02d %d M",namelist[k]->d_name, (j + 1),(i+1), size/(1024*1024));
						record->next = NULL;

						if (tmp_list == NULL)
							tmp_list = g_list = record;
						else
						{
							tmp_list->next = record;
							tmp_list = record;
						}
					}
				}

				for (k=0; k<total; k++)
				{
					free(namelist[k]);
				}

				free(namelist);
			}
		}
	}

	// ��¼���ļ���Ϣ�������� 
	sort_record_file_info();

	return 0;
}

/******************************************************************************
* �������ƣ�sort_record_file_info
* ������������¼���ļ���Ϣ��������
			˼·:������ṹ�任������ṹ,���ü򵥵�ѡ�����򷽷���������
			ȱ�㣺Ч�ʲ��ߣ���һ������£�ѡ���������������ݸ�����30������
			���飺�ÿ���(��ʵ��¼������У�ÿ���ӱ���һ���ļ�������һ���ļ�����
			      ���кܶ��ļ���ԶԶ����30����
* �����������						
* �����������
* �� �� ֵ���ɹ���0; ʧ�ܣ�-1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int sort_record_file_info()
{
	int i,j;
	int num;
	unsigned long tmp_pointer;
	unsigned long *pointer = NULL;
	RECORD_FILE_INFO_LIST tmp_list = NULL;

	num = get_record_file_info_num();
	if (num <= 1)
	{
		return -1;
	}

	// ������ṹ�任������ṹ 
	tmp_list = g_list;
	pointer = (unsigned long *)malloc(sizeof(unsigned long)*num);
	if (pointer == NULL)
	{
		return -1;
	}

	for (i=0; i<num; i++)
	{
		pointer[i] = (unsigned long)tmp_list;
		tmp_list = tmp_list->next;
	}

	// ѡ������ 
	for (i=0; i<num-1; i++)
	{
		for (j=i+1; j<num; j++) // ����С�� 
		{
			if (strncmp(((RECORD_FILE_INFO *)pointer[i])->text,((RECORD_FILE_INFO *)pointer[j])->text,50) > 0)
			{
				tmp_pointer = pointer[i];
				pointer[i] = pointer[j];
				pointer[j] = tmp_pointer;
			}
		}
	}

	for (i=0; i<num-1; i++)
	{
		((RECORD_FILE_INFO *)pointer[i])->next = (RECORD_FILE_INFO *)pointer[i+1];
	}

	((RECORD_FILE_INFO *)pointer[num-1])->next = NULL;

	g_list = (RECORD_FILE_INFO *)pointer[0];

	free(pointer);

	return 0;
}

/******************************************************************************
* �������ƣ�close_search_record_file_info
* �����������ر�¼���ļ���Ϣ
* �����������						
* �����������
* �� �� ֵ��0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int close_search_record_file_info()
{
	RECORD_FILE_INFO_LIST tmp_list = NULL;

	// �ͷŲ�ѯ¼���ļ���Ϣ�ĵ���������ڴ�
	while (g_list != NULL)
	{
		tmp_list = g_list->next;
		free(g_list);
		g_list = tmp_list;
	}
	tmp_list = g_list = NULL;

	return 0;
}

/******************************************************************************
* �������ƣ�get_record_file_info_num
* ������������ȡ����ɾ���������ļ���
* �����������						
* �����������
* �� �� ֵ������ֵ���ļ���
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_record_file_info_num()
{
	int num = 0;
	RECORD_FILE_INFO_LIST tmp_list = g_list;

	//�����ѯ¼���ļ���Ϣ�ĵ�������
	while (tmp_list != NULL)
	{
		num++;
		tmp_list = tmp_list->next;
	}

	return num;
}

/******************************************************************************
* �������ƣ�get_record_file_info_by_index
* ������������ȡ¼���ļ�����Ϣ(ͨ������)
* ���������int index ����						
* ���������char *text ��ѯ����¼���ļ���
* �� �� ֵ���ɹ���0; ʧ�ܣ�-1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_record_file_info_by_index(char *text,int index)
{
	int i;
	int num = 0;
	RECORD_FILE_INFO_LIST tmp_list = g_list;

	//��ѯ¼���ļ���Ϣ�ĵ����������ļ���Ŀ
	num = get_record_file_info_num();
	if (index < 0 || index >= num)
	{
		return -1;
	}

	// �����ѯ¼���ļ���Ϣ�ĵ�������
	for (i=0; i<index; i++)
	{
		if (tmp_list != NULL)
		{
			tmp_list = tmp_list->next;
		}
	}

	strcpy(text,tmp_list->text);

	return 0;
}

