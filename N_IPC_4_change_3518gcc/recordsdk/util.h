/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�util.h
* �ļ�˵�������ļ�������ʱ�����
* ��    �ߣ�ׯ�ݱ�
* �汾��Ϣ��V1.0
* �������: 2007-01-30
* �޸ļ�¼:
*   �޸�1      ��    ��:
*              ��    ��:
*              �޸�����:
*   �޸�:2     ��    ��:
*              ��    ��:
*              �޸�����:
* ����˵��: ��
******************************************************************************/

#ifndef __UTIL_H_
#define __UTIL_H_

int set_av_channel_num(int num);
int get_av_channel_num();

int get_year();
int get_month();
int get_day();
int get_hour();
int get_minute();
int get_second();
int get_msec();
int get_week();
int set_system_time(int year, int month, int day, int hour, int minute, int second);
int get_system_time(int *year, int *month, int *day, int *hour, int *minute, int *second);
int get_week_day(int year, int month, int day);
int get_day_num(int year, int month);
int string_to_time(char *pStr, unsigned long *start_hour, unsigned long *start_min, unsigned long *end_hour, unsigned long *end_min);

#endif


