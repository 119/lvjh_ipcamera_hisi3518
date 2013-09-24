#ifndef __VIN_MODULE_H_
#define __VIN_MODULE_H_

#define MAX_VIN_CHANNEL 1

// ��Ƶ�ڵ�����
typedef struct 
{
	unsigned long bMask;			//�Ƿ�����
	unsigned short x;				//X����
	unsigned short y;				//Y����
	unsigned short nWidth;			//���
	unsigned short nHeight;			//�߶�
	
}VIDEO_MASK;

typedef struct vinModuleInfo  *vinModuleInfo_t;

typedef int (*vinOpenFun_t)(int nChannel);
typedef int (*vinCloseFun_t)(int nChannel);
typedef int (*vinSetupFun_t)(int nChannel, int standard);
typedef int (*vinGetSetupFun_t)(int nChannel, int *standard);
typedef int (*vinStartFun_t)(int nChannel);
typedef int (*vinStopFun_t)(int nChannel);
typedef int (*vinGetStreamFun_t)(int nChannel, void *stream, int *size);
typedef int (*vinReleaseStreamFun_t)(int nChannel);
typedef int (*vinSetMaskFun_t)(int nChannel, VIDEO_MASK mask);

struct vinModuleInfo 
{
    vinOpenFun_t				open;
    vinCloseFun_t				close;
    vinSetupFun_t				setup;
    vinGetSetupFun_t			getSetup;
    vinStartFun_t				start;
    vinStopFun_t				stop;
    vinGetStreamFun_t			getStream;
    vinReleaseStreamFun_t		releaseStream;
	vinSetMaskFun_t				setMask;
};

#endif
