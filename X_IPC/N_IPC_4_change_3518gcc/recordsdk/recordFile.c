#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <semaphore.h>

#include "global.h"
#include "fileFormat.h"
#include "mp4File.h"
#include "util.h"
#include "indexFile.h"
#include "indexFileExt.h"
#include "hardDisk.h"

#include "recordFile.h"

//#define FLUSH_DISK		1	//�Ƿ�����ˢд��Ӳ��<0:����   1:����д>

// ¼�����Ƶ����
typedef struct 
{
	unsigned long nEncMode;
	unsigned long nWidth;
	unsigned long nHeight;
	unsigned long nBitRate;
	unsigned long nFrameRate;
	unsigned long nReserve;
}RECORD_VIDEO_PARAM;

// ¼�����Ƶ����
typedef struct 
{
	unsigned long nEncMode;
	unsigned long nSampleRate;
	unsigned long nChannel;
	unsigned long nBitRate;
	unsigned long nReserve;
}RECORD_AUDIO_PARAM;

// ͨ��¼��
typedef struct
{
	unsigned char *pBuffer;
	unsigned long nRecordFlag;
	unsigned long nRecordType;
	unsigned long nPreRecordFlag;
	unsigned long nRecordStatus;
	pthread_mutex_t mutex;
	unsigned long reserve;	
}RECORD_CHANNEL, *PRECORD_CHANNEL;

//ȫ�ֱ�������
static MP4_RECORD_INSTANCE *g_record_instance[MAX_AV_CHANNEL];
static FILE *g_record_fd[MAX_AV_CHANNEL];
static sem_t g_record_sem[MAX_AV_CHANNEL];
static sem_t g_record_sem2[MAX_AV_CHANNEL];
static long g_record_run_flag[MAX_AV_CHANNEL];
static long g_record_pause_flag[MAX_AV_CHANNEL];

static RECORD_VIDEO_PARAM g_record_video_param[MAX_AV_CHANNEL];
static RECORD_AUDIO_PARAM g_record_audio_param[MAX_AV_CHANNEL];
static RECORD_CHANNEL g_record_channel[MAX_AV_CHANNEL];
static char g_record_index_name[MAX_AV_CHANNEL][MAX_PATH];
static int g_cur_minute[MAX_AV_CHANNEL];
char g_cur_record_file[MAX_AV_CHANNEL][64];

// ��������
int set_record_video_param(int channel, int mode, int width, int height, int bitrate, int framerate)
{
	if (channel<0 || channel>get_av_channel_num())
	{
		return -1;
	}
	
	g_record_video_param[channel].nEncMode = mode;
	g_record_video_param[channel].nWidth = width;
	g_record_video_param[channel].nHeight = height;
	g_record_video_param[channel].nBitRate = bitrate;
	g_record_video_param[channel].nFrameRate = framerate;
	
	//printf("set_record_video_param: %d %d %d %d %d\n", mode, width, height, bitrate, framerate);
	
	return 0;
}

int set_record_audio_param(int channel, int mode, int channels, int rate, int bits)
{
	if (channel<0 || channel>get_av_channel_num())
	{
		return -1;
	}
	
	g_record_audio_param[channel].nEncMode = mode;
	g_record_audio_param[channel].nChannel = channels;
	g_record_audio_param[channel].nSampleRate = rate;
	g_record_audio_param[channel].nBitRate = bits;

	//printf("set_record_audio_param: %x %d %d %d\n", mode, channels, rate, bits);
	
	return 0;
}

