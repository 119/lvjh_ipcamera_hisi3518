/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�hdPowerManage.c
* �ļ�˵�������ļ������˲���Ӳ�̵ĺ����Ķ���
* �����б�
*    ����1��
*        �������ƣ�set_hd_APM
*        �������ܣ�����Ӳ�̸߼���Դ����
*    ����2��
*        �������ƣ�set_hd_standby
*        �������ܣ�����Ӳ�̵�״̬Ϊ����״̬
*    ����3��
*        �������ƣ�set_hd_sleep
*        �������ܣ�����Ӳ�̵�״̬Ϊ˯��״̬
*    ����4��
*        �������ƣ�set_hd_idle
*        �������ܣ�����Ӳ�̵�״̬Ϊ����״̬
*    ����5��
*        �������ƣ�restore_hd_ready_force
*        �������ܣ�ǿ�ƻָ�Ӳ�̾���״̬
*    ����6��
*        �������ƣ�soft_reset_hd
*        �������ܣ���λӲ��(ATAPI)
*    ����7��
*        �������ƣ�hard_reset_hd
*        �������ܣ�Ӳ��λӲ��(ATAPI)
*    ����8��
*        �������ƣ�get_hd_power_status
*        �������ܣ���ȡӲ�̵�Դ��״̬
*    ����9��
*        �������ƣ�set_idle_switch_standby
*        �������ܣ�����Ӳ�̵�״̬�Զ��л�(����-����)
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
*        �������ܣ���ʱ��ѯӲ����Ϣ(mainMenuDlg.c�ڶ�ʱ���е���)
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
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <linux/types.h>
#include <linux/hdreg.h>

#include "hdPowerManage.h"

static HD_POWER_MANAGE_INFO g_hd_power_manage_info;

void *hd_power_manage_proc();

