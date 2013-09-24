/* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�hardDisk.c
* �ļ�˵�������ļ������˲���Ӳ�̵ĺ����Ķ���
* �����б�
*    ����1��
*        �������ƣ�get_hard_disk_info
*        �������ܣ���ȡӲ����Ϣ
*    ����2��
*        �������ƣ�get_partition_name
*        �������ܣ���÷�����
*    ����3��
*        �������ƣ�hd_mount_partition
*        �������ܣ�MOUNT����
*    ����4��
*        �������ƣ�hd_umount_partition
*        �������ܣ�UMOUNT����
*    ����5��
*        �������ƣ�hd_mount_disk
*        �������ܣ�MOUNTӲ��
*    ����6��
*        �������ƣ�hd_umount_disk
*        �������ܣ�UMOUNTӲ��
*    ����7��
*        �������ƣ�set_cur_disk_no
*        �������ܣ����õ�ǰ��¼¼���Ӳ�̺�
*    ����8��
*        �������ƣ�set_cur_partition_no
*        �������ܣ����÷�����
*    ����9��
*        �������ƣ�get_cur_disk_no
*        �������ܣ���ȡ��ǰ���ڼ�¼¼���Ӳ�̺�
*    ����10��
*        �������ƣ�get_cur_partition_no
*        �������ܣ���ȡ��ǰ���ڼ�¼¼��ķ�����
*    ����11��
*        �������ƣ�get_hard_disk_num
*        �������ܣ���ȡӲ����
*    ����12��
*        �������ƣ�hd_get_mount_flag
*        �������ܣ���ȡӲ��MOUNT��־
*    ����13��
*        �������ƣ�hd_get_disk_formated
*        �������ܣ���ȡӲ�̸�ʽ����־
*    ����14��
*        �������ƣ�hd_mount_all_partition
*        �������ܣ�MOUNT����Ӳ�̵����з���
*    ����15��
*        �������ƣ�hd_umount_all_partition
*        �������ܣ�UMOUNT����Ӳ�̵����з���
*    ����16��
*        �������ƣ�hd_fdisk
*        �������ܣ�����Ӳ�̷���
*    ����17��
*        �������ƣ�hd_format
*        �������ܣ���ʽ����һ����
*    ����18��
*        �������ƣ�hd_cur_partition_full
*        �������ܣ����Ӳ���Ƿ���
*    ����19��
*        �������ƣ�hd_get_disk_info_for_frontpanel
*        �������ܣ����Ӳ���������Ƿ��� <ǰ�����>
*    ����20��
*        �������ƣ�hd_find_empty_partition
*        �������ܣ�����δ������(���������ݷ���)
*    ����21��
*        �������ƣ�hd_query_disk_info
*        �������ܣ���ʱ��ѯӲ����Ϣ
*    ����22��
*        �������ƣ�int hd_get_disk_total_size
*        �������ܣ���ȡ����Ӳ��������
*    ����23��
*        �������ƣ�hd_get_disk_used_size
*        �������ܣ���ȡ����Ӳ�̵����ÿռ�
*    ����24��
*        �������ƣ�hd_get_disk_available_size
*        �������ܣ�����ֵ������Ӳ�̵Ŀ��ÿռ�
*    ����25��
*        �������ƣ�hd_get_partition_total_size
*        �������ܣ���ȡ����������
*    ����26��
*        �������ƣ�hd_get_partition_used_size
*        �������ܣ���ȡ�������ÿռ�
*    ����27��
*        �������ƣ�hd_get_partition_available_size
*        �������ܣ���ȡ�������ÿռ�
*    ����28��
*        �������ƣ�hd_get_backup_partition_num
*        �������ܣ���ȡ���ݷ�����(��mount�ɹ��ı�����)
*    ����29��
*        �������ƣ�hd_get_backup_disk_no
*        �������ܣ�ͨ�����ݷ���������ñ���Ӳ�̺�
*    ����30��
*        �������ƣ�CDRecord_GetDeviceName
*        �������ܣ���ȡCD�豸����
*    ����31��
*        �������ƣ�Check_CDRecorder
*        �������ܣ����CDRecorder�Ƿ����
*    ����32��
*        �������ƣ�usb_GetUSBName
*        �������ܣ���ȡUSB�洢�豸����
*    ����33��
*        �������ƣ�check_usb_disk_num
*        �������ܣ����USB���Ƿ����
*    ����34��
*        �������ƣ�usb_mount_partition
*        �������ܣ�MOUNT USB�̵ķ���
*    ����35��
*        �������ƣ�usb_umount_partition
*        �������ܣ�UMOUNT USB�̵ķ���
*    ����36��
*        �������ƣ�format_fun
*        �������ܣ�FORMAT�̴߳�����
*    ����37��
*        �������ƣ�fdisk_fun
*        �������ܣ�FDISK�̴߳�����
*    ����38��
*        �������ƣ�get_fdisk_format_process
*        �������ܣ���ȡ������ʽ�������Ľ���
*    ����39��
*        �������ƣ�get_fdisk_format_status
*        �������ܣ���ȡ������ʽ�������Ľ��
*    ����40��
*        �������ƣ�get_disk_avail_size
*        �������ܣ������mountĿ¼��ʣ��ռ�
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
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/vfs.h>

#include "hardDisk.h"
#include "fdisk.h"

#ifndef MAX_PATH
#define MAX_PATH	256
#endif

static HARD_DISK_INFO g_hard_disk_info = {0};
static int fdisk_format_process = 0;			//Ӳ�̸�ʽ������
static int fdisk_format_status  = 0;			//Ӳ�̸�ʽ��״̬
static int g_file_delete_status = 0;			//�ļ�ɾ��״̬

/******************************************************************************
* �������ƣ�get_hard_disk_info
* ������������ȡӲ����Ϣ
* �����������
* ���������HARD_DISK_INFO *hard_disk_info Ӳ����Ϣ���ݽṹ
* �� �� ֵ��0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_hard_disk_info(HARD_DISK_INFO *hard_disk_info)
{
	if (hard_disk_info == NULL)
	{
		return -1;
	}
	
	memcpy( hard_disk_info, &g_hard_disk_info, sizeof(HARD_DISK_INFO) );
	
	return 0;
}

/******************************************************************************
* �������ƣ�get_partition_name
* ������������÷�����,��:/dev/hde1, /dev/hde2
* ���������int disk_no			Ӳ�̺�
*			int partition_no	������
* ���������char *partition_name ������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
static int get_partition_name(char *partition_name, int disk_no, int partition_no)
{
	if (partition_name == NULL)
	{
		return -1;
	}
	if (disk_no<0 || disk_no >7 || partition_no<0 || partition_no>2)
	{
		return -1;
	}

	sprintf(partition_name,"%s%d", tm3k_HD_GetDiskName(disk_no), partition_no + 1);

	return 0;
}

/******************************************************************************
* �������ƣ�hd_mount_partition
* ����������MOUNT����
* ���������char *source	Ӳ�̷����豸����
*			char *target	Ӳ�̷�������MOUNTĿ������
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_mount_partition(char *source, char *target)
{
	int ret = -1;
	int i = 0;

	if (source==NULL || target==NULL)
	{
		return -1;
	}	

	do
	{
		ret = mount(source, target, "vfat", 0, NULL);
		if (ret != 0)
		{
			printf("mount %s to %s failed = %d\n", source, target, i);
			sleep(1);
		}
		else
		{
			return 0;
		}
		
		i++;
	}while(i < 10 ); //��ೢ��10��

	return -1;
}

/******************************************************************************
* �������ƣ�hd_umount_partition
* ����������UMOUNT����
* ���������char *target	Ӳ�̷�������UMOUNTĿ������
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_umount_partition(char *target)
{
	int ret = -1;
	int i = 0;

	if (target == NULL)
	{
		return -1;
	}	
	
	do
	{
		ret = umount(target);
		if(ret != 0)
		{
			printf("umount %s failed = %d \n", target, i);
			sleep(1);
		}
		else
		{
			return 0;
		}

		i++;
	}while(i < 10 );	//��ೢ��10��

	return -1;
}

/******************************************************************************
* �������ƣ�hd_mount_disk
* ����������MOUNTӲ��
* ���������int disk_no	Ӳ�̺�
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_mount_disk(int disk_no)
{
	int i,ret;
	int partition_num;
	char partition_name[MAX_PATH];
	char dir_name[MAX_PATH];
	int  type;				//   1: ���ݷ���   2: ���ݷ���  3: ����  
	DISKSIZE disk_size;

	//pause_record_file(); 		// ��ͣ����¼�� 

	if ( tm3k_HD_IsDiskFormated(tm3k_HD_GetDiskName(disk_no)) == 0 )
	{		
		partition_num = tm3k_HD_GetDiskPartionNum(tm3k_HD_GetDiskName(disk_no));
		
		for (i=0; i<partition_num; i++)
		{
			g_hard_disk_info.disk_info[disk_no].partition_info[i].format_flag = 1;	//�Ѿ���ʽ��

			type = tm3k_HD_GetPartionVol( tm3k_HD_GetDiskName(disk_no), i + 1 );
			
			g_hard_disk_info.disk_info[disk_no].partition_info[i].type = type;

			//printf("hd_mount_disk==type = %d\n", type);

			get_partition_name(partition_name, disk_no, i);
			
			sprintf(dir_name,"/record/hd%02d/%02d",disk_no,i);
			
			ret = hd_mount_partition(partition_name,dir_name);
			if (ret == 0)
			{
				g_hard_disk_info.disk_info[disk_no].partition_info[i].mount_flag = 1;
				tm3k_HD_GetDiskInfo(partition_name, &disk_size);
				g_hard_disk_info.disk_info[disk_no].partition_info[i].availabe_size = disk_size.availablesize;
			}
			else
			{
				// ���¹���
				hd_umount_partition(dir_name);
				ret = hd_mount_partition(partition_name,dir_name);
				if (ret == 0)
				{
					g_hard_disk_info.disk_info[disk_no].partition_info[i].mount_flag = 1;
					tm3k_HD_GetDiskInfo(partition_name,&disk_size);
					g_hard_disk_info.disk_info[disk_no].partition_info[i].availabe_size = disk_size.availablesize;
				}
			}
		}
	}
	else
	{
		g_hard_disk_info.disk_info[disk_no].partition_info[0].format_flag = 0;
		g_hard_disk_info.disk_info[disk_no].partition_info[1].format_flag = 0;
		g_hard_disk_info.disk_info[disk_no].partition_info[0].mount_flag = 0;
		g_hard_disk_info.disk_info[disk_no].partition_info[1].mount_flag = 0;
	}

	//restart_record_file(); // ���¿�ʼ����¼�� 

	return 0;
}

/******************************************************************************
* �������ƣ�hd_umount_disk
* ����������UMOUNTӲ��
* ���������int disk_no	Ӳ�̺�
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_umount_disk(int disk_no)
{
	int i,ret;
	int partition_num;
	char dir_name[MAX_PATH];

	//pause_record_file(); // ��ͣ����¼�� 

	partition_num = tm3k_HD_GetDiskPartionNum(tm3k_HD_GetDiskName(disk_no));
	
	for (i=0; i<partition_num; i++)
	{
		sprintf(dir_name, "/record/hd%02d/%02d", disk_no, i);
		if (g_hard_disk_info.disk_info[disk_no].partition_info[i].mount_flag == 1)
		{
			ret = hd_umount_partition(dir_name);
			if (ret == 0)
				g_hard_disk_info.disk_info[disk_no].partition_info[i].mount_flag = 0;
			else
				return -1;
		}
	}

	return 0;
}

/******************************************************************************
* �������ƣ�set_cur_disk_no
* �������������õ�ǰ��¼¼���Ӳ�̺�
* ���������int disk_no	Ӳ�̺�
* �����������
* �� �� ֵ��0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int set_cur_disk_no(int disk_no)
{
	g_hard_disk_info.cur_disk_no = disk_no;
	
	return 0;
}

/******************************************************************************
* �������ƣ�set_cur_partition_no
* �������������÷�����
* ���������int disk_no	Ӳ�̺�
* �����������
* �� �� ֵ��0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int set_cur_partition_no(int partition_no)
{
	g_hard_disk_info.cur_partition_no = partition_no;
	
	return 0;
}

/******************************************************************************
* �������ƣ�get_cur_disk_no
* ������������ȡ��ǰ���ڼ�¼¼���Ӳ�̺�
* �����������
* �����������
* �� �� ֵ������ֵ����ǰ���ڼ�¼¼���Ӳ�̺�
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_cur_disk_no()
{
	return g_hard_disk_info.cur_disk_no;
}

/******************************************************************************
* �������ƣ�get_cur_partition_no
* ������������ȡ��ǰ���ڼ�¼¼��ķ�����
* �����������
* �����������
* �� �� ֵ������ֵ����ǰ���ڼ�¼¼��ķ�����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_cur_partition_no()
{
	return g_hard_disk_info.cur_partition_no;
}

/******************************************************************************
* �������ƣ�get_hard_disk_num
* ������������ȡӲ����
* �����������
* �����������
* �� �� ֵ������ֵ��Ӳ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_hard_disk_num()
{
	return g_hard_disk_info.hard_disk_num;
}

int get_hard_disk_num_ext()
{
	g_hard_disk_info.hard_disk_num = tm3k_HD_GetDiskNum();

	return g_hard_disk_info.hard_disk_num;
}

/******************************************************************************
* �������ƣ�hd_get_mount_flag
* ������������ȡӲ��MOUNT��־
* ���������int disk_no			Ӳ�̺�
*			int partition_no	������
* �����������
* �� �� ֵ������ֵ��MOUNT��־
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_get_mount_flag(int disk_no,int partition_no)
{
	return g_hard_disk_info.disk_info[disk_no].partition_info[partition_no].mount_flag;
}

/******************************************************************************
* �������ƣ�hd_get_disk_formated
* ������������ȡӲ�̸�ʽ����־
* ���������int disk_no			Ӳ�̺�
* �����������
* �� �� ֵ������ֵ����ʽ����־
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_get_disk_formated(int disk_no, int partition_no)
{
	return g_hard_disk_info.disk_info[disk_no].partition_info[partition_no].format_flag;
}

/******************************************************************************
* �������ƣ�hd_mount_all_partition
* ����������MOUNT����Ӳ�̵����з���
* �����������
* �����������
* �� �� ֵ��0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_mount_all_partition()
{
	int i;
	int disk_num;

	disk_num = tm3k_HD_GetDiskNum();
	
	if (disk_num == 0)
	{
		g_hard_disk_info.hard_disk_num = disk_num;
		return -1;
	}
	
	for(i=0; i<disk_num; i++)
	{
		hd_mount_disk(i); // mount ��i��Ӳ�� 
	}
	
	g_hard_disk_info.hard_disk_num = disk_num;

	return 0;
}

/******************************************************************************
* �������ƣ�hd_umount_all_partition
* ����������UMOUNT����Ӳ�̵����з���
* �����������
* �����������
* �� �� ֵ��0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_umount_all_partition()
{
	int i;
	int disk_num;

	disk_num = tm3k_HD_GetDiskNum();
	
	if (disk_num == 0)
	{
		g_hard_disk_info.hard_disk_num = disk_num;
		return -1;
	}
	
	for(i=0; i<disk_num; i++)
	{
		hd_umount_disk(i); // umount ��i��Ӳ�� 
	}

	return 0;
}

/******************************************************************************
* �������ƣ�hd_fdisk
* ��������������Ӳ�̷���
* ���������int disk_no				Ӳ�̺�
*			int data_partition		���ݷ���������
*			int backup_partition	���ݷ���������
* �����������
* �� �� ֵ���ɹ�:0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_fdisk(int disk_no,int data_partition,int backup_partition)
{
	int ret;
	int disk_num;
	int cur_disk_no;

	cur_disk_no = g_hard_disk_info.cur_disk_no;

	disk_num = get_hard_disk_num();
	if ( disk_no > (disk_num - 1) )
	{
		return -4;
	}

	if ( disk_no == cur_disk_no )	// ����ǵ�ǰ��
	{
		//��ͣ¼��
		//pause_record_file();

		// umount��Ӳ�̵����з��� 
		ret = hd_umount_disk(disk_no);
		if (ret < 0)
		{
			//�ָ�¼��
			//restart_record_file();
			
			return -1;
		}

		// ������ʽ����Ӳ�� 
		ret = tm3k_HD_FDisk(tm3k_HD_GetDiskName(disk_no),data_partition,backup_partition);
		if (ret != 0)
			printf("fdisk disk: %s failed \n",tm3k_HD_GetDiskName(disk_no));
		else
			printf("fdisk disk: %s success \n",tm3k_HD_GetDiskName(disk_no));

		// mount��Ӳ�̵����з��� 
		hd_mount_disk(disk_no);

		//�ָ�ϵͳ
		//restart_record_file();
	}
	else		// ������ǵ�ǰ��
	{
		// umount��Ӳ�̵����з��� 
		ret = hd_umount_disk(disk_no);
		if(ret < 0)
		{
			return -1;
		}
		
		// ������ʽ����Ӳ�� 
		ret = tm3k_HD_FDisk(tm3k_HD_GetDiskName(disk_no),data_partition,backup_partition);
		if (ret != 0)
			printf("fdisk disk: %s failed \n",tm3k_HD_GetDiskName(disk_no));
		else
			printf("fdisk disk: %s success \n",tm3k_HD_GetDiskName(disk_no));

		// mount��Ӳ�̵����з��� 
		hd_mount_disk(disk_no);
	}
	return ret;
}

/******************************************************************************
* �������ƣ�hd_format
* ������������ʽ����һ����
* ���������int disk_no				Ӳ�̺�
*			int partition_no		������
* �����������
* �� �� ֵ���ɹ�:0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_format(int disk_no,int partition_no)
{
	int ret;
	int partition_num;
	int disk_num;

	int cur_disk_no;
	int cur_partition_no;
	char dir_name[MAX_PATH];
	char dev_name[MAX_PATH];

	cur_disk_no = g_hard_disk_info.cur_disk_no;
	cur_partition_no = g_hard_disk_info.cur_partition_no;

	partition_num = tm3k_HD_GetDiskPartionNum(tm3k_HD_GetDiskName(disk_no));
	if ( partition_no > (partition_num -1) )
	{
		return -3;
	}

	disk_num = get_hard_disk_num();
	if ( disk_no > (disk_num - 1) )
	{
		return -4;
	}

	if ( (disk_no == cur_disk_no) && (partition_no == cur_partition_no) )
	{
		//��ͣ¼��
		//pause_record_file();
		
		sprintf(dir_name,"/record/hd%02d/%02d",disk_no, partition_no);
		if (g_hard_disk_info.disk_info[disk_no].partition_info[partition_no].mount_flag == 1)
		{
			ret = hd_umount_partition(dir_name);		// umount�÷��� 
			if (ret != 0)
			{
				//�ָ�¼��
				//restart_record_file();
				return -1;
			}
		}

		ret = tm3k_HD_Format(tm3k_HD_GetDiskName(disk_no), partition_no + 1);

		get_partition_name(dev_name, disk_no, partition_no);

		hd_mount_partition(dev_name,dir_name);

		//�ָ�¼��
		//restart_record_file();
	}
	else
	{
		sprintf(dir_name,"/record/hd%02d/%02d",disk_no, partition_no);
		if (g_hard_disk_info.disk_info[disk_no].partition_info[partition_no].mount_flag == 1)
		{
			ret = hd_umount_partition(dir_name);		// umount�÷��� 
			if (ret != 0)
			{
				return -1;
			}
		}

		ret = tm3k_HD_Format(tm3k_HD_GetDiskName(disk_no), partition_no + 1);
		
		get_partition_name(dev_name, disk_no, partition_no);
		
		hd_mount_partition(dev_name,dir_name);
	}

	return ret;
}

/******************************************************************************
* �������ƣ�hd_cur_partition_full
* �������������Ӳ���Ƿ���
* ���������int disk_no				Ӳ�̺�
*			int partition_no		������
* �����������
* �� �� ֵ��δ��:0; ��: 1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_cur_partition_full(int disk_no, int partition_no)
{
	char partition_name[MAX_PATH];
	DISKSIZE disk_size;

	get_partition_name(partition_name,disk_no,partition_no);
	tm3k_HD_GetDiskInfo(partition_name,&disk_size);
	g_hard_disk_info.disk_info[disk_no].partition_info[partition_no].availabe_size = disk_size.availablesize;

	// ��Ӳ�̿ռ�С��300M,����Ϊ���Ӳ��û�пռ��� 
	if (disk_size.availablesize < 300) 
	{
		return 1;
	}

	return 0;
}

/******************************************************************************
* �������ƣ�hd_get_disk_info_for_frontpanel
* �������������Ӳ���������Ƿ��� <ǰ�����>
* �����������
* ���������unsigned char *disk_num		Ӳ�̴��ڱ�־
*			unsigned char *disk_full	Ӳ������־
* �� �� ֵ��0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_get_disk_info_for_frontpanel(unsigned char *disk_num, unsigned char *disk_full)
{
	int i;
	int hd_num;
	unsigned char disk_num_status  = 0;
	unsigned char disk_full_status = 0;

	if (disk_num==NULL ||disk_full==NULL)
	{
		return -1;
	}
	
	hd_num = get_hard_disk_num();

	for(i = 0; i < hd_num; i++)
	{
		disk_num_status |= (1<<i);
		if (g_hard_disk_info.disk_info[i].partition_info[0].availabe_size < 300 )
		{
			disk_full_status |= (1<<i);
		}
	}

	*disk_num  = disk_num_status;
	*disk_full = disk_full_status;

	return 0;
}

/******************************************************************************
* �������ƣ�hd_find_empty_partition
* ��������������δ������(���������ݷ���)
* �����������
* ���������int *disk_no		Ӳ�̺�
*			int *partition_no	������
* �� �� ֵ���ɹ�:0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_find_empty_partition(int *disk_no,int *partition_no)
{
	int i;
	int ret = 0;
	char partition_name[MAX_PATH];
	DISKSIZE disk_size;
	int  disk_num;

	*disk_no = -1;
	*partition_no = -1;

	// ֻ�������ݷ���,ÿ��Ӳ�̵ĵ�һ����Ϊ������ 
	disk_num = get_hard_disk_num();
	
	for (i=0; i<disk_num; i++)
	{
		if ( g_hard_disk_info.disk_info[i].partition_info[0].format_flag == 1 )	//�Ƿ��ʽ��
		{							
			//��Ϊ���ݷ���
			if (g_hard_disk_info.disk_info[i].partition_info[0].mount_flag && g_hard_disk_info.disk_info[i].partition_info[0].type != 2 )
			{
				get_partition_name(partition_name,i,0);
				
				ret = tm3k_HD_GetDiskInfo(partition_name,&disk_size);

				g_hard_disk_info.disk_info[i].partition_info[0].availabe_size = disk_size.availablesize;

				// ��Ӳ�̿ռ����300M,����Ϊ���Ӳ���пռ� 
				if (disk_size.availablesize >= 300)
				{
					*disk_no = i;
					*partition_no = 0;
					break;
				}
			}
		}
	}

	if (*disk_no == -1 || *partition_no == -1)
		return -1;
	else
		return 0;
}

/******************************************************************************
* �������ƣ�hd_query_disk_info
* ������������ʱ��ѯӲ����Ϣ(mainMenuDlg.c�ڶ�ʱ���е���)
* �����������
* �����������
* �� �� ֵ��2: Ӳ������ͣ¼��,ɾ�������ļ�
*			1: Ӳ������ͣ¼��,������ʾ
*			0: ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_query_disk_info()
{
	int cur_disk_no;
	int cur_partition_no;
	int cover_mode = 0;
	int ret = 0;
	int disk_num = 0;
	cur_disk_no = g_hard_disk_info.cur_disk_no;
	cur_partition_no = g_hard_disk_info.cur_partition_no;

	disk_num = get_hard_disk_num();
	if (disk_num <= 0)
	{
		return -1;
	}
		
	if (hd_cur_partition_full(cur_disk_no,cur_partition_no))	// ��ǰ������ 
	{
		if (get_del_file_status() == 1)					//����ɾ���ļ�
		{
			return 0;
		}

		if (hd_find_empty_partition(&cur_disk_no, &cur_partition_no) == 0) // ��һ���յķ��� 
		{
			g_hard_disk_info.cur_disk_no = cur_disk_no;
			g_hard_disk_info.cur_partition_no = cur_partition_no;

			if(g_file_delete_status == 1)
			{
				g_file_delete_status = 0;
			}
		}
		else // û��Ӳ�̿ռ�,ֹͣ¼����߸����ϵ�¼���ļ� 
		{
			//cover_mode = getRecordCoverMode(); // [zhb][add][2006-03-01]
			if (cover_mode == 0)
			{
				//ɾ�������ļ�
				del_oldest_record_file();
				
				g_file_delete_status = 1;

				ret = 2;				//Ӳ������ͣ¼��,ɾ�������ļ� 
			}
			else
			{
				//��ͣϵͳ
				g_file_delete_status = 1;
				//nvsRecorderPause();
				ret = 1;				//Ӳ������ͣ¼��,������ʾ   
			}
		}
	}
	else		// ��ǰ��δ�� 
	{
		if (g_file_delete_status == 1)
		{
			g_file_delete_status = 0;
		}
		ret = 3;
	}
	return ret;
}

int hd_query_disk_info_ext()
{
	static int first_query_flag = 1;
	int i = 0;
	int j = 0;
	int cur_disk_no;
	int cur_partition_no;
	int cover_mode = 0;
	int ret = 0;
	int disk_num = 0;
	char *disk_name = NULL;
	char partition_name[MAX_PATH];
	DISKSIZE disk_size;
	
	cur_disk_no = g_hard_disk_info.cur_disk_no;
	cur_partition_no = g_hard_disk_info.cur_partition_no;

	disk_num = get_hard_disk_num_ext();
	if (disk_num <= 0)
	{
		printf("No found hard disk!\n");
		return -1;
	}

	for (i=0; i<disk_num; i++)
	{
		if (g_hard_disk_info.disk_info[i].partition_info[0].format_flag == 0) // not formated
		{
			// Add the code by lvjh, 2008-01-10
			unsigned long disksize = 0;

			disk_name = tm3k_HD_GetDiskName(i);
			ret = tm3k_HD_GetDiskInfo_ext(disk_name, &disksize);
			if (ret < 0)
			{
				printf("tm3k_HD_GetDiskInfo_ext(%s): Failed!n", disk_name);
				continue;
			}
			else
			{
				for (j=0; j<1; j++)
				{
					g_hard_disk_info.disk_info[i].partition_info[j].mount_flag = 0;
					g_hard_disk_info.disk_info[i].partition_info[j].format_flag = 0;
					g_hard_disk_info.disk_info[i].partition_info[j].total_size = disksize;
					g_hard_disk_info.disk_info[i].partition_info[j].used_size = 0;
					g_hard_disk_info.disk_info[i].partition_info[j].availabe_size = 0;
					g_hard_disk_info.disk_info[i].partition_info[j].used_parent = 0;
				}
			}
		}
		else
		{
			for (j=0; j<2; j++)
			{
				get_partition_name(partition_name, i, j);

				printf("(%d %d)Partition Name: %s\n", i, j, partition_name);

				ret = tm3k_HD_GetDiskInfo(partition_name, &disk_size);
				if (!ret)
				{
					g_hard_disk_info.disk_info[i].partition_info[j].total_size = disk_size.totalsize;
					g_hard_disk_info.disk_info[i].partition_info[j].used_size = disk_size.usedsize;
					g_hard_disk_info.disk_info[i].partition_info[j].availabe_size = disk_size.availablesize;
					g_hard_disk_info.disk_info[i].partition_info[j].used_parent = disk_size.usedpercent;
				}		
			}
		}

	}

	/*
	if (first_query_flag)
	{
		for (i=0; i<disk_num; i++)
		{
			// Add the code by lvjh, 2008-01-10
			unsigned long disksize = 0;

			disk_name = tm3k_HD_GetDiskName(i);
			ret = tm3k_HD_GetDiskInfo_ext(disk_name, &disksize);
			if (ret < 0)
			{
				continue;
			}
			else
			{

			}

			//if (g_hard_disk_info.disk_info[i].formated == 1) // formated
			{
				for (j=0; j<2; j++)
				{
					//if (g_hard_disk_info.disk_info[i].partition_info[j].mount_flag)
					{
						get_partition_name(partition_name, i, j);

						printf("(%d %d)Partition Name: %s\n", i, j, partition_name);

						ret = tm3k_HD_GetDiskInfo(partition_name, &disk_size);
						if (!ret)
						{
							g_hard_disk_info.disk_info[i].partition_info[j].total_size = disk_size.totalsize;
							g_hard_disk_info.disk_info[i].partition_info[j].used_size = disk_size.usedsize;
							g_hard_disk_info.disk_info[i].partition_info[j].availabe_size = disk_size.availablesize;
							g_hard_disk_info.disk_info[i].partition_info[j].used_parent = disk_size.usedpercent;
						}
					}
				}
			}
		}
		first_query_flag = 0;
	}
	else
	{
		//if (g_hard_disk_info.disk_info[cur_disk_no].formated == 1) // formated
		{
			for (j=0; j<2; j++)
			{
				//if (g_hard_disk_info.disk_info[cur_disk_no].partition_info[j].mount_flag)
				{
					get_partition_name(partition_name, cur_disk_no, j);

					printf("(%d %d)Partition Name: %s\n", i, j, partition_name);

					ret = tm3k_HD_GetDiskInfo(partition_name, &disk_size);
					if (!ret)
					{
						g_hard_disk_info.disk_info[cur_disk_no].partition_info[j].total_size = disk_size.totalsize;
						g_hard_disk_info.disk_info[cur_disk_no].partition_info[j].used_size = disk_size.usedsize;
						g_hard_disk_info.disk_info[cur_disk_no].partition_info[j].availabe_size = disk_size.availablesize;
						g_hard_disk_info.disk_info[cur_disk_no].partition_info[j].used_parent = disk_size.usedpercent;
					}
				}
			}
		}
	}
	*/

	return 0;
}

