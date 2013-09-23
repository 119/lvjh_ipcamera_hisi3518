/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�fileBak.c
* �ļ�˵�������ļ�������¼���ļ����ݲ���
* �����б�
*    ����1��
*        �������ƣ�start_backup_proc
*        �������ܣ�¼���ļ��ı��ֲ���
*    ����2��
*        �������ƣ�get_total_file_num
*        �������ܣ���ô������ļ�������
*    ����3��
*        �������ƣ�get_cur_file_no
*        �������ܣ���õ�ǰ���ڱ��ݵ��ļ���
*    ����4��
*        �������ƣ�end_backup_proc
*        �������ܣ��ļ����ݽ���
*    ����5��
*        �������ƣ�get_iso_back_dir
*        �������ܣ���ȡ�����ļ�����
*    ����6��
*        �������ƣ�my_cp
*        �������ܣ����Ƶ����ļ�
*    ����7��
*        �������ƣ�get_iso_back_dir_full_flag
*        �������ܣ���ȡ�����ļ���ȫ·����־
*    ����8��
*        �������ƣ�create_iso_dir
*        �������ܣ����������ļ���
*    ����9��
*        �������ƣ�delete_iso_dir
*        �������ܣ�ɾ�������ļ���
*    ����10��
*        �������ƣ�get_iso_back_dir
*        �������ܣ���ȡ�����ļ�����
*    ����11��
*        �������ƣ�file_backup_fun
*        �������ܣ��ļ������̴߳���
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
#include <dirent.h>
#include <sys/stat.h>

#include "global.h"
#include "fileBak.h"
#include "util.h"

#define DATA_BLK_SIZE	(1024*8)					//ÿ�ζ�ȡ�ļ����ֽ���

static int g_total_file_num = 0;
static int g_cur_file_no = 0;
static int g_end_cp_flag = 0;
static FILE_BACKUP_INFO g_file_backup_info = {0};
static pthread_t g_p_thread;						//����¼���ļ����߳�
static char g_iso_back_dir[256] = "";
static int  g_iso_backed_file_size = 0;
static int  g_iso_back_dir_full_flag = 0; 