int send_one_frame_to_recorder(int channel, unsigned char *buffer, int size)
{
	int ret = -1;
	int value = 0;
	FRAME_HEADER *pFrameHead = NULL;

	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}
	if (buffer == NULL)
	{
		return -1;
	}
	if (size<=0 || size>=MAX_FRAME_SIZE)
	{
		return -1;
	}

	// û��¼��
	if (!g_record_channel[channel].nRecordFlag)
	{
		return -1;
	}
	
	// Add the code by lvjh, 2008-08-27
	//if (g_record_pause_flag[channel]==1 || g_record_run_flag[channel]==0)
	if (g_record_run_flag[channel]==0)
	{
		return -1;	
	}
	
	// Add the code by lvjh, 2008-04-26
	sem_wait(&g_record_sem2[channel]);
	
	pthread_mutex_lock(&g_record_channel[channel].mutex);

	if (g_record_channel[channel].pBuffer)
	{
		memcpy(g_record_channel[channel].pBuffer, buffer, size);
	}
	else
	{
		pthread_mutex_unlock(&g_record_channel[channel].mutex);

		return -1;
	}

	// ����������־
	pFrameHead = (FRAME_HEADER *)g_record_channel[channel].pBuffer;
	pFrameHead->nKeyFrame |= 0x10;

	pthread_mutex_unlock(&g_record_channel[channel].mutex);

	sem_post(&g_record_sem[channel]);

	//printf("send_one_frame_to_recorder(%d)(%d %p %d %d %x)\n", size, channel, pFrameHead->nTimeTick, pFrameHead->nVideoSize, pFrameHead->nAudioSize, pFrameHead->nKeyFrame);

	return 0;
}