/******************************************************************************
* �������ƣ�int hd_get_disk_total_size
* ������������ȡ����Ӳ��������
* �����������
* �����������
* �� �� ֵ������ֵ������Ӳ��������
* �޸ļ�¼: ��
* ����˵��: ��

********************************************************************************/
int hd_get_disk_total_size()
{
	int i,j;
	int disk_num;
	int partition_num;
	int total_size = 0;

	disk_num = get_hard_disk_num();
	for(i=0; i<disk_num; i++)
	{
		partition_num = tm3k_HD_GetDiskPartionNum(tm3k_HD_GetDiskName(i));
		for(j=0; j<partition_num; j++)
		{
			total_size += hd_get_partition_total_size(i,j);
		}
	}

	return total_size;
}

/******************************************************************************
* �������ƣ�hd_get_disk_used_size
* ������������ȡ����Ӳ�̵����ÿռ�
* �����������
* �����������
* �� �� ֵ������ֵ������Ӳ�̵����ÿռ�
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_get_disk_used_size()
{
	int i,j;
	int disk_num;
	int partition_num;
	int used_size = 0;

	disk_num = get_hard_disk_num();
	
	for(i=0; i<disk_num; i++)
	{
		partition_num = tm3k_HD_GetDiskPartionNum(tm3k_HD_GetDiskName(i));
		for(j=0; j<partition_num; j++)
		{
			used_size += hd_get_partition_used_size(i,j);
		}
	}
	
	g_hard_disk_info.hard_disk_num = disk_num;
	
	return used_size;
}


/******************************************************************************
* �������ƣ�hd_get_disk_available_size
* ������������ȡ����Ӳ�̵Ŀ��ÿռ�
* �����������
* �����������
* �� �� ֵ������ֵ������Ӳ�̵Ŀ��ÿռ�
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_get_disk_available_size()
{
	int i,j;
	int disk_num;
	int partition_num;
	int available_size = 0;

	disk_num = get_hard_disk_num();
	
	for(i=0; i<disk_num; i++)
	{
		partition_num = tm3k_HD_GetDiskPartionNum(tm3k_HD_GetDiskName(i));
		for(j=0; j<partition_num; j++)
		{
			available_size += hd_get_partition_available_size(i,j);
		}
	}

	return available_size;
}

/******************************************************************************
* �������ƣ�hd_get_partition_total_size
* ������������ȡ����������
* ���������int disk_no			Ӳ�̺�
*			int partition_no	������
* �����������
* �� �� ֵ������ֵ������������
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_get_partition_total_size(int disk_no,int partition_no)
{
	char partition_name[MAX_PATH];
	DISKSIZE disk_size;

	get_partition_name(partition_name,disk_no,partition_no);
	tm3k_HD_GetDiskInfo(partition_name,&disk_size);

	g_hard_disk_info.disk_info[disk_no].partition_info[partition_no].availabe_size = disk_size.availablesize;

	return disk_size.totalsize;
}

/******************************************************************************
* �������ƣ�hd_get_partition_used_size
* ������������ȡ�������ÿռ�
* ���������int disk_no			Ӳ�̺�
*			int partition_no	������
* �����������
* �� �� ֵ������ֵ���������ÿռ�
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_get_partition_used_size(int disk_no,int partition_no)
{
	char partition_name[MAX_PATH];
	DISKSIZE disk_size;

	get_partition_name(partition_name,disk_no,partition_no);
	tm3k_HD_GetDiskInfo(partition_name,&disk_size);

	g_hard_disk_info.disk_info[disk_no].partition_info[partition_no].availabe_size = disk_size.availablesize;

	return disk_size.usedsize;
}

/******************************************************************************
* �������ƣ�hd_get_partition_available_size
* ������������ȡ�������ÿռ�
* ���������int disk_no			Ӳ�̺�
*			int partition_no	������
* �����������
* �� �� ֵ������ֵ���������ÿռ�
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_get_partition_available_size(int disk_no,int partition_no)
{
	char partition_name[MAX_PATH];
	DISKSIZE disk_size;

	get_partition_name(partition_name,disk_no,partition_no);
	tm3k_HD_GetDiskInfo(partition_name,&disk_size);

	g_hard_disk_info.disk_info[disk_no].partition_info[partition_no].availabe_size = disk_size.availablesize;

	return disk_size.availablesize;
}

/******************************************************************************
* �������ƣ�hd_get_backup_partition_num
* ������������ȡ���ݷ�����(��mount�ɹ��ı�����)
* �����������
* �����������
* �� �� ֵ������ֵ�����ݷ�����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_get_backup_partition_num()
{
	int i;
	int backup_partition_num;

	backup_partition_num = 0;
	for (i=0; i<MAX_DISK_NUM; i++)
	{
		if (g_hard_disk_info.disk_info[i].partition_info[1].mount_flag)
			backup_partition_num++;
	}

	return backup_partition_num;
}

/******************************************************************************
* �������ƣ�hd_get_backup_disk_no
* ����������ͨ�����ݷ���������ñ���Ӳ�̺�
* ���������int backup_partition_no		���ݷ�����
* �����������
* �� �� ֵ���ɹ�������Ӳ�̺�; ʧ�ܣ�-1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hd_get_backup_disk_no(int backup_partition_no)
{
	int i;
	int backup_partition_num;
	int disk_no;
	int partition_no;

	backup_partition_num = hd_get_backup_partition_num();
	if (backup_partition_num <= 0 || backup_partition_no >= backup_partition_num)
		return -1;

	disk_no = -1;
	partition_no = -1;
	for (i=0; i<MAX_DISK_NUM; i++)		//backup_partition_num
	{
		if (g_hard_disk_info.disk_info[i].partition_info[1].mount_flag)
			partition_no++;

		if (partition_no == backup_partition_no)
		{
			disk_no = i;
			return  disk_no;
		}
	}

	return disk_no;
}

/******************************************************************************
* �������ƣ�get_fdisk_format_process
* ������������ȡ������ʽ�������Ľ���
* �����������
* �����������
* �� �� ֵ������ֵ��������ʽ�������Ľ���ֵ
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_fdisk_format_process()
{
	return fdisk_format_process;
}

/******************************************************************************
* �������ƣ�get_fdisk_format_status
* ������������ȡ������ʽ�������Ľ��
* �����������
* �����������
* �� �� ֵ������ֵ��������ʽ�������Ľ��
* �޸ļ�¼: ��

* ����˵��: ��
********************************************************************************/
int get_fdisk_format_status()
{
	return fdisk_format_status;
}

