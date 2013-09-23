/* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�sdCard.c
* �ļ�˵�������ļ������˲���Ӳ�̵ĺ����Ķ���
* �����б�
*    ����1��
*        �������ƣ�get_hard_disk_info
*        �������ܣ���ȡӲ����Ϣ
*    ����2��
*        �������ƣ�get_partition_name
*        �������ܣ���÷�����
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
#include <errno.h>

#include "sdCard.h"
#include "fdisk.h"

#ifndef MAX_PATH
#define MAX_PATH	256
#endif

static SD_CARD_INFO g_sd_card_info;
static char g_sd_dev_name[128];

static char *g_usb_dev_name1 = "/dev/scsi/host0/bus0/target0/lun0/disc";	// Add the code by lvjh, 2008-03-12
static char *g_usb_dev_name2 = "/dev/scsi/host0/bus0/target0/lun0/part1";	// Add the code by lvjh, 2008-08-18

static char *g_sd_dev_name1 = "/dev/mmc/blk0/disc";	// Add the code by lvjh, 2008-03-12
static char *g_sd_dev_name2 = "/dev/mmc/blk0/part1";

static char *g_sd_mount_name = "/record/hd00/00/";

static int fdisk_format_process = 100;			//Ӳ�̸�ʽ������
static int fdisk_format_status  = 0;			//Ӳ�̸�ʽ��״̬
static int g_file_delete_status = 0;			//�ļ�ɾ��״̬

/******************************************************************************
* �������ƣ�sd_check
* �������������USB���Ƿ����
* �����������
* �����������
* �� �� ֵ������ֵ,USB�̵ĸ���
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int sd_check()
{
	int fd = -1;

	fd = open(g_sd_dev_name, O_RDWR);
	if (fd < 0)
	{
		fd = open(g_sd_dev_name, O_RDONLY);
		if (fd < 0)
		{
			return -1;
		}
		close(fd);

		return 1;
	}

	close(fd);
	
	return 0;
}

/******************************************************************************
* �������ƣ�sd_mount
* ����������MOUNT����
* ���������char *source	Ӳ�̷����豸����
*			char *target	Ӳ�̷�������MOUNTĿ������
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
/*
int sd_mount()
{
	int ret = -1;
	int i = 0;

	ret = tm3k_HD_IsDiskFormated(g_sd_dev_name);
	if (ret < 0)
	{
		g_sd_card_info.formated = 0;
		
		//return -1;
	}

	g_sd_card_info.formated = 1;
	
	do
	{
		ret = mount(g_sd_dev_name, g_sd_mount_name, "vfat", 0, NULL);
		if (ret != 0)
		{
			printf("Mount1(%s %s %d): Failed!\n", g_sd_dev_name, g_sd_mount_name, ret);
			printf("Mount1: %d %s\n", errno, strerror(errno));

			// Add the code by lvjh, 2008-05-14
			ret = mount(g_sd_dev_name, g_sd_mount_name, "vfat", MS_RDONLY, NULL);
			if (ret != 0)
			{
				printf("Mount2(%s %s %d): Failed!\n", g_sd_dev_name, g_sd_mount_name, ret);
				printf("Mount2: %d %s\n", errno, strerror(errno));
			}
			else
			{
				g_sd_card_info.rdwr_flag = 1;
				g_sd_card_info.mount_flag = 1;
				return 0;
			}
			usleep(100*1000);
		}
		else
		{
			g_sd_card_info.rdwr_flag = 0;
			g_sd_card_info.mount_flag = 1;

			return 0;
		}
		
		i++;
	}while(i < 10 ); //��ೢ��10��

	return -1;
}
*/

