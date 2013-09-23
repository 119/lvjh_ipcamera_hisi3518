/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�fileFormat.h
* �ļ�˵�������ļ�������¼���ļ���ʽ
* ��    �ߣ�ׯ�ݱ�
* �汾��Ϣ��V1.0
* �������: 2007-01-29
* �޸ļ�¼:
*   �޸�1      ��    ��:
*              ��    ��:
*              �޸�����:
*   �޸�:2     ��    ��:
*              ��    ��:
*              �޸�����:
* ����˵��: ��
******************************************************************************/

#ifndef __FILE_FORMAT_H_
#define __FILE_FORMAT_H_

#include "baseType.h"

#define PACK_SIZE			1024*4

#define TDSFLAG				0x53564448
#define INDEXFLAG			0xDDDDDDDD

//��Ƶ��������
#define ENCODE_VIDEO_DIVX	0x58564944
#define ENCODE_VIDEO_XVID	0x44495658
#define ENCODE_VIDEO_HISI	0x49534948
#define ENCODE_VIDEO_H264	0x34363248


//��Ƶ��������
#define ENCODE_AUDIO_MP3	0x55
#define ENCODE_AUDIO_G722 	0x65
#define ENCODE_AUDIO_G711	0x6

//#define ENCODE_AUDIO_G711	0x25
#define ENCODE_AUDIO_G726	0x45

#define HDVSF_HASINDEX	0x01
#define HDVSF_HASVIDEO	0x02
#define HDVSF_HASAUDIO	0x04

typedef struct
{
	unsigned long  nTimeTick;
	unsigned long  nVideoSize;
	unsigned short nAudioSize;
	unsigned short nImgWidth;
	unsigned short nImgHeight;
	unsigned short nKeyFrame;
}FRAME_HEADER, *PFRAME_HEADER;

typedef struct
{
	WORD	nYear;
	BYTE	nMonth;
	BYTE	nDay;
	WORD nHour;
	BYTE	nMinute;
	BYTE	nSecond;
}TDS_FILETIME, *PTDS_FILETIME;

typedef struct
{
	unsigned long	nVideoEncType;		//��Ƶ��������
	unsigned long	nImageWidth;		//��Ƶ���
	unsigned long	nImageHeight;		//��Ƶ�߶�
	unsigned long	nVideoBitRate;		//��Ƶ������
	unsigned long	nFrameRate;			//��Ƶ֡��
	unsigned long	nAudioEncType;		//��Ƶ�������� 0x55-MP3 0x25-G711 0x65-722 0x64-G726
	unsigned long	nAudioChannels;		//��Ƶͨ����
	unsigned long	nAudioSamples;		//��Ƶ������
	unsigned long	nAudioBitRate;		//��Ƶ������
	unsigned long	nReserve;			//����������Ϊ0

}AV_FORMAT, *PAV_FORMAT;

/*
typedef struct
{
	unsigned long	nVideoEncType;
	unsigned long	nImageWidth;
	unsigned long	nImageHeight;
	unsigned long	nAudioEncType;
	unsigned long	nAudioChannels;
	unsigned long	nAudioSamples;
	unsigned long nAudioBits;
}AV_FORMAT, *PAV_FORMAT;
*/

typedef struct
{
	DWORD	File_Flag;
	DWORD	File_Size;
	TDS_FILETIME	Creation_Date;
	DWORD	Play_Duration;		//����ʱ�䣬���뵥λ��
	DWORD	BeginTimeTick;
	DWORD	EndTimeTick;
	DWORD	File_Type;			//�ļ�����

	DWORD Index_Position;
	AV_FORMAT AV_Format;
	char Title[32];
	BYTE Reserved[32];	
}TDS_FILEHEADER, *PTDS_FILEHEADER;

typedef struct
{
	DWORD Index_Flag;		//=0xDDDDDDDD
	DWORD Index_Count;
}TDS_INDEXOBJECT, *PTDS_INDEXOBJECT;

typedef struct
{
	DWORD Index_TimeTick;
	DWORD Index_Offset;
}TDS_INDEXENTRIES, *PTDS_INDEXENTRIES;

typedef struct
{
	unsigned char bIsDataHead;
	unsigned char nChannel;
	unsigned short nBufSize;
	unsigned long bPacketID;
	FRAME_HEADER FrameHeader;
	unsigned char PackData[PACK_SIZE];
}DATA_PACKET, *PDATA_PACKET;

#endif