/******************************************************************************
* �������ƣ�get_channel_record_type
* ������������ȡָ��ͨ����¼������
* ���������int channel ָ��ͨ����
* �����������
* �� �� ֵ���ɹ�: ��0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_channel_record_type(int channel)
{
	unsigned long type = 0;

	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}

	pthread_mutex_lock(&g_record_channel[channel].mutex);

	type = g_record_channel[channel].nRecordType;

	pthread_mutex_unlock(&g_record_channel[channel].mutex);

	return type;
}

/******************************************************************************
* �������ƣ�get_record_file_name
* ������������ȡָ��ͨ����¼���ļ����ļ���
* ���������int channel		ָ��ͨ����
* ���������char *file_name ¼���ļ���
* �� �� ֵ��0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_record_file_name(int channel, char *file_name)
{
	int year,month,day,hour,minute,second;
	int cur_disk_no;
	int cur_partition_no;

	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}
	if (file_name == NULL)
	{
		return -1;
	}

	get_system_time(&year, &month, &day, &hour, &minute, &second);

#ifdef SD_STORAGE
	if (sd_get_mount_flag() != 1)
	{
		return -1;
	}
	if (sd_get_readwrite_flag() != 1)
	{
		return -1;
	}

	cur_disk_no = 0;
	cur_partition_no = 0;
#endif

#ifdef HD_STORAGE
	cur_disk_no = get_cur_disk_no();
	cur_partition_no = get_cur_partition_no();
#endif

	// ����¼���ļ���
	sprintf(file_name, "/record/hd%02d/%02d/ch%02d", cur_disk_no, cur_partition_no, channel);
	mkdir(file_name, 0x777);
	sprintf(file_name, "%s/%04d-%02d-%02d", file_name, year, month, day);
	mkdir(file_name, 0x777);
	sprintf(file_name,"%s/%02d", file_name, hour);
	mkdir(file_name, 0x777);
	sprintf(file_name, "%s/%02d-%02d-%02d.tds", file_name, hour, minute, second);

	// Ϊ���ļ��Ŀ��ٲ�ѯ,�ش��������ļ���
	sprintf(g_record_index_name[channel], "/record/hd%02d/%02d/ch%02d/%04d-%02d-%02d/%02d/index.db",
			cur_disk_no, cur_partition_no, channel, year, month, day, hour);

	g_cur_minute[channel] = get_minute();

	//printf("get_record_file_name: %s\n", file_name);

	return 0;
}

/******************************************************************************
* �������ƣ�get_snapshot_file_name
* ������������ȡָ��ͨ����¼���ļ����ļ���
* ���������int channel		ָ��ͨ����
* ���������char *file_name ¼���ļ���
* �� �� ֵ��0
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int get_snapshot_file_name(int channel, char *file_name)
{
	int year,month,day,hour,minute,second;
	int cur_disk_no;
	int cur_partition_no;

	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}
	if (file_name == NULL)
	{
		return -1;
	}

	get_system_time(&year, &month, &day, &hour, &minute, &second);

#ifdef SD_STORAGE
	if (sd_get_mount_flag() != 1)
	{
		return -1;
	}
	if (sd_get_readwrite_flag() != 1)
	{
		return -1;
	}

	cur_disk_no = 0;
	cur_partition_no = 0;
#else
	cur_disk_no = get_cur_disk_no();
	cur_partition_no = get_cur_partition_no();
#endif

	// ����¼���ļ���
	sprintf(file_name, "/record/hd%02d/%02d/ch%02d", cur_disk_no, cur_partition_no, channel);
	mkdir(file_name, 0x777);
	sprintf(file_name, "%s/%04d-%02d-%02d", file_name, year, month, day);
	mkdir(file_name, 0x777);
	sprintf(file_name,"%s/%02d", file_name, hour);
	mkdir(file_name, 0x777);
	sprintf(file_name,"%s/snapshot", file_name);
	mkdir(file_name, 0x777);
	sprintf(file_name, "%s/%02d-%02d-%02d.jpeg", file_name, hour, minute, second);

	return 0;
}

int get_snapshot_index_name(int channel, char *file_name)
{
	int year,month,day,hour,minute,second;
	int cur_disk_no;
	int cur_partition_no;

	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}
	if (file_name == NULL)
	{
		return -1;
	}

#ifdef SD_STORAGE
	if (sd_get_mount_flag() != 1)
	{
		return -1;
	}
	if (sd_get_readwrite_flag() != 1)
	{
		return -1;
	}

	cur_disk_no = 0;
	cur_partition_no = 0;
#else
	cur_disk_no = get_cur_disk_no();
	cur_partition_no = get_cur_partition_no();
#endif

	get_system_time(&year, &month, &day, &hour, &minute, &second);

	// ����¼���ļ���
	sprintf(file_name, "/record/hd%02d/%02d/ch%02d", cur_disk_no, cur_partition_no, channel);
	mkdir(file_name, 0x777);
	sprintf(file_name, "%s/%04d-%02d-%02d", file_name, year, month, day);
	mkdir(file_name, 0x777);
	sprintf(file_name,"%s/%02d", file_name, hour);
	mkdir(file_name, 0x777);
	sprintf(file_name,"%s/snapshot", file_name);
	mkdir(file_name, 0x777);
	sprintf(file_name, "%s/index.db", file_name);

	return 0;
}

/******************************************************************************
* �������ƣ�pause_record_file
* ������������ͣ¼��
* �����������
* �����������
* �� �� ֵ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int record_channel_pause(int channel)
{
	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}

	g_record_pause_flag[channel] = 1;

	return 0;
}

/******************************************************************************
* �������ƣ�record_resume
* �������������¿�ʼ¼��
* �����������
* �����������
* �� �� ֵ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int record_channel_resume(int channel)
{
	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}

	g_record_pause_flag[channel] = 0;

	// Add the code by lvjh, 2008-05-05
	//sem_post(&g_record_sem2[channel]);

	return 0;
}

/******************************************************************************
* �������ƣ�write_record_file
* ����������д¼���ļ��Ļص�����
* ���������WORD channel	ָ��ͨ����
*			LPVOID pBuffer	¼�����ݻ�����
*			DWORD size		¼�����ݵĴ�С
*			BYTE flag		¼��ı�־��0-д¼�����ݣ�1-��¼���ļ���2-�ر�¼���ļ�
* �����������
* �� �� ֵ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int write_record_file(int channel, void *pBuffer, int size, int flag)
{
	char file_name[MAX_PATH];
	struct timeval tv;
	unsigned long long nStartTimeCount = 0;
	unsigned long long nCurTimeCount = 0;
	
	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}

#ifdef SD_STORAGE
	if (sd_get_mount_flag() != 1)
	{
		return -1;
	}
	if (sd_get_readwrite_flag() != 1)
	{
		return -1;
	}
#endif

#ifdef HD_STORAGE
	if (get_hard_disk_num() <= 0)	//û�д洢��
	{
		return -1;
	}
#endif

	// �ļ�д����
	switch (flag)
	{
	case 0:		// д¼������
		{			
			if (g_record_fd[channel] != NULL)
			{
				fwrite(pBuffer, size, 1, g_record_fd[channel]);
				fflush(g_record_fd[channel]);
#if	FLUSH_DISK
				//sync();
#endif				
			}
		}
		break;
		
	case 1:		// ��¼���ļ�
		{
			// �ɵ��ļ�û�رգ����ȹر�
			if (g_record_fd[channel] != NULL)
			{
				fclose(g_record_fd[channel]);
				g_record_fd[channel] = NULL;
			}

			// ��ȡ�ļ���
			get_record_file_name(channel, file_name);

			// ���ļ�
			g_record_fd[channel] = fopen(file_name, "w+b");
			if (g_record_fd[channel] == NULL)
			{
				return -1;
			}
			strncpy(g_cur_record_file[channel], file_name, 64);		// Add the code by lvjh, 2008-04-29
			
			// д¼�����ݵ��ļ���
			fwrite(pBuffer, size, 1, g_record_fd[channel]);
			fflush(g_record_fd[channel]);

#if	FLUSH_DISK
			//sync();
#endif

		}
		break;
		
	case 2:		// �ر�¼���ļ�
		{
			if (g_record_fd[channel] != NULL)
			{
				REC_FILE_INFO recFileInfo;

				memset(&recFileInfo, 0, sizeof(REC_FILE_INFO));
				
				// ��λ���ļ�ͷ��д�ļ�ͷ��Ϣ
				fseek(g_record_fd[channel], 0, SEEK_SET);
				fwrite(pBuffer, size, 1, g_record_fd[channel]); 
				fflush(g_record_fd[channel]);
#if	FLUSH_DISK
				//sync();
#endif
				// �ر��ļ�
				fclose(g_record_fd[channel]);
				g_record_fd[channel] = NULL;
              
				// д¼�������ļ�
				pthread_mutex_lock(&g_record_channel[channel].mutex);
				
				//write_record_index_file(g_record_index_name[channel], g_cur_minute[channel], g_record_channel[channel].nRecordType);
			
				// Add the code by lvjh, 2008-04-29
				recFileInfo.nSize = g_record_instance[channel]->file_size;
				recFileInfo.nPlayTime = g_record_instance[channel]->play_time;
				recFileInfo.nType = g_record_channel[channel].nRecordType;
				strncpy(recFileInfo.szFileName, g_cur_record_file[channel], 64);
				write_record_index_file_ext(g_record_index_name[channel], g_cur_minute[channel], recFileInfo);

				pthread_mutex_unlock(&g_record_channel[channel].mutex);
			}
		}
		break;
		
	default:
		break;
	}

	return 0;
}

/******************************************************************************
* �������ƣ�output_file
* �������������¼���ļ�
* ���������void * context	������
*			DWORD ip		IP��ַ
*			WORD channel	ָ��ͨ����
*			LPVOID pBuffer	¼�����ݻ�����
*			DWORD size		¼�����ݵĴ�С
*			BYTE flag		¼��ı�־��0-д¼�����ݣ�1-��¼���ļ���2-�ر�¼���ļ�
* �����������
* �� �� ֵ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int record_callback(void * context, DWORD ip, WORD channel, LPVOID pBuffer, DWORD size, BYTE flag)
{
	int ret = -1;
	 
	ret = write_record_file(channel, pBuffer, size, flag);

	return ret;
}

/******************************************************************************
* �������ƣ�record_av_stream
* ������������ȡ�������Ļص�����
* ���������void* context		������
*			uint32 nIP			IP��ַ
*			uint16 nChannel		ָ��ͨ����
*			uint32 nSize		��ȡ�������Ĵ�С
*			uint8 Flag			��ȡ��������־��1��ʼ¼��2ֹͣ¼��3����¼������
* ���������void* pBuffer		��ȡ������������
* �� �� ֵ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
static int record_av_stream(void* context, uint32 nIP, uint16 nChannel, void* pBuffer, uint32 nSize, uint8 Flag)
{
	if (nChannel>=get_av_channel_num())
	{
		return -1;
	}

	//printf("record_av_stream(%d %d %d)\n", nChannel, nSize, Flag);

	switch (Flag)
	{
	case 1:		// ��ʼ¼AV����
		if (g_record_instance[nChannel] != NULL)
		{
			mp4_record_set_record_type(g_record_instance[nChannel], g_record_channel[nChannel].nRecordType);
			mp4_record_set_video_format(g_record_instance[nChannel], 
										g_record_video_param[nChannel].nEncMode,
										g_record_video_param[nChannel].nWidth, 
										g_record_video_param[nChannel].nHeight,
										g_record_video_param[nChannel].nBitRate,
										g_record_video_param[nChannel].nFrameRate);
			mp4_record_set_audio_format(g_record_instance[nChannel], 
										g_record_audio_param[nChannel].nEncMode,
										g_record_audio_param[nChannel].nChannel, 
										g_record_audio_param[nChannel].nSampleRate, 
										g_record_audio_param[nChannel].nBitRate);
			mp4_record_set_callback_fun(g_record_instance[nChannel], (void *)record_callback, context, nIP, nChannel);
			
			mp4_record_start_record(g_record_instance[nChannel], 0x03);	//����Ƶ����

			if (nSize>0 && pBuffer!=NULL)
			{
				mp4_record_send_one_frame(g_record_instance[nChannel], pBuffer, nSize);
			}
		}
		break;
		
	case 2:		// ֹͣ¼AV����
		if (g_record_instance[nChannel] != NULL)
		{
			if (nSize > 0 && pBuffer != NULL)
			{
				mp4_record_send_one_frame(g_record_instance[nChannel], pBuffer, nSize);
			}
			mp4_record_set_record_type(g_record_instance[nChannel], g_record_channel[nChannel].nRecordType);
			mp4_record_stop_record(g_record_instance[nChannel]);
		}
		break;
		
	case 3:	// ����¼AV����
		if (g_record_instance[nChannel] != NULL && nSize > 0 && pBuffer != NULL)
		{
			mp4_record_set_record_type(g_record_instance[nChannel], g_record_channel[nChannel].nRecordType);
			mp4_record_send_one_frame(g_record_instance[nChannel], pBuffer, nSize);
		}
		break;
		
	default:
		return -1;
		break;
	}

	return 0;
}

static int record_av_stream_ext(int nChannel, void* pBuffer, int nSize, int Flag)
{
	char file_name[MAX_PATH];

	//printf("record_av_stream(%d %d %d)\n", nChannel, nSize, Flag);

	if (nChannel<0 || nChannel>=get_av_channel_num())
	{
		return -1;
	}

	switch (Flag)
	{
	case 1:		// ��ʼ¼AV����
		{
			TDS_FILEHEADER file_header;
	
			// �ɵ��ļ�û�رգ����ȹر�
			if (g_record_fd[nChannel] != NULL)
			{
				fclose(g_record_fd[nChannel]);
				g_record_fd[nChannel] = NULL;
			}

			// ��ȡ�ļ���
			get_record_file_name(nChannel, file_name);

			// ���ļ�
			g_record_fd[nChannel] = fopen(file_name, "w+b");
			if (g_record_fd[nChannel] == NULL)
			{
				return -1;
			}

			if (g_record_fd[nChannel] != NULL)
			{
				// д¼�����ݵ��ļ���
				fwrite(&file_header, 1, sizeof(TDS_FILEHEADER), g_record_fd[nChannel]);
#if	FLUSH_DISK
				fflush(g_record_fd[nChannel]);
				//sync();
#endif
			}
		}
		break;
		
	case 2:		// ֹͣ¼AV����
		if (g_record_fd[nChannel] != NULL)
		{
			TDS_FILEHEADER file_header;

			// ��λ���ļ�ͷ��д�ļ�ͷ��Ϣ
			fseek(g_record_fd[nChannel], 0, SEEK_SET);
			fwrite(&file_header, 1, sizeof(TDS_FILEHEADER), g_record_fd[nChannel]); 
#if	FLUSH_DISK
			fflush(g_record_fd[nChannel]);
			//sync();
#endif
			// �ر��ļ�
			fclose(g_record_fd[nChannel]);
			g_record_fd[nChannel] = NULL;
              
			// д¼�������ļ�
			pthread_mutex_lock(&g_record_channel[nChannel].mutex);
			
			write_record_index_file(g_record_index_name[nChannel], g_cur_minute[nChannel], g_record_channel[nChannel].nRecordType);

			pthread_mutex_unlock(&g_record_channel[nChannel].mutex);
		}
		break;
		
	case 3:	// ����¼AV����
		if (nSize > 0 && pBuffer != NULL)
		{
			if (g_record_fd[nChannel] != NULL)
			{
				fwrite(pBuffer, nSize, 1, g_record_fd[nChannel]);
#if	FLUSH_DISK
				fflush(g_record_fd[nChannel]);
				//sync();
#endif				
			}
		}
		break;
		
	default:
		break;
	}
}

int print_memory(char *buffer, int size)
{
	int i = 0;

	if (buffer==NULL || size<=0)
	{
		return -1;
	}

	printf("Memory Start: %p\n", buffer);
	for (i=0; i<size; i++)
	{
		printf("%02x ", buffer[i]);
	}
	printf("\n");
	printf("\nMemory End: %p\n", buffer+size);

	return 0;
}

//#define RECORD_TEST	1

int record_thread_fun(void *param)
{
	int ret = -1;
	FRAME_HEADER *pFrameHeader = NULL;
	FRAME_HEADER FrameHeader;
	unsigned char *pDataBuffer = NULL;
	unsigned int nDataSize = 0;
	//unsigned char preBuffer[720*576*3/2];
	unsigned char *preBuffer = NULL; // Add the code by lvjh, 2008-03-12
	unsigned char *result = NULL;
	int preSize = 0;
	unsigned int nRecordFlag = 0;
	int channel = 0;
	int newDataFlag = 0;
	int nNoData = 0;

	struct timeval tv;
	unsigned long long nStartTimeCount = 0;
	unsigned long long nCurTimeCount = 0;

#ifdef RECORD_TEST
	char fileName[64];
	FILE *fp = NULL;
#endif
	
	preBuffer = (unsigned char *)malloc(400*1024);
	if (preBuffer == NULL)
	{
		printf("preBuffer malloc failed!\n");
		return -1;
	}

	// ͨ����
	channel = (int *)param;
	if (channel<0 || channel>=MAX_AV_CHANNEL)
	{
		printf("Channel Error: %d!\n", channel);
		return -1;
	}

#ifdef RECORD_TEST
	sprintf(fileName, "/record/hd00/00/ch00/%02d.tds", channel);
	fp = fopen(fileName, "w+b");
	if (fp == NULL)
	{
		printf("Can not open file: %s\n", fileName);
	}
#endif

	//printf("Channel Buffer Pointer: %p\n", g_record_channel[channel].pBuffer);

	while (g_record_run_flag[channel])
	{
		// ���¼����ͣ�����߳�����
		if (g_record_pause_flag[channel] == 1)
		{
			record_av_stream(NULL, 0, channel, NULL, 0, 2);		// Add the code by lvjh, 2008-04-28
			printf("pause record: 1\n");
			usleep(1000*1000);
			continue;
		}

		// �ȴ��ź���
		sem_wait(&g_record_sem[channel]);
		/*
		// ���¼����ͣ�����߳�����
		if (g_record_pause_flag[channel] == 1)
		{
			record_av_stream(NULL, 0, channel, NULL, 0, 2);		// Add the code by lvjh, 2008-04-28
			printf("pause record: 2\n");
			sleep(1);
			continue;
		}
		*/
		pthread_mutex_lock(&g_record_channel[channel].mutex);

		// ��ȡ֡ͷ��Ϣ
		pFrameHeader = (FRAME_HEADER *)g_record_channel[channel].pBuffer;

		// ���֡ͷ��Ϣ
		newDataFlag = pFrameHeader->nKeyFrame & 0x10;
		
		if (g_record_channel[channel].nRecordFlag)	// ���µ�¼������
		{
			pFrameHeader->nKeyFrame &= 0x0F;

			pDataBuffer = g_record_channel[channel].pBuffer;
			nDataSize = pFrameHeader->nVideoSize + pFrameHeader->nAudioSize + sizeof(FRAME_HEADER);

#ifdef RECORD_TEST
			fwrite(pDataBuffer, 1, nDataSize, fp);
			fflush(fp);
#endif

			//printf("record_thread_fun(%d %d %d %x)\n", pFrameHeader->nTimeTick, pFrameHeader->nVideoSize, pFrameHeader->nAudioSize, pFrameHeader->nKeyFrame);

			// ��ʼ¼��,Ҫ��I֡��ʼ
			nRecordFlag = g_record_channel[channel].nRecordStatus;
			if ( pFrameHeader->nKeyFrame && nRecordFlag==0x01)
			//if ( nRecordFlag==0x01)
			{
				
				// ��־¼��ͨ��������¼��
				g_record_channel[channel].nRecordStatus = 0x11;	

				record_av_stream(NULL, 0, channel, NULL, 0, 1);
				
				printf("###Start record ...\n");
			
				// �ٴ洢��ǰ¼�������
				record_av_stream(NULL, 0, channel, pDataBuffer, nDataSize, 3);
			}

			// ����¼��
			nRecordFlag = g_record_channel[channel].nRecordStatus;
			if ((nRecordFlag==0x10 && !pFrameHeader->nKeyFrame) || nRecordFlag==0x11)		// ��״̬Ϊ0x11������¼�񣬻��е�Ҫֹͣ¼�񣬵���I֡��û��ʱ������Ҫ����¼��
			{
				//printf("record continue.....\n");
				record_av_stream(NULL, 0, channel, pDataBuffer, nDataSize, 3);
			}

			// ֹͣ¼��	
			nRecordFlag = g_record_channel[channel].nRecordStatus;
			if (pFrameHeader->nKeyFrame && nRecordFlag==0x10)
			{
				printf("Stop record!!!\n");
				// ��־¼��ͨ������ֹͣ״̬

				if (g_record_channel[channel].nRecordFlag)		// Add the code by lvjh, 2008-04-28
				{
					g_record_channel[channel].nRecordStatus = 0x01;
				}
				
				record_av_stream(NULL, 0, channel, NULL, 0, 2);
			}
		}
		else// ֹͣ¼���¼������û�и���
		{
			// û��¼��Ĵ���
			if (newDataFlag == 0)
			{
				nNoData++;
				printf("Not Recording Data(%d)(%d)(%x %d)!\n", channel, nNoData, newDataFlag, g_record_channel[channel].nRecordFlag);
			}
			
			// Add the code by lvjh, 2008-04-30
			if (g_record_channel[channel].nRecordFlag == 0)
			{
				printf("Stop record!!!\n");
				record_av_stream(NULL, 0, channel, NULL, 0, 2);
			}
		}
	
		pthread_mutex_unlock(&g_record_channel[channel].mutex);
		
		// Add the code by lvjh, 2008-04-26
		sem_post(&g_record_sem2[channel]);
	}

	// ֹͣ¼���߳��˳�
	if (g_record_instance[channel])
	{
		mp4_record_release_instance(g_record_instance[channel]);
		g_record_instance[channel] = NULL;
	}

	// Add the code by lvjh, 2008-03-12
	if (preBuffer)
	{
		free(preBuffer);
		preBuffer = NULL;
	}

	pthread_exit(NULL);

	return 0;
}