int sd_mount()
{
	int ret = -1;
	int i = 0;
	
	// 
	do
	{
		ret = mount(g_sd_dev_name1, g_sd_mount_name, "vfat", 0, NULL);
		if (ret != 0)
		{
			//printf("Mount1(%s %s %d)(%d %s): Failed!\n", g_sd_dev_name1, g_sd_mount_name, ret, errno, strerror(errno));

			// Add the code by lvjh, 2008-05-14
			ret = mount(g_sd_dev_name1, g_sd_mount_name, "vfat", MS_RDONLY, NULL);
			if (ret != 0)
			{
				//printf("Mount2(%s %s %d)(%d %s): Failed!\n", g_sd_dev_name1, g_sd_mount_name, ret, errno, strerror(errno));
			}
			else
			{
				g_sd_card_info.rdwr_flag = 0;
				g_sd_card_info.mount_flag = 1;
			
				strcpy(g_sd_dev_name, g_sd_dev_name1);
				
				goto CHECK_FORMAT;
			}
			usleep(10*1000);
		}
		else
		{
			g_sd_card_info.rdwr_flag = 1;
			g_sd_card_info.mount_flag = 1;

			strcpy(g_sd_dev_name, g_sd_dev_name1);

			goto CHECK_FORMAT;
		}
		
		i++;
	}while(i < 2 ); //��ೢ��2��
	
	do
	{
		ret = mount(g_sd_dev_name2, g_sd_mount_name, "vfat", 0, NULL);
		if (ret != 0)
		{
			//printf("Mount1(%s %s %d)(%d %s): Failed!\n", g_sd_dev_name2, g_sd_mount_name, ret, errno, strerror(errno));

			// Add the code by lvjh, 2008-05-14
			ret = mount(g_sd_dev_name2, g_sd_mount_name, "vfat", MS_RDONLY, NULL);
			if (ret != 0)
			{
				//printf("Mount2(%s %s %d)(%d %s): Failed!\n", g_sd_dev_name2, g_sd_mount_name, ret, errno, strerror(errno));
			}
			else
			{
				g_sd_card_info.rdwr_flag = 0;
				g_sd_card_info.mount_flag = 1;

				strcpy(g_sd_dev_name, g_sd_dev_name2);
				
				goto CHECK_FORMAT;
			}
			usleep(10*1000);
		}
		else
		{
			g_sd_card_info.rdwr_flag = 1;
			g_sd_card_info.mount_flag = 1;

			strcpy(g_sd_dev_name, g_sd_dev_name2);

			goto CHECK_FORMAT;
		}
		
		i++;
	}while(i < 10 ); //��ೢ��10��
	
	do
	{
		ret = mount(g_usb_dev_name1, g_sd_mount_name, "vfat", 0, NULL);
		if (ret != 0)
		{
		//	printf("Mount1(%s %s %d)(%d %s): Failed!\n", g_usb_dev_name1, g_sd_mount_name, ret, errno, strerror(errno));

			// Add the code by lvjh, 2008-05-14
			ret = mount(g_usb_dev_name1, g_sd_mount_name, "vfat", MS_RDONLY, NULL);
			if (ret != 0)
			{
			//	printf("Mount2(%s %s %d)(%d %s): Failed!\n", g_usb_dev_name1, g_sd_mount_name, ret, errno, strerror(errno));
			}
			else
			{
				g_sd_card_info.rdwr_flag = 0;
				g_sd_card_info.mount_flag = 1;
			
				strcpy(g_sd_dev_name, g_usb_dev_name1);
				
				goto CHECK_FORMAT;
			}
			usleep(10*1000);
		}
		else
		{
			g_sd_card_info.rdwr_flag = 1;
			g_sd_card_info.mount_flag = 1;

			strcpy(g_sd_dev_name, g_usb_dev_name1);

			goto CHECK_FORMAT;
		}
		
		i++;
	}while(i < 10 ); //��ೢ��10��
	
	do
	{
		ret = mount(g_usb_dev_name2, g_sd_mount_name, "vfat", 0, NULL);
		if (ret != 0)
		{
		//	printf("Mount1(%s %s %d)(%d %s): Failed!\n", g_usb_dev_name2, g_sd_mount_name, ret, errno, strerror(errno));

			// Add the code by lvjh, 2008-05-14
			ret = mount(g_usb_dev_name2, g_sd_mount_name, "vfat", MS_RDONLY, NULL);
			if (ret != 0)
			{
				//printf("Mount2(%s %s %d)(%d %s): Failed!\n", g_usb_dev_name2, g_sd_mount_name, ret, errno, strerror(errno));
			}
			else
			{
				g_sd_card_info.rdwr_flag = 0;
				g_sd_card_info.mount_flag = 1;

				strcpy(g_sd_dev_name, g_usb_dev_name2);
				
				goto CHECK_FORMAT;
			}
			usleep(10*1000);
		}
		else
		{
			g_sd_card_info.rdwr_flag = 1;
			g_sd_card_info.mount_flag = 1;

			strcpy(g_sd_dev_name, g_usb_dev_name2);

			goto CHECK_FORMAT;
		}
		
		i++;
	}while(i < 10 ); //��ೢ��10��

	strcpy(g_sd_dev_name, g_sd_dev_name1);
	

	return -1;

CHECK_FORMAT:
	ret = tm3k_HD_IsDiskFormated(g_sd_dev_name);
	if (ret < 0)
	{
		g_sd_card_info.formated = 0;
		
		//return -1;
	}

	
	system("mount -o remount,rw /record/hd00/00/");
	g_sd_card_info.formated = 1;

	return 0;
}

