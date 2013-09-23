/* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�hardDisk.h
* �ļ�˵�������ļ������˲���Ӳ�̵ĺ�������
*           ������
*           1��FDISK_INFO���ݽṹ�Ķ���
*			2. PARTITION_INFO���ݽṹ�Ķ���
*			3. DISK_INFO���ݽṹ�Ķ���
*			4. HARD_DISK_INFO���ݽṹ�Ķ���
*           5������Ӳ�̵ĺ�������
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

#ifndef __HARD_DISK_H_
#define __HARD_DISK_H_

#define KILOBYTE			1024	//K  1024
#define MILOBYTE			1048576	//M  1024 * 1024

#define MAX_DISK_NUM		8
#define MAX_PARTITION_NUM	2

typedef struct __FDISK_INFO
{
	int type;						//0: fdisk  1:format
	int disk_no;
	int partition_no;
	int data_partition;
	int backup_partition;
}FDISK_INFO,*PFDISK_INFO;

// ϵͳ����Ӳ�̼�������Ϣ
typedef struct __PARTITION_INFO
{
	unsigned long mount_flag;		// MOUNT��־
	unsigned long format_flag;		// �Ƿ��Ѿ���ʽ��0:no 1:yes
	unsigned long total_size;		// ������������λΪM
	unsigned long used_size;		// ������������λΪM
	unsigned long availabe_size;	// ������������λΪM
	unsigned long used_parent;		//
	unsigned long type;   			// 1: ���ݷ��� 2: ���ݷ��� 3: ����
}PARTITION_INFO,*PPARTITION_INFO;

typedef struct __DISK_INFO
{
	unsigned long nPartitionNum;	// ��������add the code by lvjh, 2008-04-26
	PARTITION_INFO	partition_info[MAX_PARTITION_NUM];
}DISK_INFO,*PDISK_INFO;

typedef struct __HARD_DISK_INFO
{
	int cur_disk_no; 				// ����ϵͳ���8��Ӳ�� :0-7
	int cur_partition_no; 			// һ��Ӳ�����2������: 0-1
	int hard_disk_num; 				// ϵͳ�ܹ���ЧӲ����
	DISK_INFO disk_info[MAX_DISK_NUM];
}HARD_DISK_INFO,*PHARD_DISK_INFO;

int set_cur_disk_no(int disk_no);
int set_cur_partition_no(int partition_no);
int get_cur_disk_no();
int get_cur_partition_no();
int get_hard_disk_num();
int hd_get_mount_flag(int disk_no,int partition_no);
int hd_get_disk_total_size();
int hd_get_disk_used_size();
int hd_get_disk_available_size();
int hd_get_partition_total_size(int disk_no,int partition_no);
int hd_get_partition_used_size(int disk_no,int partition_no);
int hd_get_partition_available_size(int disk_no,int partition_no);

/* ·����ʽ /record/hd##/##/... */
/* ϵͳ�ܹ����8��Ӳ��,ÿ��Ӳ�����2������(������/������) */
int hd_mount_partition(char *source,char *target);
int hd_umount_partition(char *target);

/* mount ����Ӳ�̵����з��� */
int hd_mount_all_partition();
/* umount ����Ӳ�̵����з��� */
int hd_umount_all_partition();

int hd_fdisk(int disk_no,int data_partition,int backup_partition);
int hd_format(int disk_no,int partition_no);				// partition_no = 0, 1 cyg
int hd_cur_partition_full(int disk_no,int partition_no);
int hd_cur_partition_full(int disk_no,int partition_no);
int hd_find_empty_partition(int *disk_no,int *partition_no);
int hd_query_disk_info();
int hd_get_backup_partition_num(); 							// ��ñ��ݷ�����(��mount�ɹ��ı�����)
int hd_get_backup_disk_no(int backup_partition_no); 		// ͨ�����ݷ���������ñ���Ӳ�̺� 
int hd_get_disk_formated(int disk_no, int partition_no);	// ���Ӳ���Ƿ��Ѿ���ʽ��0:no   1:yes
int get_hard_disk_info(HARD_DISK_INFO *hard_disk_info); 	// �������Ӳ�̵���Ϣ

int get_fdisk_format_process();								// ��÷�����ʽ�������Ľ���
int get_fdisk_format_status();								// ��÷�����ʽ�������Ľ��

//�����mountĿ¼��ʣ��ռ�
//	ex. get_disk_avail_size("/"), get_disk_avail_size("/usb1"), get_disk_avail_size("/record/hd00/00")
//����ֵ: >= 0   �ռ��С,MΪ��λ
//����:   < 0    mount_point�����ڵ�
int get_disk_avail_size(char *mount_point);

int hd_query_disk_info_ext();

#endif


