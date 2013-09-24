#ifndef __PTZ_H_
#define __PTZ_H_

#include "com.h"

#define MAX_PTZ_NUM			80
#define MAX_PTZ_NAME		32
#define MAX_CMD_SIZE		64
#define MAX_CMD_NUM			64


typedef enum
{
	
	PTZ_LEFT            =  0,     //nTypeThird: �ٶ���(1-10)
	PTZ_RIGHT           =  1,    // nTypeThird: �ٶ���(1-10)
	PTZ_UP              =  2,    // nTypeThird: �ٶ���(1-10)
	PTZ_DOWN            =  3,    // nTypeThird: �ٶ���(1-10)
	PTZ_IRISADD         =  4,    //nTypeThird: �ٶ���(1-10)
	PTZ_IRISDEC         =  5,    // nTypeThird: �ٶ���(1-10)
	PTZ_FOCUSADD        =  6,    // nTypeThird: �ٶ���(1-10)
	PTZ_FOCUSDEC        =  7,   //  nTypeThird: �ٶ���(1-10)
	PTZ_ZOOMADD         =  8,    // nTypeThird: �ٶ���(1-10)
	PTZ_ZOOMDEC         =  9,    // nTypeThird: �ٶ���(1-10)
	PTZ_GOTOPOINT       = 10,    // nTypeThird: Ԥ�õ��(1-255)
	PTZ_SETPOINT        = 11,    // nTypeThird: Ԥ�õ��(1-255)
	PTZ_AUTO            = 12,      // nTypeThird: δ����
	PTZ_STOP            = 13,     // nTypeThird: δ����
	PTZ_LEFTSTOP        = 14,     // nTypeThird: δ����
	PTZ_RIGHTSTOP       = 15,     // nTypeThird: δ����
	PTZ_UPSTOP	        = 16,     // nTypeThird: δ����
	PTZ_DOWNSTOP        = 17,     // nTypeThird: δ����
	PTZ_IRISADDSTOP     = 18,     // nTypeThird: δ����
	PTZ_IRISDECSTOP     = 19,    //   nTypeThird: δ����
	PTZ_FOCUSADDSTOP    = 20,     // nTypeThird: δ����
	PTZ_FOCUSDECSTOP    = 21,     // nTypeThird: δ����
	PTZ_ZOOMADDSTOP     = 22,     // nTypeThird: δ����
	PTZ_ZOOMDECSTOP     = 23,     // nTypeThird: δ����
	PTZ_DEVOPEN         = 30,    // nTypeThird: �������غ�(0-7)
	PTZ_DECCLOSE        = 31,    // nTypeThird: �������غ�(0-7)
	PTZ_AUTOSTOP        = 32,    // nTypeThird: δ����
	PTZ_CLEARPOINT      = 33,    // nTypeThird: δ����
}HOSS_PTZ_CTRL_CMD;




typedef enum
{
	UP_START 				= 0,
	UP_STOP 				= 1,
	DOWN_START				= 2,
	DOWN_STOP				= 3,
	LEFT_START				= 4,
	LEFT_STOP				= 5,
	RIGHT_START				= 6,
	RIGHT_STOP				= 7,
	ROTATION_START			= 8,
	ROTATION_STOP			= 9,
	
	AADD_START				= 10,	// Iris
	AADD_STOP				= 11,
	ADEC_START				= 12,	// Iris
	ADEC_STOP				= 13,
	
	FADD_START				= 14,	// Focus
	FADD_STOP				= 15,
	FDEC_START				= 16,	// Focus
	FDEC_STOP				= 17,
	
	DADD_START				= 18,	// Zoom
	DADD_STOP				= 19,
	DDEC_START				= 20,	// Zoom
	DDEC_STOP				= 21,
	
	LIGHT_START				= 22,	// Light
	LIGHT_STOP				= 23,
	
	RAIN_START				= 24,	// Rain Brush
	RAIN_STOP				= 25,
	
	TRACK_START				= 26,	// Track
	TRACK_STOP				= 27,
	
	IO_START				= 28,	// IO
	IO_STOP					= 29,
	
	SET_POINT				= 30,	// Point
	GOTO_POINT				= 31,
	CLEAR_POINT				= 32,
	
	BYPASS					= 33,	// Add the code by lvjh, 2011-01-11

   	LEFTUP_START			= 34,
	LEFTUP_STOP				= 35,
	LEFTDOWN_START			= 36,
	LEFTDOWN_STOP			= 37,
	RIGHTUP_START			= 38,
	RIGHTUP_STOP			= 39,
	RIGHTDOWN_START			= 40,
	RIGHTDOWN_STOP			= 41,
		
	VERT_ROTATION_START		= 42,
	VERT_ROTATION_STOP		= 43,
}PTZ_CTRL_CMD;