/******************************************************************************
* �������ƣ�my_cp
* �������������Ƶ����ļ�
* ���������char *src_file_name Դ�ļ���
*           char *dst_file_name Ŀ���ļ���
* �����������
* �� �� ֵ��ʧ�ܣ�-1���ɹ���0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
static int my_cp(char *src_file_name, char *dst_file_name)
{
	int ret = -1;
	char buf[DATA_BLK_SIZE];
	FILE *src_fd = NULL;
	FILE *dst_fd = NULL;

	src_fd = fopen(src_file_name, "r+b"); //ֻ����Դ�ļ�
	if (src_fd < 0)
	{
		printf("open src file: %s error!\n", src_file_name);
		return -1;
	}

	dst_fd = fopen(dst_file_name, "w+b"); //��Ŀ���ļ�
	if(dst_fd < 0)
	{
		printf("open dst file: %s error!\n", dst_file_name);
		fclose(src_fd);
		return -1;
	}

	while (!feof(src_fd))
	{
		ret = fread(buf, 1, DATA_BLK_SIZE, src_fd); //��Դ�ļ�
		if (ret <= 0)
		{
			break;
		}

		ret = fwrite(buf, 1, ret, dst_fd); //дĿ���ļ�
		if (ret <= 0)
		{
			break;
		}
		//fflush(dst_fd);
	}

	fclose(src_fd);
	fclose(dst_fd);

	return 0;
}

/******************************************************************************
* �������ƣ�get_iso_back_dir_full_flag
* ������������ȡ�����ļ���ȫ·����־
* �����������
* �����������
* �� �� ֵ������ֵ�����ػ�ȡ�����ļ���ȫ·����־
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_iso_back_dir_full_flag()
{
	return g_iso_back_dir_full_flag;
}

/******************************************************************************
* �������ƣ�create_iso_dir
* �������������������ļ���
* �����������
* �����������
* �� �� ֵ���ɹ���0��ʧ�ܣ�-1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int create_iso_dir()
{
	int i = 0;
	int size = 0;
	int disk_num = get_hard_disk_num();	//��ȡ�洢Ӳ���ĸ���

	for (i=0; i<disk_num; i++)
	{
		size = hd_get_partition_available_size(i, 1); // ��ȡ���ݷ������ÿռ�Ĵ�С

		if (size >= 600*2) //���ÿռ�>1200
		{
			sprintf(g_iso_back_dir, "/record/hd%02d/01/iso_back/",i);
			return 0;
		}
	}

	return -1;
}

/******************************************************************************
* �������ƣ�delete_iso_dir
* ����������ɾ�������ļ���
* �����������
* �����������
* �� �� ֵ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int delete_iso_dir()
{
	if (g_iso_back_dir != "")
	{
		delete_dir((char *)(get_iso_back_dir())); //ɾ�������ļ��м����µ������ļ�
		strcpy(g_iso_back_dir, "");
		g_iso_backed_file_size = 0;
		g_iso_back_dir_full_flag = 0;

		return 0;
	}

	return -1;
}

/******************************************************************************
* �������ƣ�get_iso_back_dir
* ������������ȡ�����ļ�����
* �����������
* �����������
* �� �� ֵ���ַ����������ļ�����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
char *get_iso_back_dir()
{
	return g_iso_back_dir;
}

/******************************************************************************
* �������ƣ�file_backup_fun
* �����������ļ������̴߳���
* ���������void *param ʵ���ϴ���FILE_BACKUP_INFO���ݽṹ��ָ��
* �����������
* �� �� ֵ��void *
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
static void * file_backup_fun(void *param)
{
	int i,j;
	int channel,year,month,day;
	int start_hour,start_minute;
	int end_hour,end_minute,dst_disk_no;
	int disk_type;
	char src_path[MAX_PATH];
	char dst_path[MAX_PATH];
	char src_start_file_name[MAX_PATH];
	char src_end_file_name[MAX_PATH];
	char src_file_name[MAX_PATH];
	char src_full_file_name[MAX_PATH];
	char dst_file_name[MAX_PATH];
	char dst_index_name[MAX_PATH];
	char str[MAX_PATH];
	int  index_minute = 0;
	int  disk_num;

	struct dirent *ptr;
	DIR *dir = NULL;
	struct stat st;

#ifdef CD_R
	DIR *dst_dir = NULL;
#endif
	

	FILE_BACKUP_INFO *file_backup_info = (FILE_BACKUP_INFO *)param;

	channel = file_backup_info->channel;
	year    = file_backup_info->year;
	month   = file_backup_info->month;
	day     = file_backup_info->day;

	start_hour   = file_backup_info->start_hour;
	start_minute = file_backup_info->start_minute;
	end_hour     = file_backup_info->end_hour;
	end_minute   = file_backup_info->end_minute;

	dst_disk_no  = file_backup_info->dst_disk_no;
	disk_type    = file_backup_info->disk_type;

	// ��������Ӳ�̵��������ݷ��������Դ���� 
	disk_num = get_hard_disk_num();
	
	for (i=0; i<disk_num; i++)
	{
		if (!hd_get_mount_flag(i, 0)) //�жϴ洢��¼�����ݷ����Ƿ�ҿ���
		{
			continue;
		}

		sprintf(src_path,"/record/hd%02d/00/ch%02d/%04d-%02d-%02d/",i,channel,year,month,day);
		sprintf(src_start_file_name,"/record/hd%02d/00/ch%02d/%04d-%02d-%02d/%02d/%02d-%02d-%02d.tm4",
					i,channel,year,month,day,start_hour,start_hour,start_minute,0);
		sprintf(src_end_file_name,"/record/hd%02d/00/ch%02d/%04d-%02d-%02d/%02d/%02d-%02d-%02d.tm4",
					i,channel,year,month,day,end_hour,end_hour,end_minute,59);

		for (j=start_hour; j<=end_hour; j++) // ��Сʱʱ��� 
		{
			sprintf(src_file_name, "%s%02d/", src_path,j);

			dir = opendir(src_file_name); //��Сʱʱ����ļ���
			if (dir == NULL)
			{
				printf("opendir failed ! dir = %s\n",src_file_name);
				continue;
			}

			while ((ptr = readdir(dir)) != NULL) //��ȡ�ļ�Ŀ¼
			{
				if (strcmp(ptr->d_name, ".") == 0 
					|| strcmp(ptr->d_name, "..") == 0 
					|| strcmp(ptr->d_name, "index.tds") == 0)
				{
					continue;
				}

				sprintf(src_full_file_name, "%s%s", src_file_name, ptr->d_name);

				if (strcmp(src_full_file_name, src_start_file_name) < 0 
					|| strcmp(src_full_file_name, src_end_file_name) > 0)
				{
					continue;
				}

				// ����Ŀ��Ŀ¼ 
				if (disk_type == 0)  //���ݵ�Ӳ��
				{
					// ����Ŀ��Ŀ¼ 
					sprintf(dst_path, "/record/hd%02d/01/ch%02d/", dst_disk_no, channel);
					mkdir(dst_path, 0x777);
					sprintf(dst_path, "%s%04d-%02d-%02d/", dst_path, year, month, day);
					mkdir(dst_path, 0x777);
					sprintf(dst_path, "%s%02d/", dst_path, j);
					mkdir(dst_path, 0x777);
					sprintf(dst_file_name, "%s%s", dst_path,ptr->d_name);
					sprintf(dst_index_name, "%s%s", dst_path, "index.tds");
				}
				else if (disk_type == 1)	//���ݵ�U��
				{
					// ����Ŀ��Ŀ¼ 
					sprintf(dst_path, "/usb%01d/ch%02d/", dst_disk_no, channel);
					mkdir(dst_path,0x777);
					sprintf(dst_path, "%s%04d-%02d-%02d/", dst_path, year, month, day);
					mkdir(dst_path, 0x777);
					sprintf(dst_path, "%s%02d/", dst_path, j);
					mkdir(dst_path, 0x777);

					sprintf(dst_file_name, "%s%s", dst_path,ptr->d_name);
					sprintf(dst_index_name, "%s%s", dst_path, "index.tds");
				}
				else if(disk_type == 2) // ���ݵ��������µ��ض�Ŀ¼;Ϊ��¼
				{
					// ����Ŀ��Ŀ¼ 
#ifdef CD_R					
					sprintf(dst_path, "%s", get_iso_back_dir());
					dst_dir = opendir(dst_path);
					if (dst_dir == NULL)
					{
						mkdir(dst_path, 0x777);
					}
					else
					{
						closedir(dst_dir);
					}

					mkdir(dst_path,0x777);
					sprintf(dst_path,"%sch%02d/", dst_path, channel);
					dst_dir = opendir(dst_path);
					if (dst_dir == NULL)
					{
						mkdir(dst_path,0x777);
					}
					else
					{
						closedir(dst_dir);
					}

					mkdir(dst_path,0x777);
					sprintf(dst_path,"%s%04d-%02d-%02d/", dst_path,year,month,day);
					dst_dir = opendir(dst_path);
					if (dst_dir == NULL)
					{
						mkdir(dst_path,0x777);
					}
					else
					{
						closedir(dst_dir);
					}

					mkdir(dst_path, 0x777);
					sprintf(dst_path, "%s%02d/", dst_path, j);
					dst_dir = opendir(dst_path);
					if (dst_dir == NULL)
					{
						mkdir(dst_path,0x777);
					}
					else
					{
						closedir(dst_dir);
					}

					mkdir(dst_path, 0x777);
					sprintf(dst_file_name, "%s%s", dst_path, ptr->d_name);
					sprintf(dst_index_name, "%s%s", dst_path, "index.tds");
#endif					
				}

				// ���Ƶ����ļ� 
				memcpy(str, ptr->d_name, sizeof(ptr->d_name) );
       			memmove(str, &str[3], 2);
				str[2] = '\0';
				index_minute = atoi(str);
				
				if (strcmp(dst_index_name, src_full_file_name) != 0)	
				{
					my_cp(src_full_file_name, dst_file_name);
					if ( index_minute >=0 && index_minute <= 59 )
					{
						//  д�����ļ�
						write_record_index_file(dst_index_name, index_minute, 0xff);
					}
				}

				g_cur_file_no++;

				// ���ݵ��������µ��ض�Ŀ¼;Ϊ��¼
				if (disk_type == 2)
				{
					stat(dst_file_name, &st);

					g_iso_backed_file_size += st.st_size ;
					if (g_iso_backed_file_size >= 600*1024*1024)
					{
						g_iso_back_dir_full_flag = 1;
						return NULL;
					}
				}
				
				if (g_end_cp_flag) 
				{
					break;
				}

				usleep(1000*200); // 100 ms
			}

			closedir(dir);

			if (g_end_cp_flag)
			{
				break;
			}
		}

		if (g_end_cp_flag) //�ļ����������������߳�
		{
			break;
		}
	}

	return NULL;
}

/******************************************************************************
* �������ƣ�start_backup_proc
* ������������ʼ�������ݴ���
* ���������int channel			Ҫ����¼���ļ���ͨ��
*			int year			Ҫ����¼���ļ������
*			int month			Ҫ����¼���ļ����·�
*			int day				Ҫ����¼���ļ�������
*			int start_hour		Ҫ����¼���ļ��Ŀ�ʼʱ�䣺Сʱ
*			int start_minute	Ҫ����¼���ļ��Ŀ�ʼʱ�䣺����
*			int end_hour		Ҫ����¼���ļ��Ľ���ʱ�䣺Сʱ
*			int end_minute		Ҫ����¼���ļ��Ľ���ʱ�䣺����
*			int disk_type		Ҫ����¼���ļ��Ĵ洢�̵�����
*			int dst_disk_no		Ҫ����¼���ļ��Ĵ洢�̺�
* �����������
* �� �� ֵ��ʧ��: -1; �ɹ���0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int start_backup_proc(int channel,int year,int month,int day,int start_hour,int start_minute,
							int end_hour,int end_minute,int disk_type, int dst_disk_no)
{
	int ret = -1;
	struct sched_param param;

	g_end_cp_flag = 0;
	g_cur_file_no = 0;
	g_file_backup_info.channel = channel;
	g_file_backup_info.year    = year;
	g_file_backup_info.month   = month;
	g_file_backup_info.day     = day;
	g_file_backup_info.start_hour  = start_hour;
	g_file_backup_info.start_minute= start_minute;
	g_file_backup_info.end_hour    = end_hour;
	g_file_backup_info.end_minute  = end_minute;
	g_file_backup_info.disk_type   = disk_type;
	g_file_backup_info.dst_disk_no = dst_disk_no;
	
	//��������¼�����ݵ��߳�
	ret = pthread_create(&g_p_thread,NULL,file_backup_fun,&g_file_backup_info);
	
	//���ñ���¼�����ݵ��̵߳����ȼ�
	param.sched_priority = 10;
	pthread_setschedparam(g_p_thread, SCHED_OTHER, &param);
	
	return ret;
}

/******************************************************************************
* �������ƣ�get_total_file_num
* ������������ô������ļ�������(Ϊ����ʾ���ݽ���ʱ��)
* �����������
* �����������
* �� �� ֵ������ֵ���������ļ�������
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_total_file_num()
{
	int i,j;
	int disk_num;
	int channel,year,month,day;
	int start_hour,start_minute;
	int end_hour,end_minute,dst_disk_no;
	char src_path[MAX_PATH];
	char src_start_file_name[MAX_PATH];
	char src_end_file_name[MAX_PATH];
	char src_file_name[MAX_PATH];
	char src_full_file_name[MAX_PATH];
	struct dirent *ptr;
	DIR *dir = NULL;

	channel = g_file_backup_info.channel;
	year = g_file_backup_info.year;
	month = g_file_backup_info.month;
	day = g_file_backup_info.day;
	start_hour = g_file_backup_info.start_hour;
	start_minute = g_file_backup_info.start_minute;
	end_hour = g_file_backup_info.end_hour;
	end_minute = g_file_backup_info.end_minute;
	dst_disk_no = g_file_backup_info.dst_disk_no;

	// ��������Ӳ�̵��������ݷ��������Դ���� 
	g_total_file_num = 0;
	
	disk_num = get_hard_disk_num(); //��ȡӲ�̸���
	
	for (i=0; i<disk_num; i++)
	{
		//�ж�Ӳ��¼���������Ƿ�ҿ���
		if (!hd_get_mount_flag(i, 0))
		{
			continue;
		}

		sprintf(src_path,"/record/hd%02d/00/ch%02d/%04d-%02d-%02d/",i,channel,year,month,day);
		sprintf(src_start_file_name,"/record/hd%02d/00/ch%02d/%04d-%02d-%02d/%02d/%02d-%02d-%02d.tm4",
					i,channel,year,month,day,start_hour,start_hour,start_minute,0);
		sprintf(src_end_file_name,"/record/hd%02d/00/ch%02d/%04d-%02d-%02d/%02d/%02d-%02d-%02d.tm4",
					i,channel,year,month,day,end_hour,end_hour,end_minute,59);

		for (j=start_hour; j<=end_hour; j++) // ��Сʱʱ��β�ѯ 
		{
			sprintf(src_file_name, "%s%02d/", src_path, j);

			dir = opendir(src_file_name);
			if (dir == NULL)
			{
				continue;
			}

			//����Сʱ�������ļ��ĸ���
			while ((ptr = readdir(dir)) != NULL)
			{
				if (strcmp(ptr->d_name,".") == 0 
					|| strcmp(ptr->d_name,"..") == 0
					|| strcmp(ptr->d_name,"index.tds") == 0)
				{
					continue;
				}

				sprintf(src_full_file_name,"%s%s", src_file_name, ptr->d_name);

				if (strcmp(src_full_file_name,src_start_file_name) < 0
					|| strcmp(src_full_file_name,src_end_file_name) > 0)
				{
					continue;
				}

				g_total_file_num++;
			}
			
			closedir(dir);
		}
	}

	return g_total_file_num;
}

/******************************************************************************
* �������ƣ�get_cur_file_no
* ������������õ�ǰ���ڱ��ݵ��ļ���(Ϊ����ʾ���ݽ���ʱ��)
* �����������
* �����������
* �� �� ֵ������ֵ����ǰ���ڱ��ݵ��ļ���
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_cur_file_no()
{
	return g_cur_file_no;
}

/******************************************************************************
* �������ƣ�end_backup_proc
* �����������ļ����ݽ���
* �����������
* �����������
* �� �� ֵ��0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int end_backup_proc()
{
	g_end_cp_flag = 1;
	
	return 0;
}

 