/******************************************************************************
* �������ƣ�set_hd_APM
* ��������������Ӳ�̸߼���Դ����
* ���������const char *hd_name	Ӳ�̵����֣��磺/dev/hda1

*			unsigned int level	�߼���Դ����ļ���1-255��255��ȡ���߼���Դ����
*								���ܣ�����Խ�ߣ�����Խ��
* �����������
* �� �� ֵ���ɹ���0�� ʧ�ܣ�-1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int set_hd_APM(const char *hd_name, unsigned int level)
{	
	#ifndef SETFEATURES_EN_APM
		#define SETFEATURES_EN_APM	0x05
	#endif
	#ifndef SETFEATURES_DIS_APM
		#define SETFEATURES_DIS_APM 0x85
	#endif
	
	int hd_fd = -1;
	struct stat hd_stat;
	unsigned int hd_APM_level = 255;
	unsigned char param[4] = {WIN_SETFEATURES,0,0,0};
	
	// �ж�Ӳ�������Ƿ����
	if (stat(hd_name, &hd_stat))
	{
		printf("The device: %s not exist!\n", hd_name);
		return -1;
	}
	
	// ��Ӳ���豸
	hd_fd = open(hd_name, O_RDONLY|O_NONBLOCK);
	if (hd_fd == -1)
	{
		printf("Can not open the device: %s!\n", hd_name);
		return -1;
	}
	
	// Ӳ�̸߼���Դ����ļ���:1-255
	if (level < 1)
	{
		hd_APM_level = 1;
	}
	else
	{
		if (level > 255)
		{
			hd_APM_level = 255;
		}
		else
		{
			hd_APM_level = level;
		}
	}
	
	// ����Ӳ�̸߼���Դ����Ĳ���
	if (hd_APM_level == 255)
	{
		param[2] = SETFEATURES_DIS_APM;
	}
	else
	{
		param[1] = hd_APM_level;
		param[2] = SETFEATURES_EN_APM;
	}

	// ���ø߼���Դ���������Ӳ����
	if (ioctl(hd_fd, HDIO_DRIVE_CMD, &param))
	{
		printf("Can not set APM parameter to hard disk(%s).\n", hd_name);
		close(hd_fd);
		
		return -1;
	}
	else
	{
		close(hd_fd);
		
		return 0;
	}
}

/******************************************************************************
* �������ƣ�set_hd_standby
* ��������������Ӳ�̵�״̬Ϊ����״̬
* ���������const char *hd_name	Ӳ�̵����֣��磺/dev/hda1
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int set_hd_standby(const char *hd_name)
{
	#ifndef WIN_STANDBYNOW1
		#define WIN_STANDBYNOW1 0xE0
	#endif
	#ifndef WIN_STANDBYNOW2
		#define WIN_STANDBYNOW2 0x94
	#endif

	int hd_fd = -1;
	struct stat hd_stat;
	unsigned char param1[4] = {WIN_STANDBYNOW1,0,0,0};
	unsigned char param2[4] = {WIN_STANDBYNOW2,0,0,0};
	
	// �ж�Ӳ�������Ƿ����
	if (stat(hd_name, &hd_stat))
	{
		printf("The device: %s not exist!\n", hd_name);
		return -1;
	}
	
	// ��Ӳ���豸
	hd_fd = open(hd_name, O_RDONLY|O_NONBLOCK);
	if (hd_fd == -1)
	{
		printf("Can not open the device: %s!\n", hd_name);
		return -1;
	}
	
	// ����Ӳ�̵�״̬Ϊ����״̬
	if (ioctl(hd_fd, HDIO_DRIVE_CMD, &param1) && ioctl(hd_fd, HDIO_DRIVE_CMD, &param2))
	{
		printf("Can not set standby to hard disk(%s).\n", hd_name);

		close(hd_fd);
		
		return -1;
	}
	else
	{
		close(hd_fd);
		
		return 0;
	}
}

/******************************************************************************
* �������ƣ�set_hd_sleep
* ��������������Ӳ�̵�״̬Ϊ˯��״̬
* ���������const char *hd_name	Ӳ�̵����֣��磺/dev/hda1
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int set_hd_sleep(const char *hd_name)
{
	#ifndef WIN_SLEEPNOW1
		#define WIN_SLEEPNOW1 0xE6
	#endif
	#ifndef WIN_SLEEPNOW2
		#define WIN_SLEEPNOW2 0x99
	#endif

	int hd_fd = -1;
	struct stat hd_stat;
	unsigned char param1[4] = {WIN_SLEEPNOW1,0,0,0};
	unsigned char param2[4] = {WIN_SLEEPNOW2,0,0,0};
	
	// �ж�Ӳ�������Ƿ����
	if (stat(hd_name, &hd_stat))
	{
		printf("The device: %s not exist!\n", hd_name);
		return -1;
	}
	
	// ��Ӳ���豸
	hd_fd = open(hd_name, O_RDONLY|O_NONBLOCK);
	if (hd_fd == -1)
	{
		printf("Can not open the device: %s!\n", hd_name);
		return -1;
	}
	
	// ����Ӳ�̵�״̬Ϊ˯��״̬
	if (ioctl(hd_fd, HDIO_DRIVE_CMD, &param1) && ioctl(hd_fd, HDIO_DRIVE_CMD, &param2))
	{
		printf("Can not set sleep to hard disk(%s).\n", hd_name);

		close(hd_fd);
		
		return -1;
	}
	else
	{
		close(hd_fd);
		
		return 0;
	}
}

/******************************************************************************
* �������ƣ�set_hd_idle
* ��������������Ӳ�̵�״̬Ϊ����״̬
* ���������const char *hd_name		Ӳ�̵����֣��磺/dev/hda1
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int set_hd_idle(const char *hd_name)
{
	#ifndef WIN_SETIDLE1
		#define WIN_SETIDLE1 0xE3
	#endif
	#ifndef WIN_SETIDLE2
		#define WIN_SETIDLE2 0x97
	#endif

	int hd_fd = -1;
	struct stat hd_stat;
	unsigned char param1[4] = {WIN_SETIDLE1,0,0,0};
	unsigned char param2[4] = {WIN_SETIDLE2,0,0,0};
	
	// �ж�Ӳ�������Ƿ����
	if (stat(hd_name, &hd_stat))
	{
		printf("The device: %s not exist!\n", hd_name);
		return -1;
	}
	
	// ��Ӳ���豸
	hd_fd = open(hd_name, O_RDONLY|O_NONBLOCK);
	if (hd_fd == -1)
	{
		printf("Can not open the device: %s!\n", hd_name);
		return -1;
	}
	
	// ����Ӳ�̵�״̬Ϊ����״̬
	if (ioctl(hd_fd, HDIO_DRIVE_CMD, &param1) && ioctl(hd_fd, HDIO_DRIVE_CMD, &param2))
	{
		printf("Can not set idle to hard disk(%s).\n", hd_name);

		close(hd_fd);
		
		return -1;
	}
	else
	{
		close(hd_fd);
		
		return 0;
	}
}

/******************************************************************************
* �������ƣ�restore_hd_ready_force
* ����������ǿ�ƻָ�Ӳ�̾���״̬
* ���������const char *hd_name	Ӳ�̵����֣��磺/dev/hda1
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int restore_hd_ready_force(const char *hd_name)
{
	#ifndef WIN_IDLEIMMEDIATE
		#define WIN_IDLEIMMEDIATE 0xE1
	#endif

	int hd_fd = -1;
	struct stat hd_stat;
	unsigned char param[4] = {WIN_IDLEIMMEDIATE,0,0,0};
	
	// �ж�Ӳ�������Ƿ����
	if (stat(hd_name, &hd_stat))
	{
		printf("The device: %s not exist!\n", hd_name);
		return -1;
	}
	
	// ��Ӳ���豸
	hd_fd = open(hd_name, O_RDONLY|O_NONBLOCK);
	if (hd_fd == -1)
	{
		printf("Can not open the device: %s!\n", hd_name);
		return -1;
	}
	
	// ǿ�ƻָ�Ӳ�̾���״̬
	if (ioctl(hd_fd, HDIO_DRIVE_CMD, &param))
	{
		printf("Can not restore ready to hard disk(%s).\n", hd_name);

		close(hd_fd);
		
		return -1;
	}
	else
	{
		close(hd_fd);
		
		return 0;
	}
}

/******************************************************************************
* �������ƣ�soft_reset_hd
* ������������λӲ��(ATAPI)
* ���������const char *hd_name	Ӳ�̵����֣��磺/dev/hda1
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int soft_reset_hd(const char *hd_name)
{
	#ifndef WIN_DEVICE_RESET
		#define WIN_DEVICE_RESET 0x08
	#endif

	int hd_fd = -1;
	struct stat hd_stat;
	unsigned char param[4] = {WIN_DEVICE_RESET,0,0,0};
	
	// �ж�Ӳ�������Ƿ����
	if (stat(hd_name, &hd_stat))
	{
		printf("The device: %s not exist!\n", hd_name);
		return -1;
	}
	
	// ��Ӳ���豸
	hd_fd = open(hd_name, O_RDONLY|O_NONBLOCK);
	if (hd_fd == -1)
	{
		printf("Can not open the device: %s!\n", hd_name);
		return -1;
	}
	
	// ��λӲ��
	if (ioctl(hd_fd, HDIO_DRIVE_CMD, &param))
	{
		printf("Can not soft reset to hard disk(%s).\n", hd_name);

		close(hd_fd);
		
		return -1;
	}
	else
	{
		close(hd_fd);
		
		return 0;
	}
}

/******************************************************************************
* �������ƣ�hard_reset_hd
* ����������Ӳ��λӲ��(ATAPI)
* ���������const char *hd_name	Ӳ�̵����֣��磺/dev/hda1
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int hard_reset_hd(const char *hd_name)
{
	#ifndef HDIO_DRIVE_RESET
		#define HDIO_DRIVE_RESET 0x031c
	#endif

	int hd_fd = -1;
	struct stat hd_stat;
	
	// �ж�Ӳ�������Ƿ����
	if (stat(hd_name, &hd_stat))
	{
		printf("The device: %s not exist!\n", hd_name);
		return -1;
	}
	
	// ��Ӳ���豸
	hd_fd = open(hd_name, O_RDONLY|O_NONBLOCK);
	if (hd_fd == -1)
	{
		printf("Can not open the device: %s!\n", hd_name);
		return -1;
	}
	
	// Ӳ��λӲ��
	if (ioctl(hd_fd, HDIO_DRIVE_RESET, NULL))
	{
		printf("Can not hard reset to hard disk(%s).\n", hd_name);

		close(hd_fd);
		
		return -1;
	}
	else
	{
		close(hd_fd);
		
		return 0;
	}
}

/******************************************************************************
* �������ƣ�get_hd_power_status
* ������������ȡӲ�̵�Դ��״̬
* ���������const char *hd_name	Ӳ�̵����֣��磺/dev/hda1
* �����������
* �� �� ֵ��ʧ�ܣ�-1
*			�ɹ���ACTIVE	�
*				  STANDBY	����
*				  SLEEP		˯��
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_hd_power_status(const char *hd_name)
{	
	#ifndef WIN_CHECKPOWERMODE1
		#define WIN_CHECKPOWERMODE1	0xE5
	#endif
	#ifndef WIN_CHECKPOWERMODE2
		#define WIN_CHECKPOWERMODE2 0x98
	#endif
	
	int hd_fd = -1;
	struct stat hd_stat;
	//unsigned int hd_APM_level = 255;
	unsigned char param1[4] = {WIN_CHECKPOWERMODE1,0,0,0};
	unsigned char param2[4] = {WIN_CHECKPOWERMODE1,0,0,0};
	
	// �ж�Ӳ�������Ƿ����
	if (stat(hd_name, &hd_stat))
	{
		printf("The device: %s not exist!\n", hd_name);
		return -1;
	}
	
	// ��Ӳ���豸
	hd_fd = open(hd_name, O_RDONLY|O_NONBLOCK);
	if (hd_fd == -1)
	{
		printf("Can not open the device: %s!\n", hd_name);
		return -1;
	}
	
	// ��ȡӲ�̵�Դ��״̬
	if (ioctl(hd_fd, HDIO_DRIVE_CMD, &param1) && ioctl(hd_fd, HDIO_DRIVE_CMD, &param2))	
	{
		/*
		if (errno!=EIO || param1[0]!=0 || param1[1]!=0 || param2[0]!=0 || param2[1]!=0)
		{
			printf("Can not get power status for hard disk (%s).\n", hd_name);	
			return -1;
		}
		else
		{
			return SLEEP;
		}
		*/

		close(hd_fd);
		
		return SLEEP;		
	}
	else
	{
		close(hd_fd);
		
		if (param1[2] == 255 || param2[2] == 255)
		{
			return ACTIVE;
		}
		else
		{
			return STANDBY;
		}
	}
}

