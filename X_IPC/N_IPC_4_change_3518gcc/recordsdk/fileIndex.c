/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�indexFile.c
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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <dirent.h>

#include "fileIndex.h"

#include "fileQuery.h"


static fileIndexInfo *g_fileIndexInfo = NULL;	//�����ļ���Ϣ

/******************************************************************************
* �������ƣ�init_file_index_func
* ������������ʼ��ϵͳ�����ļ��Ļص��������ú������̻߳ص���������ϵͳ��ʼ��ʱ
*			�����ļ������߳�.
* ���������void *file_index_info	
* �����������
* �� �� ֵ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
void *init_file_index_func(void *file_index_info)
{	
	int hd_num;
	int partition_num;
	int channel_num;
	FILE *fp = NULL;
	char index_name_full[64];
	char hd_name[16];
	char partition_name[16];
	char channel_name[16];
	char data[16];
	char file_name[16];
	char full_path[128];
	DIR *dir = NULL;
	struct dirent *ptr;
	pthread_mutexattr_t attr;

	//���ݻص������Ĳ���,���ģ��Ķ�����
	if (file_index_info == NULL)
	{
		printf("(init_file_index_func): not parameter.\n");
		return;
	}
	else
	{
		g_fileIndexInfo = (fileIndexInfo *)malloc(sizeof(fileIndexInfo));
		if (g_fileIndexInfo == NULL)
		{
			printf("(init_file_index_func): memory allocate fail.\n");			
			return;
		}
		else
		{	
			memset(g_fileIndexInfo, 0, sizeof(fileIndexInfo));
			memcpy(g_fileIndexInfo, (fileIndexInfo *)file_index_info, sizeof(fileIndexInfo));
		}
	}

	//�ж�ϵͳ�Ƿ���Ӳ��
	if (g_fileIndexInfo->cur_disk == NULL)
	{
		printf("(init_file_index_func):System not hard disk.\n");
		free(g_fileIndexInfo);
		return;
	}
	
	//�����������
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_FAST_NP);	// [ZHB]
	pthread_mutex_init(&g_fileIndexInfo->index_mutex, &attr);
	pthread_mutexattr_destroy(&attr);

	//���������ļ�
	sprintf(index_name_full, "/%s/%s", g_fileIndexInfo->cur_disk, g_fileIndexInfo->index_name);
	fp = fopen(index_name_full, "w+");
	if (fp == NULL)
	{
		if (g_fileIndexInfo)
		{		
			free(g_fileIndexInfo);
		}
		printf("(init_file_index_func):Can not open the index file(%s).\n", index_name_full);
		return;
	}

	//Ӳ��
	for (hd_num=0; hd_num<8; hd_num++)
	{
		//дӲ�̺�
		sprintf(hd_name, "[HD%02d]\n", hd_num);
		fputs(hd_name, fp);

		//���Ӳ��MOUNT
		if(!g_fileIndexInfo->disk_mount_flag[hd_num])
		{
			fputs("\tNULL\n\n", fp);
			continue;
		}

		//����
		for (partition_num=0; partition_num<2; partition_num++)
		{
			//д������
			if (partition_num == 0)
			{			
				//partition_name = "\t[DATA PARTITION]\n";
				strcpy(partition_name, "\t[DATA PARTITION]\n");
			}
			else
			{
				//partition_name = "\t[BACKUP PARTITION]\n";
				strcpy(partition_name, "\t[BACKUP PARTITION]\n");
			}
			fputs(partition_name, fp);
			
			dir = opendir(full_path);
			if (dir == NULL)
			{
				fputs("\t\t[NULL]\n\n", fp);
				continue;	
			}
			
			//ͨ����
			for (channel_num=0; channel_num<16; channel_num++)
			{
				//дͨ����
				sprintf(channel_name, "\t\t[CHANNEAL%02d]", channel_num);
				fputs(channel_name, fp);
			
				dir = opendir(full_path);
				if (dir == NULL)
				{
					fputs("\t\t[NULL]\n\n", fp);
					continue;	
				}				
			}
			
			while ((ptr=readdir(dir)) != NULL)
			{
				//��Ŀ¼
				if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0 )
				{
						
				}
			}
		}
	}

	return ;
}

/******************************************************************************
* �������ƣ�write_record_index_file
* ����������д�����ļ�
* ���������char *file_name		�ļ���
*			int minute			����
*			int file_type		�ļ�����
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int write_record_index_file(char *file_name, int minute, int file_type)
{
	FILE * file = NULL;
	struct stat buf;
	RECORD_FILE_INDEX index;
	int num = 0;
	
	if (minute < 0 || minute > 59)
		return -1;
	
	memset(&index,0,sizeof(RECORD_FILE_INDEX));
	
	if (stat(file_name,&buf) != -1) /* �ļ����� */
	{
		file = fopen(file_name, "rb");
		if (file == NULL)
		{
			printf("open index file = %s failed \n",file_name);
			return -1;
		}

		num = fread(&index,1,sizeof(RECORD_FILE_INDEX),file);
		if (num < sizeof(RECORD_FILE_INDEX))
			memset(&index,0,sizeof(RECORD_FILE_INDEX));
		fclose(file);	
	}
		
	index.file_type[minute] = file_type;
	file = fopen(file_name, "wb");
	if (file == NULL)
	{
		printf("open index file = %s failed \n",file_name);
		return -1;
	}
	fseek(file,0,SEEK_SET);
	fwrite(&index,1,sizeof(RECORD_FILE_INDEX),file);
	fflush(file);
	sync();
	fclose(file);

	return 0;
}

/******************************************************************************
* �������ƣ�search_record_index_file_by_type
* ����������ͨ�������ļ�����ѯ�ļ�����
* ���������char *file_name		�ļ���
*			int file_type		�ļ�����
* �����������
* �� �� ֵ���ɹ�: 1; ʧ��: 0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int search_record_index_file_by_type(char *file_name,int file_type)
{
	FILE * file = NULL;
	struct stat buf;
	RECORD_FILE_INDEX index;
	int i,num = 0;
	
	if (stat(file_name, &buf) != 0) /*            */
	{
		printf("the file (%s) is not exist!\n");
		return 0;
	}
		
	file = fopen(file_name,"rb");
	if(file == NULL)
	{
		printf("open index file = %s failed \n",file_name);
		return 0;
	}
	
	num = fread(&index,1,sizeof(RECORD_FILE_INDEX),file);
	fclose(file);
	if (num < sizeof(RECORD_FILE_INDEX))
	{
		return 0;
	}
		
	for (i=0; i<60; i++)
	{
		if ((index.file_type[i] & file_type) != 0)
		{
			return 1;
		}
	}	
	
	return 0;
}

/******************************************************************************
* �������ƣ�read_record_index_file
* �����������������ļ�
* ���������char *file_name				�ļ���		
* ���������RECORD_FILE_INDEX *index	�ļ�������Ϣ
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int read_record_index_file(char *file_name,RECORD_FILE_INDEX *index)
{
	FILE * file = NULL;
	int num = 0;
		
	file = fopen(file_name,"rb");
	if(file == NULL)
	{
		return -1;
	}
	
	num = fread(index,1,sizeof(RECORD_FILE_INDEX),file);
	fclose(file);
	if(num < sizeof(RECORD_FILE_INDEX))
	{
		return -1;
	}
	
	return 0;
}



















