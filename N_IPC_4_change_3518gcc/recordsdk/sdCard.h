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

#ifndef __SD_CARD_H_
#define __SD_CARD_H_

typedef struct
{
	unsigned int formated;			// �Ƿ��Ѿ���ʽ��0:no 1:yes
	unsigned int mount_flag;		// MOUNT��־
	unsigned int rdwr_flag;			// ��д��־��0���ɶ���д��1��ֻ��
	unsigned int total_size;		// ������������λΪM
	unsigned int used_size;			// ������������λΪM
	unsigned int availabe_size;		// ������������λΪM
	unsigned int used_parent;

}SD_CARD_INFO, *PSD_CARD_INFO;

int sd_get_mount_flag();
int sd_get_total_size();
int sd_get_used_size();
int sd_get_available_size();

int sd_mount();
int sd_umount();

int sd_fdisk();
int sd_format();
int sd_get_info(SD_CARD_INFO *sd_card_info);
int sd_get_formated();

int sd_get_fdisk_format_process();
int sd_get_fdisk_format_status();

int sd_query_info();

#endif