/******************************************************************************
* �������ƣ�set_idle_switch_standby
* ��������������Ӳ�̵�״̬�Զ��л�(����-����)
* ���������const char *hd_name		Ӳ�̵����֣��磺/dev/hda1
*			unsigned char timeout	���г�ʱʱ��
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int set_idle_switch_standby(const char *hd_name, unsigned char timeout)
{
	#ifndef WIN_SETIDLE1
		#define WIN_SETIDLE1 0xE3
	#endif
	#ifndef WIN_SETIDLE2
		#define WIN_SETIDLE2 0x97
	#endif

	int hd_fd = -1;
	struct stat hd_stat;
	unsigned char param1[4] = {WIN_SETIDLE1,timeout,0,0};
	unsigned char param2[4] = {WIN_SETIDLE2,timeout,0,0};
	
	// �ж�Ӳ�������Ƿ����
	if (stat(hd_name, &hd_stat))
	{
		printf("The device: %s not exist!\n", hd_name);
		return -1;
	}
	
	// ��Ӳ���豸
	hd_fd = open(hd_name, O_RDONLY|O_NONBLOCK);
	if (hd_fd == -1)
	{
		printf("Can not open the device: %s!\n", hd_name);
		return -1;
	}

	// ����Ӳ�̵�״̬Ϊ����״̬
	if (ioctl(hd_fd, HDIO_DRIVE_CMD, &param1) && ioctl(hd_fd, HDIO_DRIVE_CMD, &param2))
	//if (ioctl(hd_fd, HDIO_DRIVE_CMD, &param1))
	{
		printf("Can not set standby timeout to hard disk(%s).\n", hd_name);

		close(hd_fd);
		
		return -1;
	}
	else
	{
		close(hd_fd);
		
		return 0;
	}
}

/******************************************************************************
* �������ƣ�get_hd_num
* ������������ȡϵͳ�е�Ӳ�̸���
* �����������
* �����������
* �� �� ֵ��0-8,Ӳ�̸���
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
unsigned int get_hd_num()
{
	int i;
	int fd;
	int hd_num = 0;
	char hd_name[32];

	for (i = 0; i <MAX_DISK_NUM; i++)
	{
		sprintf(hd_name, "/dev/hd%c", 'a'+i);
         
		if ((fd = open(hd_name, O_RDWR)) >0)   
		{
			hd_num++;

			close(fd);
		}	          
	}
    
	return hd_num;
}

/******************************************************************************
* �������ƣ�init_all_hd_power_status
* ����������ϵͳӲ�̵�Դ�����ʼ��
* �����������
* �����������
* �� �� ֵ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
void init_all_hd_power_status()
{
	int fd;
	unsigned int hd_num = 0;
	char hd_name[32];
	int cur_hd_no = -1;
	int i;
	int status;

	//cur_hd_no = 0;
	//hd_num = 2;
	cur_hd_no = get_cur_disk_no();
	hd_num = get_hard_disk_num();

	memset(&g_hd_power_manage_info, 0, sizeof(HD_POWER_MANAGE_INFO));
	g_hd_power_manage_info.cur_active_disk_no = -1;
	g_hd_power_manage_info.prev_active_disk_no = -1;
	g_hd_power_manage_info.idle_disk_num = 0;
	g_hd_power_manage_info.active_disk_num = 1;
	g_hd_power_manage_info.standby_disk_num = 0;
	g_hd_power_manage_info.hd_number = hd_num;

	hd_num = 0;
	
	for (i=0; i<MAX_DISK_NUM; i++)
	{
		sprintf(hd_name, "/dev/hd%c", 'a'+i);		
		
		//�ж�Ӳ���Ƿ����
		if ((fd = open(hd_name, O_RDWR)) >0) 
		{
			//������Ӳ�̲���֮ǰ,��Ӳ��λӲ��,����������û�ʧ��
			printf("init_all_hd_power_status(): %s hard reset.\n", hd_name);
			hard_reset_hd(hd_name);

			g_hd_power_manage_info.hd_power_info[hd_num].hd_no = hd_num;
			strcpy(g_hd_power_manage_info.hd_power_info[hd_num].hd_name, hd_name);

			//����Ӳ���Զ����ߵ�ʱ��
			if (set_idle_switch_standby(hd_name, STANDBY_TIMEOUT))
			{
				printf("Disable hard disk(%s) standby feature.\n", hd_name);
				g_hd_power_manage_info.hd_power_info[hd_num].standby_timeout = 0;
			}
			else
			{
				printf("Set hard disk(%s) standby timeout: %d s\n", hd_name, STANDBY_TIMEOUT*5);
				g_hd_power_manage_info.hd_power_info[hd_num].standby_timeout = STANDBY_TIMEOUT*5;
			}

			/*	
			//����Ӳ�̸߼���Դ����
			if (set_hd_APM(hd_name, APM_LEVEL))
			{
				g_hd_power_manage_info.hd_power_info[hd_num].APM_flag = 0;
				g_hd_power_manage_info.hd_power_info[hd_num].APM_level = 255;
			}
			else
			{
				g_hd_power_manage_info.hd_power_info[hd_num].APM_flag = 1;
				g_hd_power_manage_info.hd_power_info[hd_num].APM_level = APM_LEVEL;
			}
			*/
			
			//��������Ӳ�̵ĵ�Դ״̬
			if (i == cur_hd_no)
			{
				//hard_reset_hd(hd_name);
				g_hd_power_manage_info.cur_active_disk_no = hd_num;
			}
			else
			{
				//�ǵ�ǰ�̶����ó����ߵ�״̬
				//if (set_hd_sleep(hd_name))
				if (set_hd_standby(hd_name))
				{
					status = get_hd_power_status(hd_name);
					if (status == -1)
					{
						g_hd_power_manage_info.hd_power_info[hd_num].cur_power_status = SLEEP;
						g_hd_power_manage_info.sleep_disk_num++;
					}
					else
					{						
						switch (status)
						{
						case ACTIVE:
							g_hd_power_manage_info.active_disk_num++;
							g_hd_power_manage_info.hd_power_info[hd_num].cur_power_status = ACTIVE;
							break;

						case STANDBY:
							g_hd_power_manage_info.standby_disk_num++;
							g_hd_power_manage_info.hd_power_info[hd_num].cur_power_status = STANDBY;
							break;

						case IDLE:
							g_hd_power_manage_info.idle_disk_num++;
							g_hd_power_manage_info.hd_power_info[hd_num].cur_power_status = IDLE;
							break;

						default:
							g_hd_power_manage_info.sleep_disk_num++;
							g_hd_power_manage_info.hd_power_info[hd_num].cur_power_status = SLEEP;
							break;
						}
					}
				}
				else
				{
					g_hd_power_manage_info.hd_power_info[hd_num].cur_power_status = SLEEP;
					g_hd_power_manage_info.sleep_disk_num++;
				}
			}			
			
			memset(g_hd_power_manage_info.hd_power_info[hd_num].reserved, 0, 8*sizeof(unsigned int));

			hd_num++;
		}	
		
	}	
}