/******************************************************************************
* �������ƣ�sd_umount
* ����������UMOUNT����
* ���������char *target	Ӳ�̷�������UMOUNTĿ������
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int sd_umount()
{
	int ret = -1;
	int i = 0;
	char cmd[128];

	// Add the code by Jerry, 2008-11-10
	if (g_sd_card_info.mount_flag == 0)
	{
		return 0;
	}
	
	sprintf(cmd, "umount %s", g_sd_mount_name);

	do
	{
		//ret = umount(g_sd_mount_name);
		ret = umount2(g_sd_mount_name, MNT_FORCE);
		if(ret != 0)
		{
			printf("UnMount(%s): Failed!\n", g_sd_mount_name);
			usleep(100*1000);
		}
		else
		{
			g_sd_card_info.mount_flag = 0;

			printf("sd_umount(): OK!\n");
			
			return 0;
		}

		i++;
	}while(i < 10 );	//��ೢ��10��

	printf("sd_umount(): Failed!\n");

	return -1;
}

/******************************************************************************
* �������ƣ�sd_get_info
* ������������ȡӲ����Ϣ
* �����������
* ���������SD_CARD_INFO
* �� �� ֵ��0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int sd_get_info(SD_CARD_INFO *sd_card_info)
{
	if (sd_card_info == NULL)
	{
		return -1;
	}
	if (sd_check() < 0)
	{
		return -1;
	}
	
	memcpy(sd_card_info, &g_sd_card_info, sizeof(SD_CARD_INFO) );
	
	return 0;
}

int sd_get_mount_flag()
{
	return g_sd_card_info.mount_flag;
}

int sd_get_readwrite_flag()
{
	return 1;
}

/******************************************************************************
* �������ƣ�sd_get_disk_formated
* ������������ȡӲ�̸�ʽ����־
* �����������
* �����������
* �� �� ֵ������ֵ����ʽ����־
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int sd_get_formated()
{
	return g_sd_card_info.formated;
}

/******************************************************************************
* �������ƣ�sd_fdisk
* ��������������Ӳ�̷���
* �����������
* �����������
* �� �� ֵ���ɹ�:0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int sd_fdisk()
{
	int ret = -1;

	ret = sd_check();
	if (ret != 0)
	{
		return -1;
	}

	ret = sd_umount();
	if (ret < 0)
	{
		return -1;
	}

	// ������ʽ����Ӳ�� 
	ret = tm3k_HD_FDisk(g_sd_dev_name, 100, 0);
	if (ret < 0 )
	{
		sd_mount();
		
		return -1;
	}

	sd_mount();

	return 0;
}

/******************************************************************************
* �������ƣ�sd_format
* ������������ʽ����һ����
* �����������
* �����������
* �� �� ֵ���ɹ�:0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int sd_format()
{
	int ret = -1;
	char buffer[128];

	ret = sd_check();
	if (ret != 0)
	{
		return -1;
	}
	// Add the code by lvjh, 2009-05-27
	printf("g_sd_card_info.rdwr_flag = %d\n", g_sd_card_info.rdwr_flag);
#if 0
	if (g_sd_card_info.rdwr_flag != 1)
	{
		printf("sd_format error!!\n");
		return -1;
	}		
#endif

	ret = sd_umount();
	if (ret < 0 )
	{
		return -1;
	}

	// ������ʽ����Ӳ�� 
	//ret = tm3k_HD_Format(g_sd_dev_name, 1);
	sprintf(buffer, "/mnt/mtd/dvs/app/mkdosfs %s -I", g_sd_dev_name);	// Add the code by lvjh, 2008-04-26
	ret = system(buffer);
	if (ret < 0 )
	{
		sd_mount();
		
		return -1;
	}

	sd_mount();

	return 0;
}

/******************************************************************************
* �������ƣ�sd_get_full
* �������������Ӳ���Ƿ���
* ���������int disk_no				Ӳ�̺�
*			int partition_no		������
* �����������
* �� �� ֵ��δ��:0; ��: 1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int sd_get_full()
{
	DISKSIZE disk_size;
	
	tm3k_HD_GetDiskInfo(g_sd_dev_name, &disk_size);
	g_sd_card_info.availabe_size = disk_size.availablesize;

	printf("sd_get_full: %d %d\n", g_sd_card_info.total_size, g_sd_card_info.availabe_size);

	// ��Ӳ�̿ռ�С����������5%,����Ϊ���Ӳ��û�пռ��� 
	//if (disk_size.availablesize < g_sd_card_info.total_size/20)
	//if (disk_size.availablesize < g_sd_card_info.total_size/3)  // test
	if (disk_size.availablesize < 100)  // add the code by lvjh, 2008-10-29
	{
		return 1;
	}

	return 0;
}

int sd_query_info()
{
	int ret = -1;
	unsigned long size = 0;
	DISKSIZE disk_size;

	ret = sd_check();
	if (ret < 0)
	{
		printf("sd_check(): Failed!\n");

		g_sd_card_info.total_size = 0;
		g_sd_card_info.used_size = 0;
		g_sd_card_info.availabe_size = 0;
		g_sd_card_info.used_parent = 0;

		return -1;
	}

	if (g_sd_card_info.mount_flag)
	{
		ret = tm3k_HD_GetDiskInfo(g_sd_dev_name, &disk_size);
	}
	else
	{
		ret = tm3k_HD_GetDiskInfo_ext(g_sd_dev_name, &size);	// Add the code by lvjh, 2008-05-14
	}

	if (ret < 0 )
	{
		printf("tm3k_HD_GetDiskInfo(%s): Failed!\n", g_sd_dev_name);
		return -1;
	}
	g_sd_card_info.rdwr_flag = 1;

	if (g_sd_card_info.mount_flag)	
	{
	//	if (g_sd_card_info.rdwr_flag == 1)	// Add the code by lvjh, 2009-05-27
		{
			g_sd_card_info.total_size = disk_size.totalsize;
			g_sd_card_info.used_size = disk_size.usedsize;
			g_sd_card_info.availabe_size = disk_size.availablesize;
			g_sd_card_info.used_parent = disk_size.usedpercent;
		}
#if 0		
		else
		{
			printf("SD Cord Only Read!\n");
			g_sd_card_info.total_size = disk_size.totalsize;
			g_sd_card_info.used_size = disk_size.usedsize;
			g_sd_card_info.availabe_size = 0;
			g_sd_card_info.used_parent = disk_size.usedpercent;
		}
#endif

	}
	else
	{
		g_sd_card_info.total_size = size;
		g_sd_card_info.used_size = 0;
		g_sd_card_info.availabe_size = 0;
		g_sd_card_info.used_parent = 0;
	}

	printf("SD: %d %d %d %d\n", g_sd_card_info.total_size, g_sd_card_info.used_size, g_sd_card_info.availabe_size, g_sd_card_info.used_parent);

	return 0;
}

/******************************************************************************
* �������ƣ�sd_get_total_size
* ������������ȡ����������
* ���������int disk_no			Ӳ�̺�
*			int partition_no	������
* �����������
* �� �� ֵ������ֵ������������
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int sd_get_total_size()
{
	DISKSIZE disk_size;

	tm3k_HD_GetDiskInfo(g_sd_dev_name, &disk_size);

	g_sd_card_info.total_size = disk_size.totalsize;

	return disk_size.totalsize;
}

/******************************************************************************
* �������ƣ�sd_get_used_size
* ������������ȡ�������ÿռ�
* ���������int disk_no			Ӳ�̺�
*			int partition_no	������
* �����������
* �� �� ֵ������ֵ���������ÿռ�
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int sd_get_used_size()
{
	DISKSIZE disk_size;

	tm3k_HD_GetDiskInfo(g_sd_dev_name, &disk_size);

	g_sd_card_info.used_size = disk_size.usedsize;

	return disk_size.usedsize;
}

/******************************************************************************
* �������ƣ�sd_get_available_size
* ������������ȡ�������ÿռ�
* ���������int disk_no			Ӳ�̺�
*			int partition_no	������
* �����������
* �� �� ֵ������ֵ���������ÿռ�
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int sd_get_available_size()
{
	DISKSIZE disk_size;

	tm3k_HD_GetDiskInfo(g_sd_dev_name, &disk_size);

	g_sd_card_info.availabe_size = disk_size.availablesize;

	return disk_size.availablesize;
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
int sd_get_fdisk_format_process()
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
int sd_get_fdisk_format_status()
{
	return fdisk_format_status;
}

