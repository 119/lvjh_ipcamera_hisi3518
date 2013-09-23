/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�recordFile.h
* �ļ�˵�������ļ�������¼���ļ�����
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

#ifndef __RECORDFILE_H_
#define __RECORDFILE_H_

// ¼�����Ͷ���
#define RECORD_TYPE_TIME			0x00000001	// ��ʱ¼��
#define RECORD_TYPE_VIDEO_MOVE		0x00000002	// ��Ƶ�ƶ�
#define RECORD_TYPE_VIDEO_LOST		0x00000004	// ��Ƶ��ʧ
#define RECORD_TYPE_ALARM_INPUT		0x00000008	// ��������
#define RECORD_TYPE_NORMAL			0x00000010	// normal record

#define RECORD_TYPE_NUM				5			// ¼��������
#define MAX_CAPTION_LEN				128

//int set_record_video_param(int channel, int mode, int width, int height);
int set_record_video_param(int channel, int mode, int width, int height, int bitrate, int framerate);
int set_record_audio_param(int channel, int mode, int channels, int rate, int bits);
int send_one_frame_to_recorder(int channel, unsigned char *buffer, int size);
int get_channel_record_type(int channel);
int get_record_file_name(int channel, char *file_name);
int record_channel_pause(int channel);
int record_channel_resume(int channel);
int record_channel_start(int channel);
int record_channel_stop(int channel);
int start_record(int channel, unsigned int type);
int stop_record(int channel, unsigned int type);
int switch_record_file(int channel);
int get_record_status(int channel);

#endif