/******************************************************************************
* �������ƣ�start_hd_power_manage
* ��������������Ӳ�̵�Դ����(�߳�)
* �����������
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int start_hd_power_manage()
{
	pthread_t hd_power_manage_thread;
	pthread_attr_t hd_power_manage_thread_attr;
	
	printf("Start_hd_power_manage....\n");
	printf("Init all hard disk power control.....\n");
	
	init_all_hd_power_status();
	
	if (pthread_attr_init(&hd_power_manage_thread_attr) == 0)
	{		
		pthread_attr_setschedpolicy(&hd_power_manage_thread_attr, HD_POWER_MANAGE_PRIORITY);
	}
	
  	if (pthread_create(&hd_power_manage_thread, &hd_power_manage_thread_attr, hd_power_manage_proc, NULL) == 0)
	{
		printf("Hard disk power manage server: start.\n");
		pthread_attr_destroy(&hd_power_manage_thread_attr);
		return 0;
	}
	else
	{
		printf("Hard disk power manage server: stop.\n");
		pthread_attr_destroy(&hd_power_manage_thread_attr);
		return -1;
	}
	
}	

void hd_pre_ative(int hd_no)
{
	if (hd_no >= 0 && hd_no <= 7)
	{
		g_hd_power_manage_info.prev_active_disk_no = hd_no;
	}
}

int get_pre_ative_hd_no()
{
	return g_hd_power_manage_info.prev_active_disk_no;
}

/******************************************************************************
* �������ƣ�hd_power_manage_proc
* ����������Ӳ�̵�Դ������
* �����������
* �����������
* �� �� ֵ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
void *hd_power_manage_proc()
{
	int i;
	int hd_pre_active_flag = 0;
	int old_hd_no = -1;
	int hd_status;
	int cur_hd_no = -1;
	

	while (1)
	{
		//��ȡϵͳ������Ӳ�̵�Դ״̬��Ϣ
		for (i=0; i<g_hd_power_manage_info.hd_number; i++)
		{
			hd_status = get_hd_power_status(g_hd_power_manage_info.hd_power_info[i].hd_name);
			if (hd_status>=ACTIVE && hd_status<=SLEEP)
			{
				g_hd_power_manage_info.hd_power_info[i].cur_power_status = hd_status;
				printf("Get hard disk(%s) power status: %d\n", g_hd_power_manage_info.hd_power_info[i].hd_name,
						 hd_status);
			}
		}

		//Ԥ������һ��Ӳ��
		if (g_hd_power_manage_info.hd_number >= 1)
		{	
			if (g_hd_power_manage_info.prev_active_disk_no != -1 && hd_pre_active_flag == 0)
			{
				hd_pre_active_flag = 1;
				old_hd_no = g_hd_power_manage_info.cur_active_disk_no;
				hard_reset_hd(g_hd_power_manage_info.hd_power_info[g_hd_power_manage_info.prev_active_disk_no].hd_name);		
				printf("Active next hard disk: %s.\n",	
						g_hd_power_manage_info.hd_power_info[g_hd_power_manage_info.prev_active_disk_no].hd_name);
			}

			if (hd_pre_active_flag == 1)
			{
				//cur_hd_no = get_cur_disk_no();
				if (cur_hd_no == g_hd_power_manage_info.prev_active_disk_no)
				{
					//set_hd_sleep(g_hd_power_manage_info.hd_power_info[old_hd_no].hd_name);
					set_hd_standby(g_hd_power_manage_info.hd_power_info[old_hd_no].hd_name);
					g_hd_power_manage_info.cur_active_disk_no = cur_hd_no;
					g_hd_power_manage_info.prev_active_disk_no = -1;
					hd_pre_active_flag = 0;
				}
				
				//Ԥ���ٴ�����
				if (g_hd_power_manage_info.hd_power_info[i].cur_power_status == STANDBY)
				{
					hard_reset_hd(g_hd_power_manage_info.hd_power_info[g_hd_power_manage_info.prev_active_disk_no].hd_name);
				}
			}			
		}
		
		sleep(60);
	}
}