// PTZ�б�
typedef struct
{
	unsigned long nNum;						//����
	//unsigned long nChnIndex;				//��ǰͨ����PTZ
	char strName[MAX_PTZ_NUM][MAX_PTZ_NAME];	//PTZ����
	unsigned long nReserve;
	
}PTZ_INDEX;

//PTZ�ļ�
typedef struct
{
	char strName[MAX_PTZ_NAME];		//PTZ����
	unsigned long nSize;			//�ļ���С
	//char strData[5*1024];			//PTZ�ļ�����
}PTZ_DATA;	// ʵ�ʷ��͡���������ṹʱ�����ݴ�СӦΪ��sizeof(PTZ_DATA)-5*1024+size

//PTZ����
typedef struct
{
	char strName[MAX_PTZ_NAME];		//PTZ����
	unsigned long nAddr;			//PTZ��ַ
	unsigned long nRate;			//PTZ�ٶ�
	
}PTZ_PARAM;

//PTZ��������
typedef struct
{
	unsigned long nCmd;
	unsigned long nValue;
	
}PTZ_CMD;

//͸�������õ�
typedef struct ptzCtrlCmd
{
	unsigned long nCmdLen;
	char cmd[MAX_CMD_SIZE];
}PTZCTRLCMD;

typedef struct 
{
	char name[32];
	unsigned int addr;
	unsigned int rate;
	COM_PARAM param;
	PTZCTRLCMD ptzCmd[MAX_CMD_NUM];
	unsigned int checksum;
	unsigned int sum_start_bit;
	unsigned int sum_end_bit;
	unsigned int reserve;
		
}PTZCTRLINFO, *PPTZCTRLINFO;

typedef struct 
{
	COM_PARAM param;
	unsigned long nCmdLen;
	char cmd[MAX_CMD_SIZE];
}PTZCTRLCMD_THROUGH;

// Add the code by lvjh, 2009-09-16
typedef struct
{
	unsigned long bEnabled;			//�Ƿ�����Ѳ���Զ�ֹͣ(PT��Ĭ������)
	unsigned long nAutoTimeLen;		//����Ѳ�����Զ�ֹͣ��ʱ��(��λ���룩
	unsigned long dwReserved;		//������Ϊ��
}PTZ_AUTO_CTRL;

int setChnPtz(int nChannel, int index);
int getChnPtz(int nChannel, int *index);
int setChnPtzExt(int nChannel, char *ptzName, int addr, int rate);
int getChnPtzExt(int nChannel, char *ptzName, int *addr, int *rate);
int getPtzList(PTZ_INDEX *list);
int getPtzRs485Com(int nChannel, COM_PARAM *param);
int setPtzRs485Com(int nChannel, COM_PARAM param);
int getPtzRs232Com(int nChannel, COM_PARAM *param);
int setPtzRs232Com(int nChannel, COM_PARAM param);
int getPtzRate(int nChannel, int *param);
int addPtz(char *ptzName, char *fileData, int size);
int deletePtz(char *ptzName);
int ptzStart();
int ptzStop();
int ptzPause();
int ptzResume();
int ptzControl(int nChannel, PTZ_CMD cmd);

int setAutoPtz(int nChannel, PTZ_AUTO_CTRL param);
int getAutoPtz(int nChannel, PTZ_AUTO_CTRL *param);

// ͸������
//int ptzControlThrough(int nChannel, int nComType, PTZCTRLINFO cmd);
int ptzControlThrough(int nChannel, int nComType, PTZCTRLCMD_THROUGH cmd);

#endif
 