static long kscale(long b, long bs)
{
    return ( b * (long long) bs + KILOBYTE/2 ) / KILOBYTE;
}

static long mscale(long b, long bs)
{
    return ( b * (long long) bs + 1024 ) / MILOBYTE;
}

/******************************************************************************
* �������ƣ�get_fdisk_format_status
* ���������������mountĿ¼��ʣ��ռ�
* ���������char *mount_point MOUNT·��
* �����������
* �� �� ֵ��>= 0 �ռ��С,MΪ��λ,< 0 mount_point�����ڵ�
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_disk_avail_size(char *mount_point)
{
	long avail_size = 0;
	struct statfs s;

	if (statfs(mount_point, &s) != 0)
	{
		return -1;
	}

	if (s.f_blocks > 0)
	{
		avail_size  = mscale(s.f_bavail, s.f_bsize);
	}

	return avail_size;
}

//=====================================================================
//CDRecord ���
//=====================================================================
char *CDRecorderName = "/dev/scd0";

/******************************************************************************
* �������ƣ�CDRecord_GetDeviceName
* ������������ȡCD�豸����
* �����������
* �����������
* �� �� ֵ���ַ�����CD�豸����(/dev/scd0)
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
char *CDRecord_GetDeviceName(void)
{
   	return CDRecorderName;
}

/******************************************************************************
* �������ƣ�CDRecord_GetDeviceName
* �������������CDRecorder�Ƿ����
* �����������
* �����������
* �� �� ֵ����: 1; ��: 0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int Check_CDRecorder()
{
	//int  fd;
	//int  ret;
	//char buf[128];
	FILE *fp = NULL;
    int  nCDRecorderNum = 0;

    if ((fp = fopen(CDRecorderName, "r")) == NULL)
	{
		nCDRecorderNum = 0;
		printf("not find cd\n");
    }
	else
	{
		fclose(fp);
		nCDRecorderNum = 1;
		printf("find cd\n");
	}
	return nCDRecorderNum;
}


//=====================================================================
//USB ���
//=====================================================================
int   UsbIndex[4] = {-1, -1, -1, -1};
char *USBName[4]  = {"/dev/sda1", "/dev/sdb1", "/dev/sdc1", "/dev/sdd1"};

/******************************************************************************
* �������ƣ�usb_GetUSBName
* ������������ȡUSB�洢�豸����
* �����������
* �����������
* �� �� ֵ���ַ�����USB�洢�豸����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
char *usb_GetUSBName(int no)
{
    int       i;

    if( (UsbIndex[no] < 0)  ||   (UsbIndex[no] > 3))
      return NULL;

    i = UsbIndex[no];
    if( i >= 0 && i < 4)
    	return USBName[i];
    else
		return NULL;
}

/******************************************************************************
* �������ƣ�CDRecord_GetDeviceName
* �������������USB���Ƿ����
* �����������
* �����������
* �� �� ֵ������ֵ,USB�̵ĸ���
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int check_usb_disk_num()
{
	int  fd;
	int  ret;
	int  i;
	char buf[128];
    int  nUsbNum = 0;

	UsbIndex[0] = -1;
	UsbIndex[1] = -1;
	UsbIndex[2] = -1;
	UsbIndex[3] = -1;

	for(i = 0; i < 4; i++)
	{
		fd = open(USBName[i], O_RDWR );         //| O_NOCTTY | O_NDELAY
 		if(fd >= 0)
		{
			ret = read(fd, buf, 128);
			if(ret > 0)
			{
				close(fd);
			    UsbIndex[nUsbNum] = i;
             	nUsbNum ++;
			}
			close(fd);
		}
	}
	return nUsbNum;
}

/******************************************************************************
* �������ƣ�usb_mount_partition
* ����������MOUNT USB�̵ķ���
* ���������int no USB�̺�(0-3)
* �����������
* �� �� ֵ���ɹ�:0; ʧ��:-1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int usb_mount_partition(int no)		//no:   0----3
{
	char usb_name[32];
	int  i = 0;

	if(no < 0 || no > 3)
	   return -1;

	if( (UsbIndex[no] < 0)  ||   (UsbIndex[no] > 3))
		return -1;

	i = UsbIndex[no];

	memset(usb_name, 0, sizeof(usb_name));
	sprintf(usb_name, "/usb%d", no);
	return hd_mount_partition(USBName[i], usb_name);
}

/******************************************************************************
* �������ƣ�usb_mount_partition
* ����������UMOUNT USB�̵ķ���
* ���������int no USB�̺�(0-3)
* �����������
* �� �� ֵ���ɹ�:0; ʧ��:-1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int usb_umount_partition(int no)	//no:   0----3
{
	char usb_name[32];

	if(no < 0 || no > 3)
	   return -1;

	memset(usb_name, 0, sizeof(usb_name));
	sprintf(usb_name, "/usb%d", no);

	return hd_umount_partition(usb_name);
}