int record_channel_start(int channel)
{
	int ret = -1;
	int max_priority;
	pthread_t threadID;
	pthread_attr_t init_attr;
	struct sched_param init_priority;
	pthread_mutexattr_t mutex_attr;

	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}

	// Add the code by lvjh, 2008-05-04
	g_record_channel[channel].nRecordType = 0x00;
	
	// ��ʼ���ź���
	ret = sem_init(&g_record_sem[channel], 0, 0);
	if (ret)
	{
		return -1;
	}
	// Add the code by lvjh, 2008-04-26
	ret = sem_init(&g_record_sem2[channel], 0, 1);
	if (ret)
	{
		return -1;
	}
    
	// �������ݻ�����
	//g_record_channel[channel].pBuffer = (unsigned char *)malloc(720*576*3/2);
	g_record_channel[channel].pBuffer = (unsigned char *)malloc(400*1024);
	if (g_record_channel[channel].pBuffer == NULL)
	{
		g_record_channel[channel].pBuffer = NULL;
		sem_destroy(&g_record_sem[channel]);
		return -1;
	}
    	
	// ��������������������
	pthread_mutexattr_init(&mutex_attr);
	pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&g_record_channel[channel].mutex, &mutex_attr);
	pthread_mutexattr_destroy(&mutex_attr);
   
    
	// ��ʼ��¼���̵߳�����
	max_priority = sched_get_priority_max( SCHED_FIFO );
	pthread_attr_init( &init_attr );
	pthread_attr_setschedpolicy( &init_attr, SCHED_FIFO );
	pthread_attr_getschedparam( &init_attr, &init_priority );
	init_priority.sched_priority = max_priority;
	pthread_attr_setschedparam( &init_attr, &init_priority );
	/*
	// ����Ԥ¼��ͨ��
	ret = Open_prerecord(channel, 1024*1024*1);
	if (!ret)
	{
		g_record_channel[channel].nPreRecordFlag = 1;
	}
	else
	{
		g_record_channel[channel].nPreRecordFlag = 0;
	}
	*/
	// test
	g_record_channel[channel].nPreRecordFlag = 0;
	

	// ����¼����
	g_record_instance[channel] = mp4_record_create_instance();
	if (g_record_instance[channel] == NULL)
	{
		return -1;
	}

	// ��־¼��ͨ����ʼ����	
	g_record_run_flag[channel] = 1;
	g_record_pause_flag[channel] = 0;

	// ����¼���߳�
	ret = pthread_create(&threadID, NULL, (void *)record_thread_fun,  (void *)channel);
	//ret = pthread_create(&threadID, &init_attr, (void *)record_thread_fun,  (void *)channel);	// Add the code by lvjh 2008-03-24
	if (ret != 0)
	{
		g_record_run_flag[channel] = 0;

		pthread_attr_destroy(&init_attr);
		record_channel_stop(channel);
		
		return -1;
	}
	
	pthread_attr_destroy(&init_attr);

	return	0;	
		
}

int record_channel_stop(int channel)
{
	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}
	
	// ��־¼��ͨ��ֹͣ
	g_record_run_flag[channel] = 0;

	// �����ź���
	sem_destroy(&g_record_sem[channel]);

	// ����Ԥ¼��ͨ��
	//Close_prerecord(channel);

	// �������ݻ�����
	pthread_mutex_destroy(&g_record_channel[channel].mutex);

	// �������ݻ�����
	if (g_record_channel[channel].pBuffer)
	{
		free(g_record_channel[channel].pBuffer);
	}

	// ����¼����
	if (g_record_instance[channel])
	{
		mp4_record_release_instance(g_record_instance[channel]);
		g_record_instance[channel] = NULL;
	}

	return 0;
}

/******************************************************************************
* �������ƣ�start_record
* ��������������ָ��ͨ��¼��
* ���������int channel_no		ָ��ͨ����
*			unsigned int type	¼������
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int start_record(int channel, unsigned int type)
{
	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}

#ifndef SD_STORAGE
	// �ж��Ƿ���Ӳ��
	if (get_hard_disk_num() <= 0)
	{
		printf("get_hard_disk_num(): Failed!\n");
		return -1;
	}
#endif

	if (g_record_channel[channel].nRecordFlag == 0) //û��¼��
	{		
		g_record_channel[channel].nRecordFlag = 1;
		g_record_channel[channel].nRecordStatus = 0x01;
	}

	g_record_channel[channel].nRecordType |= type;
			
	return 0;
}

/******************************************************************************
* �������ƣ�stop_record
* ����������ָֹͣ��ͨ��¼��
* ���������int channel_no		ָ��ͨ����
*			unsigned int type	¼������
* �����������
* �� �� ֵ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int stop_record(int channel, unsigned int type)
{
	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}

#ifndef SD_STORAGE
	// �ж��Ƿ���Ӳ��
	if (get_hard_disk_num() <= 0)
	{
		return -1;
	}
#endif

	if (g_record_channel[channel].nRecordFlag == 1)
	{
		//printf("Record Type: %x %x\n", g_record_channel[channel].nRecordType, type);

		g_record_channel[channel].nRecordType &= (~type);

		if (g_record_channel[channel].nRecordType == 0)
		{
			printf("Record Type: all stop\n");

			g_record_channel[channel].nRecordFlag = 0;
			g_record_channel[channel].nRecordStatus = 0x10;
			//g_record_channel[channel].nRecordStatus |= 0x10;

			record_av_stream(NULL, 0, channel, NULL, 0, 2);		// Add the code by lvjh, 2008-05-04
		}
	}

	return 0;
}

/******************************************************************************
* �������ƣ�switch_record_file
* �����������л�¼���ļ�
* �����������
* �����������
* �� �� ֵ����
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int switch_record_file(int channel)
{
	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}

	if (g_record_channel[channel].nRecordFlag == 1)
	{
		g_record_channel[channel].nRecordStatus = 0x10;
		//usleep(40);
		//g_record_channel[channel].nRecordStatus = 0x01;
	}

	return 0;
}

int get_record_status(int channel)
{
	if (channel<0 || channel>=get_av_channel_num())
	{
		return -1;
	}

	if (g_record_channel[channel].nRecordStatus)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

