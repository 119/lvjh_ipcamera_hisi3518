#ifndef __PARAM_H_
#define __PARAM_H_

#include "videoEnc/videoEncModule.h"
#include "audioEnc/audioEncModule.h"
#include "videoIn/videoInModule.h"
#include "com.h"
#include "ptz.h"
#include "rtc.h"
#include "ntpSetup.h"
#include "wlanSetup.h"
#include "3g.h"

#ifdef RECORD
#include "include/recordStruct.h"
#include "include/recordSDK.h"
#endif

#define HARDWARE_VERSION			0x00010003
#define SOFTWARE_VERSION			0x00030052
#define SOFTWARE_VERSION_EXT		0x000D0030
#define SOFTWARE_COMPILE_DATE		0x07DD00802
#define AUDIOSAMPLES				8000


#define PARAM_FILE_PATH				"/param"
#define PARAM_CONFIGURE_FILE		"/param/param.cur"
#define PARAM_STATUS_FILE			"/param/param.stat"
#define DEVICE_CONFIGURE_FILE		"/param/dev.conf"
#define DEBUG_CONFIGURE_FILE		"/param/debug.conf"
#define NETWORK_CONFIGURE_FILE		"/param/network.conf"
//#define MAC_CONFIGURE_FILE			"/mnt/mtd/dvs/etc/mac.conf"
#define MAC_CONFIGURE_FILE			"/param/mac.conf"
#define USER_INFO_CONFIGURE_FILE	"/param/user_info.conf"
#define RTSPPORT_CONFIGURE_FILE		"/param/RTSPPort.conf"
#define WEBSERVERPORTPATH 			"/param/webServerPort.conf"
#define WEBSERVER_MOBILE_CONFIG 	"/param/webserMobileConfig.conf"
#define UPNP_CONFIGURE_FILE			"/param/upnp.conf"
#define IP_WIFI_CONFIGURE_FILE		"/param/ip_wifi.conf"
#define PARAM_REBOOT_HOUR_FILE		"/param/param.hour"



#define ID_CONFIGURE_FILE			"/param/id.conf"
#define P2P_CONFIGURE_FILE			"/param/p2p.conf"


#define ETH_WIRE_DEV				"eth0"
//#define ETH_WIRELESS_DEV			"rausb0"
#define ETH_WIRELESS_DEV			"ra0"



#define MAX_CHANNEL					1
#define MAX_PROBE_IN				12
#define MAX_PROBE_OUT				1

#define MAX_TIME_SEGMENT			2
#define MAX_COM_NUM					2
#define MAX_IP_LEN					16
#define MAX_LOGO_DATA				30*1000
#define MAX_VIDEO_MASK_NUM			4

#define SOFTWARE_RESET				1001
#define HARDWARE_RESET				1002
#define UPNP_MAPPING_PROT			19000	
#define WIFI_CONF_PATH 				"/param/wifi.conf"
#define WIFI_STATUS_CONF_PATH 		"/param/wifi_status.conf"
#define HTTPD_STATUS_CONF_PATH 		"/param/httpd.conf"


#define NET_MAX_AP_NUMBER 			40

#ifdef IPHONE
#define DEFAULT_WIRE_IP				"192.168.188.188"
#define DEFAULT_WIRE_GATEWAY		"192.168.188.1"
#else
#define DEFAULT_WIRE_IP		    	"192.168.100.19" //"192.168.1.250"


#define DEFAULT_WIRE_GATEWAY		"192.168.100.1"
#endif

#define DEFAULT_WIRE_MASK			"255.255.255.0"

#define DEFAULT_WIRELESS_IP			"192.168.100.17"
#define DEFAULT_WIRELESS_GATEWAY	"192.168.100.1"
#define DEFAULT_WIRELESS_MASK		"255.255.255.0"

#define DEFAULT_MULTI_ADDR			"234.5.6.7" //"224.0.0.2"
#define DEFAULT_WEB_PORT			80
#define DEFAULT_RTSP_PORT			554


#ifdef IPHONE
#define DEFAULT_CMD_PORT			9500
#define DEFAULT_MEDIA_PORT			9500
#define DEFAULT_MULTI_PORT			9500
#else
#define DEFAULT_CMD_PORT			4000
#define DEFAULT_MEDIA_PORT			3000//4000
#define DEFAULT_MULTI_PORT			6000 //4000
#endif

/*
#ifdef CCD
#define DEFAULT_BRIGHTNESS			0x80
#define DEFAULT_CONTRAST			0x80
#define DEFAULT_HUE					0x00
#define DEFAULT_SATURATION			0x80
#endif
*/

#ifdef CCD
#define DEFAULT_BRIGHTNESS			0xA5
#define DEFAULT_CONTRAST			0xBE
#define DEFAULT_HUE					0x00
#define DEFAULT_SATURATION			0xBE
#endif


#ifdef HD_CMOS
#define DEFAULT_BRIGHTNESS			39
#define DEFAULT_CONTRAST			255
//#define DEFAULT_HUE					0x80
#define DEFAULT_HUE					0x00
#define DEFAULT_SATURATION			202
#endif

#define SPECIAL_IP_MANUAL_UPLOAD	0x10
#define SPECIAL_IP_AUTO_UPLOAD		0x08
#define FTP_UPLOAD					0x02
#define TFTP_UPLOAD					0x04
#define EMAIL_UPLOAD				0x01
#define LOCAL_STORE					0x20
#define SCHEDULE_EMAIL_UPLOAD		0x21



#define MAX_USER_COUNT				10	//����û���
#define ADMIN_USER					"888888" //"admin"
#define ADMIN_PASSWD 				"888888" //"admin"

//#define ADMIN_USER					"888888" //"admin"
#define GUEST_USER					"1"      //"guest"

//��Ƶ
#define ENCODE_VIDEO_DIVX			0x58564944
#define ENCODE_VIDEO_XVID			0x44495658
#define ENCODE_VIDEO_HISI			0x49534948
#define ENCODE_VIDEO_H264			0x34363248
#define ENCODE_VIDEO_MJPEG			0x47504A4D

//��Ƶ
#define ENCODE_AUDIO_MP3			0x55
#define ENCODE_AUDIO_G722 			0x65
#define ENCODE_AUDIO_G723 			0x14
//#define ENCODE_AUDIO_G711			0x25
#define ENCODE_AUDIO_G711			0x6
#define ENCODE_AUDIO_G726			0x64

// ϵͳ��Ϣ: 256 bytes
typedef struct
{
	char strDeviceName[32];				//����������	
	char strDeviceID[48];				//������ID
	unsigned long nLanguage;
	
	// ���²��������޸�
	unsigned long nHardwareVersion;		// Ӳ���汾,��16λ�����汾,��16λ�Ǵΰ汾
	unsigned long nSoftwareVersion;		//Ӧ�ó���汾��,��16λ�����汾�ţ���16λΪ�ΰ汾��
	unsigned long nSoftwareBuildDate;	//Ӧ�ó���������� 0xYYYYMMDD
	
	unsigned long nCpuType;				//CPU����
	unsigned long nCpuFreq;				//CPU��Ƶ
	unsigned long nRamSize;				//RAM�Ĵ�С
	
	unsigned long nChnNum;				//AVͨ������
	unsigned long nVideoEncType;		//��Ƶ��������
	unsigned long nAudioEncType;		//��Ƶ��������
	
	unsigned char byDeviceType;			//�豸����, 1:DVR 2:ATM DVR 3:DVS 4:COMS IPCAMERA 5:CCD IPCAMERA 6:HDCOMS IPCAMERA
	
	unsigned char byAlarmInNum;			//̽ͷ������
	unsigned char byAlarmOutNum;		//̽ͷ�����
	
	unsigned char byRS232Num;			//232���ڸ���
	unsigned char byRS485Num;			//485���ڸ���
	
	unsigned char byDiskNum;			//Ӳ�̸���
	
	unsigned char byVGANum;				//VGA�ڵĸ���
	unsigned char byUSBNum;				//USB�ڵĸ���
	
	//unsigned long nReserve[32];		//����
	
	unsigned long nDoubleStreamFlag;	//˫������ʶ
	unsigned long nReserve[31];			//����
	
}SYS_INFO;

// IP ����
typedef struct
{
	int nOnFlag;
	unsigned long nStartIP;
	unsigned long nStopIP;
	unsigned long nReserve;
}LOGON_IP_BIND_PARAM;


//�û���Ϣ
typedef struct
{
	char strName[32];			//�û���
	char strPsw[32];			//����
	unsigned long nRight[4];		//128������Ȩ��

	LOGON_IP_BIND_PARAM ipBind;

}USER_INFO;						//���10���û�

// 1152bytes
typedef struct
{
	USER_INFO Admin;
	USER_INFO Users[MAX_USER_COUNT];
	USER_INFO Guest;
	
}USER_INFO_PARAM;

//��Ƶ��ʽ
typedef struct
{
	unsigned long nStandard;		// 0: PAL, 1: NTSC
	
}VIDEO_STANDARD_PARAM;

// ��Ƶ�������
typedef struct
{	
	unsigned long nBrightness; 	//����
	unsigned long nHue;  		//ɫ��
	unsigned long nContrast;		//�Աȶ�
	unsigned long nSaturation;  	//���Ͷ�
	//unsigned long acutance;  	//���
		
	unsigned long reserve;
}VIDEO_IN_ATTR;

typedef struct
{
	char strName[32];
	unsigned long nReserve;
}VIDEO_CHANNEL_NAME;

/*
// ��Ƶ�������
typedef struct
{
	unsigned long nEncodeMode;		// 0: CBR, 1: VBR
	
	unsigned long nEncodeWidth;		// ͼ����
	unsigned long nEncodeHeight;		// ͼ��߶�
	unsigned long nKeyInterval;		// I֡���
	unsigned long nFramerate;		// Ŀ��֡�� 
	unsigned long nBitRate;			// Ŀ������
	unsigned long nMaxQueue;		// �������ϵ��
	unsigned long nMinQueue;		// ��С����ϵ��
	
	unsigned long reserve;			// ��nEncodeModeΪCBRʱ��reserveΪ0ʱ����ʾ�������ȣ�reserveΪ1ʱ����ʾ֡������
	
}VENC_PARAM;
*/

//WIFI ɨ�����
typedef struct
{
	char ch[8];
	char signal[8];
	char w_mode[8];
	char nt[8];
	char ssid[24];
	char bssid[24];
	char security[24];	
}WIFIINFO;

typedef struct tagDVSNET_WIFI_DEVICE
{
  char sSSID[36];   //WIFI ����
 // char bSSID[36];   //WIFI MAC ��ַ
  unsigned long     nSignal;//�ź�ǿ��
  unsigned long     nChannel;//ͨ����
  unsigned long     nNetType;//WIFI	ģʽ
  unsigned long     nEncryptType;//��֤����
  unsigned long     nAuthMode;//��������
  unsigned long 	WPAFormat; // ��������0 Ϊascll�룬1λ16����
  unsigned long     nReserved;
}DVSNET_WIFI_DEVICE;
typedef struct SDK_NetWifiDeviceAll
{
	int nDevNumber;
	DVSNET_WIFI_DEVICE vNetWifiDeviceAll[NET_MAX_AP_NUMBER];
}WIFI_DEVICE_ALL;

 typedef struct NODE
{
	DVSNET_WIFI_DEVICE wifi_param;
	struct NODE *next;
}List;


// ��Ƶˮӡ����
typedef struct
{
    unsigned long nOnFlag;			// ���ñ�־
    char strKey[8];					// ����ˮӡ��Կ�ַ���
    char strSymbol[16];				// ����ˮӡ��ʶ�ַ���
    unsigned long nDensity;			// ����ˮӡ��ȫ��:0��1��2
    
}VIDEO_WM_PARAM;

//ʱ��OSD
typedef struct 
{
	unsigned long bShow;			//�Ƿ���ʾ
	unsigned long nFormat;			//��ʽ: 1: XXXX/XX/XX 2:XXXX/XX/XX XX:XX:XX
	unsigned short x;				//X����
	unsigned short y;				//Y����
	unsigned long nColor;
}TIME_OSD;

//����OSD
typedef struct 
{
	unsigned long bShow;			//�Ƿ���ʾ
	unsigned short x;				//X����
	unsigned short y;				//Y����
	char sTitle[32];				//��������
	unsigned long nColor;
	
}TITLE_OSD;

//����OSD
typedef struct 
{
	unsigned long bShow;			//�Ƿ���ʾ
	unsigned short x;				//X����
	unsigned short y;				//Y����
	unsigned long nColor;

}BITSTREAM_OSD;

//OSD��������
typedef struct 
{
	TIME_OSD		TimeOSD;		//ʱ��OSD
	BITSTREAM_OSD	BitsOSD;		//����OSD
	TITLE_OSD		TitleOSD[4];	//����OSD
}OSD_PARAM;

// LOGO����
typedef struct
{
	unsigned long bShow;			//��ʾ��־
	unsigned long bTranslucent;		//�Ƿ�͸��
	unsigned long nTransColor;		//͸��ɫ
	unsigned short x;				//X����
	unsigned short y;				//Y����
	unsigned long nWidth;			//��, ������ż��
	unsigned long nHeight;			//��, ������ż��
	unsigned long nDataLen;			//ͼ�����ݳ���	
	
}LOGO_PARAM;	// ʵ�ʷ��͡���������ṹʱ�����ݴ�СӦΪ��sizeof(LOGO_PARAM)-30*1000+Width*Height*3

/*
// MASK����
typedef struct
{
	unsigned long bMask;			//�Ƿ�����
	unsigned short x;				//X����
	unsigned short y;				//Y����
	unsigned short nWidth;			//���, ������ż��
	unsigned short nHeight;			//�߶�, ������ż��
}VIDEO_MASK;
*/

//��Ƶ�ڵ�����
typedef struct
{
	VIDEO_MASK VideoMask[MAX_VIDEO_MASK_NUM];
}MASK_PARAM;

/*
// ��Ƶ�ƶ�������
typedef struct
{
	unsigned char nSensibility;				//��Ƶ�ƶ�����������
	unsigned char mask[11*9];				//��Ƶ�ƶ����ĺ��
	unsigned long nReserve;					//����
	
}VIDEO_MOTION_PARAM;
*/

//��Ƶ����
typedef struct
{
	unsigned long nBitRate;
	unsigned long nChannel;
	unsigned long nBitsPerChannel;
	
	unsigned long reserve;
}AUDIO_PARAM;

/*
//��Ƶ�������
typedef struct 
{
	unsigned long nOnFlag;		// �Ƿ�������Ƶ����
	unsigned long nChannels;	// ����ͨ�����������ǵ�ͨ����Ҳ������˫ͨ��
	unsigned long nSampleRate;	// ���������
	unsigned long nBitRate;		// �������
	unsigned long nMode;		// ����ģʽ��0����������1��MS��������2��˫ͨ����3����ͨ��
	
}AENC_PARAM;
*/

//��Ƶ�������
typedef struct 
{
	unsigned long nChannels;
	unsigned long nSampleRate;
	unsigned long nBitRate;
	unsigned long nMode;		//����������
	
}ADEC_PARAM;

//ftp�ϴ����ò���
/*
typedef struct
{
	char strIP[16];
	char strName[32];
	char strPsw[32];
	unsigned long nPort;	
	
}FTP_PARAM;
*/

//add code by lvjh 2012-08-18
typedef struct tagDVSNET_RTSP_PARAM
{
	unsigned long bEnable;
	unsigned long nRtspPort;
	unsigned long nRTPStartPort;
	unsigned long nReserve;
}DVSNET_RTSP_PARAM;

typedef struct
{
	char strIP[48];
	char strName[16];
	char strPsw[16];
	char strPath[32];
	unsigned long nPort;	

}FTP_PARAM;

//tftp�ϴ����ò���
typedef struct
{
	char strIP[16];
	unsigned long nPort;	

}TFTP_PARAM;

//email�ϴ����ò���
typedef struct
{
	char strIP[64];
	char strName[32];
	char strPsw[32];

	char strFrom[64];		//
	char strTo[128];		//���ǵ���EMAIL
	char strCc[64];			//���ǵ���EMAIL
	char strBcc[64];		//���ǵ���EMAIL

	char strHeader[128];	//�ʼ�����

	unsigned long nReserve;
	
}EMAIL_PARAM;

#ifndef RECORD

typedef struct
{
	unsigned char start_hour;
	unsigned char start_minute;
	unsigned char end_hour;
	unsigned char end_minute;

}TIME_SEGMENT;

typedef struct
{
	unsigned long nOnFlag;
	TIME_SEGMENT time_segment[MAX_TIME_SEGMENT];

}TIME_SEGMENTS;

#endif

// ��Ƶ��ʧ��������
typedef struct
{		
	unsigned long nLinkProbe;				//̽ͷ��������
	unsigned long nLinkProbeTime;			//̽ͷ�������������ʱ��,��λΪ��
	
	unsigned long nLinkRecord;				//¼������,��λ����
	unsigned long nLinkRecordTime;			//����¼���ʱ��,��λΪ����
	
	unsigned long nLinkSnapshot;			//ץ������
	unsigned long nLinkSnapshotInterval;	//����ץ�ļ��, ��λΪ��
	unsigned long nLinkSnapshotUploadFlag;	//����ץ���ϴ�����
	unsigned long nLinkSnapshotUploadMode;	//����ץ���ϴ���ʽ��smtp��ftp��tftp���ͻ��˽���
	
	TIME_SEGMENTS day[8];					//ʱ��β���
	
	unsigned long reserve;					//����
	
}VIDEO_LOST_ALARM_PARAM;

// ��Ƶ�ƶ���������
typedef struct
{		
	unsigned long nLinkProbe;				//̽ͷ��������
	unsigned long nLinkProbeTime;			//̽ͷ�������������ʱ��,��λΪ��
	
	unsigned long nLinkRecord;				//¼������,��λ����
	unsigned long nLinkRecordTime;			//����¼���ʱ��,��λΪ����
	
	unsigned long nLinkSnapshot;			//ץ������
	unsigned long nLinkSnapshotInterval;	//����ץ�ļ��, ��λΪ��
	unsigned long nLinkSnapshotUploadFlag;	//����ץ���ϴ�����
	unsigned long nLinkSnapshotUploadMode;	//����ץ���ϴ���ʽ��smtp��ftp��tftp���ͻ��˽���
	
	TIME_SEGMENTS day[8];					//ʱ��β���
	
	unsigned long reserve;					//����
	
}VIDEO_MOTION_ALARM_PARAM;

// ̽ͷ��������
typedef struct
{	
	//unsigned long nOnFlag;					//���ñ�־
 	unsigned long nLinkProbe;				//̽ͷ��������
	unsigned long nLinkProbeTime;			//̽ͷ�������������ʱ��,��λΪ��

	
	unsigned long nLinkRecord;				//¼������,��λ����
	unsigned long nLinkRecordTime;			//����¼���ʱ��,��λΪ����
	
	unsigned long nLinkSnapshot;			//ץ������
	unsigned long nLinkSnapshotInterval;	//����ץ�ļ��, ��λΪ��
	unsigned long nLinkSnapshotUploadFlag;	//����ץ���ϴ�����
	unsigned long nLinkSnapshotUploadMode;	//����ץ���ϴ���ʽ��smtp��ftp��tftp���ͻ��˽���
	
	unsigned char bPresetNo[32];			//��Ӧͨ����Ԥ�õ����
	
	TIME_SEGMENTS day[8];
	
	char probeName[32];
	
	unsigned long reserve;
	
}PROBE_IN_ALARM_PARAM;

 // ��ʱ��������
 typedef struct
 {	
 	unsigned long bEnable;         //ʹ��
     unsigned long nRebootInterval; //�����Сʱ��
     unsigned long nRebootHour;  //Сʱ
     unsigned long nRebootMinute; //����
     unsigned long dwReserved;    //����
 }TIME_REBOOT_PARAM;


//��ǰ������Ϣ
typedef struct 
{
	unsigned long nMotionStatus;//�ƶ�����״̬
	unsigned long nAlarmInStatus;//�ܿ���
	unsigned long nIrAlarmStatus[12];//̽ͷ����״̬
	unsigned long nReserve[12];
}ALARM_STATUS_PARAM;



// ̽ͷ�������
typedef struct
{
	char probeName[32];
	TIME_SEGMENTS day[8];
	 
}PROBE_OUT_ALARM_PARAM;

//̽ͷ��״̬, ������ȡ������̽ͷ��״̬����Ҫ����̽ͷ��״̬��Ҫ�Ȼ�ȡ�����ã�����statusÿ��λ����һ��̽ͷ
typedef struct
{
	unsigned long status;
	
}PROBE_STATUS;

// �������: 100 bytes
typedef struct
{ 
	unsigned long nDhcpOnFlag;
	char byServerIp[MAX_IP_LEN];	//������IP��ַ
	char byServerMask[MAX_IP_LEN];	//��������
	char byGateway[MAX_IP_LEN];		//����
	char byDnsAddr[MAX_IP_LEN];		//DNS��ַ
	char byMultiAddr[MAX_IP_LEN];	//�鲥��ַ
	unsigned short wServerPort;		//���ݶ˿�
	unsigned short wMultiPort;		//�ಥ�˿�
	unsigned short wWebPort;		//Web�˿�
	unsigned char strPhyAddr[6];	//MAC��ַ
	unsigned long dwReserved;
	
}NET_PARAM;

//PPPOE
typedef struct
{
	unsigned long nOnFlag;	//���ñ�־
	char userName[32];
	char userPsw[32];
	
	unsigned long reserve;
	        
}PPPOE_PARAM;

  //P2P����������
typedef struct tagDVSNET_P2P_PARAM
{
	unsigned long	bEnabled;									//�Ƿ�����P2Pע��
	char			sServerUrl[64];								//ע���P2P������URL
	char			strUser[32];
	char			strPasswd[32];
	unsigned long	nPort;										//ע���P2P�������˿�
	unsigned long	nInterval;									//ע���ʱ����(��)
	unsigned long	dwReserved;									//����
}DVSNET_P2P_PARAM;

//DDNS
typedef struct
{
	unsigned long nOnFlag;	//���ñ�־
	char domain[128];
	char userName[32];
	char userPsw[32];	
	char ipAddr[32];
	unsigned long port;
	unsigned long authType;

	unsigned long reserve;	//
	        
}DDNS_PARAM;


//DDNS
typedef struct
{
	unsigned long nOnFlag;	//�����������ñ�־
	char sIpAddr[32];//�����������ĵ�ַ
	unsigned long nPort;//�����������Ķ˿�
	unsigned long nTime;//����ʱ����
	unsigned long nReserve;	//Ԥ��λ
	        
}YIYUAN_ALARM_DDNS_PARAM;


typedef struct
{
	int nOnFlag;
	int nWebPort;
	int nCmdPort;
	int nWebPortStatus;
	int nCmdPortStatus;
	int nReserve;

}UPNP_PARAM;

/*
// ���ڲ���
typedef struct
{
	unsigned long nBaudRate;	//������
	unsigned long nDataBit;		//����λ
	unsigned long nParity;		//У��λ
	unsigned long nStopBit;		//ֹͣλ
	
	unsigned long nReserve;

}COM_SETUP_PARAM;
*/

// Զ�̶Խ�IP
typedef struct
{
	char strTalkIP[16];				//�������Խ�IP
	unsigned long nPort;			//�˿�
	
	unsigned long nReserve;
	
}TALK_REQUEST_PARAM;

// �����������Ӳ���
typedef struct
{
	unsigned long nOnFlag;		//���ñ�־
	char strConnectURL[128];	//�������ӵķ���������
	unsigned long nPort;		//�������ӵķ������˿�
	unsigned long nInterval;	//�������ӵļ��ʱ��
	
	unsigned long nReserve;
	
}REMOTE_CONNECT_PARAM;

// �����������Ӳ���
typedef struct
{
	unsigned long nOnFlag;		//���ñ�־
	char strConnectURL[64];		//�������ӵķ���������
	char strUserName[32];
	char strPsw[32];
	unsigned long nPort;		//�������ӵķ������˿�
	unsigned long nInterval;	//�������ӵļ��ʱ��
	
	unsigned long nReserve;
}P2P_PARAM;

//P2P�������Ӳ��� 
/*IPC<-----P2P Server ��Ӧ������:NETCMD_P2P_REQCONBACK*/
typedef struct tagDVSNET_REQCONBACK_INFO
{
	unsigned long nOnFlag;		//���ñ�־
	char strConnectURL[128];			// �������ӵ�(PC�˻�ת��������)�Ĺ���IP
	unsigned long	nPort;							// �������ӵ�(PC�˻�ת��������)�Ĺ����˿�
	unsigned long	dwUserID;						// �������ӵ�(PC��Ϊ�û�ID��ת��������Ϊ0xFFFFFFFF)�û�ID
	unsigned long nInterval;	//�������ӵļ��ʱ��

	unsigned long	nReserve;
}DVSNET_REQCONBACK_INFO;


/*IPC<---->P2P Server PC<---->P2P Server��Ӧ�����֣�NETCMD_P2P_REGDATAPORT*/
typedef struct tagDVSNET_REGISTER_INFO
{
	char			strDeviceName[32];	//����������	
	char			strSerialNo[48];	//IPC���к�
	char			strLocalIp[MAX_IP_LEN];			//����IP��ַ ,IPC��PC�ı���IP
	char			strRemoteIp[MAX_IP_LEN];		//Զ��IP��ַ ,ע��ʱΪP2P Server��IP��P2P Server����ʱΪIPC��PC�Ĺ���IP
	unsigned short	wLocalPort;						//���ض˿� ,IPC��PC�ı��ض˿�	
	unsigned short  wRemotePort;					//Զ�̶˿�,ע��ʱΪP2P Server�Ķ˿�,P2P Server����ʱΪIPC��PC�Ĺ����˿�
	unsigned long	dwDeviceType;					//�豸����	//0xFFFFFFFFΪ�ͻ���
	unsigned long   dwUserID;						//��һ��ע��ʱ����P2P Server���أ���Ϊ�жϵ�����
	unsigned long   dwCount;						//��ʶΪ�ڼ������� 0-��һ�Σ�1-�ڶ���
	unsigned long	dwNatType;						//NAT ���� 0-���� 1-��¡(ȫ��¡��IP���ƿ�¡�ɴ�) 2-�˿����ƿ�¡(���ܿ��Դ�) 3-�Գ���(ֻ�ܷ������ӻ�ת��)
	unsigned long	dwUpnp;							//�Ƿ������UPNP
	unsigned long	dwUpnpWebPort;					//Web
	unsigned long	dwUpnpDataPort;					//���ݶ˿�
	unsigned long	dwReserved;						//����
	char			strUser[32];			//�û���
	char			strPasswd[32];
}DVSNET_REGISTER_INFO;


/*IPC----->P2P Server ��Ӧ������:NETCMD_P2P_ENDCONBACK */
typedef struct tagDVSNET_ENDCONBACK_INFO
{
	unsigned long	dwUserID;						// �������ӵ�(PC�˻�ת��������)��ID��
	unsigned long	dwReserved;
}DVSNET_ENDCONBACK_INFO;


/*IPC<-----P2P Server ��Ӧ������:NETCMD_P2P_REQHOLE*/
typedef struct tagDVSNET_REQHOLE_INFO
{
	char			strIp[MAX_IP_LEN];				// ����򶴵�PC�˵Ĺ���IP
	unsigned long	dwPort;							// ����򶴵�PC�˵Ĺ����˿�
	unsigned long	dwUserID;						// ����򶴵�PC�˵Ŀͻ�ID��
	unsigned long	dwReserved;						// ����
}DVSNET_REQHOLE_INFO;


/*IPC----->P2P Server ��Ӧ������:NETCMD_P2P_ENDHOLE*/
typedef struct tagDVSNET_ENDHOLE_INFO
{
	unsigned long	dwUserID;						// �������PC�˵�ID��
	unsigned long	dwReserved;
}DVSNET_ENDHOLE_INFO;

//p2p �յ�����ȷ������
typedef struct tagDVSNET_CMDRECEIVED_INFO
{
	unsigned long	dwUserID;
	unsigned long	dwCmdReceived;
	unsigned long	dwReserved;
}DVSNET_CMDRECEIVED_INFO;

typedef struct tagRECORD_PARAM
{
	unsigned long nCoverMode;
	unsigned long nAudioFlag;

	unsigned long nReserve;
	
}RECORD_PARAM;


typedef struct tagUPNP_UPNP_PORT_INFO
{
	char			strLocalIp[16];			//����IP��ַ ,IPC��PC�ı���IP
	char			strRemoteIp[16];		//Զ��IP��ַ ,ע��ʱΪP2P Server��IP��P2P Server����ʱΪIPC��PC�Ĺ���IP
	char 			wLocalPort[8];						//���ض˿� ,IPC��PC�ı��ض˿�	
	char    		wRemotePort[8];					//Զ�̶˿�,ע��ʱΪP2P Server�Ķ˿�,P2P Server����ʱΪIPC��PC�Ĺ����˿�
}UPNP_PORT_INFO;

typedef struct tagUPNP_WIFI_IP_PORT_INFO
{
	char			strLocalIp[16];			//����IP��ַ ,IPC��PC�ı���IP
	char			strWIFIIp[16];		// wifi IP��ַ
	unsigned long	bEnabled;			//�Ƿ�����
	unsigned long	upnpEthNo;			//���� 0-���� 1-����
	
	unsigned short  nDataPort;				
	unsigned short 	nWebPort;					
	unsigned short 	nRtspPort;	
	
	unsigned short	upnpWebStatus;		//upnp	WEB״̬
	unsigned short 	upnpDataStatus;		//upnp	DATA״̬
	unsigned short  upnpRtspStatus;		//upnp	rtsp״̬

	unsigned long	nReserved;			//����
	unsigned long	nOnFlag;			
}UPNP_WIFI_IP_PORT_INFO;


typedef struct tagUPNP_PORTMAPPING_INFO
{
	char			strRemoteIp[16];	
	int				nOnFlag;
	int 			nWebPortMappingStatus;
	int    			wRemoteWebPort;		
	int    			wRemoteMediaPort;	
	int    			wRemoteDataPort;		
	int    			wRemotePamerSetPort;	
	int    			wRemoteRecodFilePort;
	
}UPNP_PORTMAPPING_INFO;


   //UPNP����
typedef struct tagDVSNET_UPNP_PARAM
{
	unsigned long	bEnabled;			//�Ƿ�����
	unsigned long	upnpEthNo;			//���� 0-���� 1-����
	unsigned short	upnpWebPort;		//upnp WEB�˿�
	unsigned short	upnpDataPort;		//upnp DATA�˿�
	unsigned short	upnpRtspPort;		//upnp rtsp�˿�
	
	unsigned long	upnpWebStatus;		//upnp	WEB״̬
	unsigned long	upnpDataStatus;		//upnp	DATA״̬
	unsigned long	upnpRtspStatus;		//upnp	rtsp״̬
	
	unsigned long	nReserved;			//����
}DVSNET_UPNP_PARAM;



typedef struct
{
	unsigned long nFlip;	// 0: ����, 1������
	
}VIDEO_FLIP_PARAM;

typedef struct
{
	unsigned long nMirror;	// 0: ����, 1������
	
}VIDEO_MIRROR_PARAM;

typedef struct
{
	unsigned long nHz;		// 0: 50HZ, 1: 60HZ
	
}VIDEO_HZ_PARAM;

typedef struct
{
	unsigned long nPath;	// 0: LINEIN, 1:MIC
	
}AUDIO_PATH_PARAM;

typedef struct
{
	TIME_SEGMENTS	day[8];							//����ʱ�� 0-ÿ�� 1-������ ...
	unsigned long	nSnapshotInterval;							//ץ�ļ��(��)
	unsigned long	nSnapshotUploadMode;						//��λ��ʾ���ϴ���ʽ
	unsigned long	dwReserved;									//����
}SCHEDULE_SNAPSHOT_PARAM;

// Add the code by lvjh, 2009-08-24
typedef struct 
{
	unsigned char URL1[64];
	unsigned char URL2[64];
}P2PSERVER;

typedef struct
{
	unsigned char ID[48];
	P2PSERVER P2P;
	unsigned char MAC[6];
	unsigned char nReserve[130];
}DEVICE_CONFIGURE;

//�໭�洦��
#if 1
typedef struct tagMULTIDEV_INFO
{
	char			sDeviceAlias[32];	//����(�豸����)
	char			sDeviceUrl[32];		//�豸IP��ַ������
	unsigned short	wCmdPort;			//����˿�
	unsigned short  wDataPort;			//���ݶ˿�(Ŀǰ����˿ں����ݶ˿���һ����)
	char			sDeviceUser[32];	//��¼�豸�û���
	char			sDevicePwd[32];		//��¼�豸����
	unsigned long	dwReserved;			//����
}MULTIDEV_INFO;

typedef struct tagDVSNET_MULTIDEVALL_INFO
{
	MULTIDEV_INFO	nDeviceInfo[16];
	unsigned long	dwReserved;
}DVSNET_MULTIDEVALL_INFO;
#endif


//�ֶ�ץ�Ľ��
typedef struct tagDVSNET_SNAPSHOT_RESULT
{
	unsigned long	nChannel;				//ͨ����
	unsigned long	nResult;				//0-�ɹ�������Ϊ������
	DATE_PARAM Time;			//ʱ��
	unsigned long	nReserve;				//����
}DVSNET_SNAPSHOT_RESULT;

//�ֶ�¼����
typedef struct tagDVSNET_RECORD_RESULT
{
	unsigned long	nChannel;				//ͨ����
	unsigned long	nResult;				//0-�ɹ�������Ϊ������
	DATE_PARAM		Time;			//ʱ��
	unsigned long	nReserve;				//����
}DVSNET_RECORD_RESULT;



//ϵͳ�������������ļ���
typedef struct
{
	unsigned int paramFlag[32];
	
	// ϵͳ��Ϣ
	SYS_INFO sysInfo;
	
	// �û���Ϣ
	USER_INFO_PARAM userInfo;
	
	// �������
	NET_PARAM network; 
	
	// Wlan
	WIFI_PARAM wlan;
	
	G3_PARAM g3;
	
	NTP_PARAM ntp;
		
	// ��Ƶ��ʽ
	VIDEO_STANDARD_PARAM videoStandard[MAX_CHANNEL];
	
	// ��Ƶ����
	VIDEO_IN_ATTR videoInAttr[MAX_CHANNEL];
	
	// ��Ƶ����
	VIDEO_FLIP_PARAM videoFlip[MAX_CHANNEL];
	
	// ��Ƶ����
	VIDEO_MIRROR_PARAM videoMirror[MAX_CHANNEL];
	
	// ��ƵHZ
	VIDEO_HZ_PARAM videoHz[MAX_CHANNEL];
	
	// �������
	VENC_PARAM videoEnc[MAX_CHANNEL][2];	// ˫����
	
	// ˮӡ����
	VIDEO_WM_PARAM videoWM[MAX_CHANNEL];

	VIDEO_CHANNEL_NAME videoChnName[MAX_CHANNEL];
	
	// OSD����
	OSD_PARAM osd[2];
		
	// LOGO����
	LOGO_PARAM logo[MAX_CHANNEL];
	unsigned char logoData[MAX_LOGO_DATA];
		
	// MASK����
	MASK_PARAM mask[MAX_CHANNEL];

	// MASK����
	VIDEO_MASK mosaic[MAX_CHANNEL];
	
	// ��Ƶ�ƶ�֡�����
	VIDEO_MOTION_PARAM motionDetect[MAX_CHANNEL];
	
	// ��Ƶ����
	AUDIO_PARAM audioInAttr[MAX_CHANNEL];
	
	// ��Ƶ·��
	AUDIO_PATH_PARAM audioInPath[MAX_CHANNEL];
	
	AUDIO_PARAM audioOutAttr[MAX_CHANNEL];
	
	// ��Ƶ�������
	AENC_PARAM audioEnc[MAX_CHANNEL];
	
	// ��������
	VIDEO_LOST_ALARM_PARAM videoLostAlarm[MAX_CHANNEL];
	VIDEO_MOTION_ALARM_PARAM videoMotionAlarm[MAX_CHANNEL];
	PROBE_IN_ALARM_PARAM probeInAlarm[MAX_PROBE_IN];
	PROBE_OUT_ALARM_PARAM probeOutAlarm[MAX_PROBE_OUT];
	
	// FTP���� 
	FTP_PARAM ftp;
	
	//rtsp����  
	DVSNET_RTSP_PARAM rtsp;

	
	// TFTP����
	TFTP_PARAM tftp;
	
	// SMTP����
	EMAIL_PARAM email;

	// JPEG�洢IP
	char JpegStoreIP[MAX_IP_LEN]; 
	
	// PPPOE
	PPPOE_PARAM pppoe;
	
	// DDNS
	DDNS_PARAM ddns;

	// REMOTE CONNECT
	REMOTE_CONNECT_PARAM remoteConnectParam;
	
	
	// PTZ����
	PTZ_PARAM ptz[MAX_CHANNEL];
	
	// ���ڲ���
	//COM_PARAM com[MAX_COM_NUM];
	COM_PARAM rs485[MAX_CHANNEL];	// changed by zhb, 2007-09-17
	COM_PARAM rs232;
		
	// �Խ�IP
	char remoteTalkIP[MAX_IP_LEN];

	
	SCHEDULE_SNAPSHOT_PARAM snapshot[MAX_CHANNEL];
	
	PTZ_AUTO_CTRL ptzAutoCtrl[MAX_CHANNEL];

	//p2p end con back
	UPNP_PORT_INFO  upnpInfo;
	DVSNET_P2P_PARAM upnp;
	DVSNET_P2P_PARAM  p2p;
	DVSNET_REGISTER_INFO p2p_register;
	DVSNET_ENDCONBACK_INFO P2P_endconback;
	DVSNET_REQCONBACK_INFO  remoteP2PConnectParam;
	DVSNET_UPNP_PARAM newUpnpInfo;

#ifdef RECORD
	// RECORD PARAM
	RECORD_PARAM recordParam[MAX_CHANNEL];
	TIMER_RECORD_CHANNEL_PARAM timerRecordParam[MAX_CHANNEL];
	DVSNET_MULTIDEVALL_INFO multiDevAll;
	int irProbeAlarmFlags;
#endif
	//����������
	YIYUAN_ALARM_DDNS_PARAM yiyuanAlarmDdnsParam;
	ALARM_STATUS_PARAM alarm_status;
	TIME_REBOOT_PARAM timerRebootParam;
	time_t 			  lastRebootTime;
}SYS_PARAM;

int generateMultiAddr(char *pMultiAddr);

int initSystemParam(int flag);
int deInitSystemParam();

int getDeviceInfo(SYS_INFO *param);
int setDeviceInfo(SYS_INFO *param);

int getSysInfoParam(SYS_INFO *param);
int setSysInfoParam(SYS_INFO *param);
int getNtpParam(NTP_PARAM *param);
int setNtpParam(NTP_PARAM *param);
int getUserInfoParam(USER_INFO_PARAM *param);
int setUserInfoParam(USER_INFO_PARAM *param);
int getVideoInStandardParam(int nChannel, VIDEO_STANDARD_PARAM *param);
int setVideoInStandardParam(int nChannel, VIDEO_STANDARD_PARAM *param);
int getVideoInAttrParam(int nChannel, VIDEO_IN_ATTR *param);
int setVideoInAttrParam(int nChannel, VIDEO_IN_ATTR *param);
int getVideoEncParam(int nChannel, int type, VENC_PARAM *param);
int setVideoEncParam(int nChannel, int type, VENC_PARAM *param);
int getVideoWMParam(int nChannel, VIDEO_WM_PARAM *param);
int setVideoWMParam(int nChannel, VIDEO_WM_PARAM *param);
int getVideoChnNameParam(int nChannel, VIDEO_CHANNEL_NAME *param);
int setVideoChnNameParam(int nChannel, VIDEO_CHANNEL_NAME *param);
int getOsdParam(int nChannel, OSD_PARAM *param);
int setOsdParam(int nChannel, OSD_PARAM *param);
int getLogoParam(int nChannel, LOGO_PARAM *param, char *data);
int setLogoParam(int nChannel, LOGO_PARAM *param, char *data);
int getMaskParam(int nChannel, MASK_PARAM *param);
int setMaskParam(int nChannel, MASK_PARAM *param);
int getMosaicParam(int nChannel, VIDEO_MASK *param);
int setMosaicParam(int nChannel, VIDEO_MASK *param);
int getVideoMotionParam(int nChannel, VIDEO_MOTION_PARAM *param);
int setVideoMotionParam(int nChannel, VIDEO_MOTION_PARAM *param);
int getAudioInAttrParam(int nChannel, AUDIO_PARAM *param);
int setAudioInAttrParam(int nChannel, AUDIO_PARAM *param);
int getAudioOutAttrParam(int nChannel, AUDIO_PARAM *param);
int setAudioOutAttrParam(int nChannel, AUDIO_PARAM *param);
int getAudioEncParam(int nChannel, AENC_PARAM *param);
int setAudioEncParam(int nChannel, AENC_PARAM *param);
int getVideoLostAlarmParam(int nChannel, VIDEO_LOST_ALARM_PARAM *param);
int setVideoLostAlarmParam(int nChannel, VIDEO_LOST_ALARM_PARAM *param);
int getVideoMotionAlarmParam(int nChannel, VIDEO_MOTION_ALARM_PARAM *param);
int setVideoMotionAlarmParam(int nChannel, VIDEO_MOTION_ALARM_PARAM *param);
int getProbeInAlarmParam(int nChannel, PROBE_IN_ALARM_PARAM *param);
int setProbeInAlarmParam(int nChannel, PROBE_IN_ALARM_PARAM *param);
int getProbeOutAlarmParam(int nChannel, PROBE_OUT_ALARM_PARAM *param);
int setProbeOutAlarmParam(int nChannel, PROBE_OUT_ALARM_PARAM *param);
int getFtpParam(FTP_PARAM *param);
int setFtpParam(FTP_PARAM *param);
//add code by lvjh
int getRtspParam(DVSNET_RTSP_PARAM *param);
int setRtspParam(DVSNET_RTSP_PARAM *param);


int setIrProbeAlarmStartParam(void);
int setIrProbeAlarmStopParam(void);
int getIrProberAlarmParam(void);

int getTftpParam(TFTP_PARAM *param);
int setTftpParam(TFTP_PARAM *param);
int getEmailParam(EMAIL_PARAM *param);
int setEmailParam(EMAIL_PARAM *param);
int getJpegIPParam(char *param);
int setJpegIPParam(char *param);
int getNetParam(NET_PARAM *param);
int setNetParam(NET_PARAM *param);
int getPPPOEParam(PPPOE_PARAM *param);
int setPPPOEParam(PPPOE_PARAM *param);
int getDDNSParam(DDNS_PARAM *param);
int setDDNSParam(DDNS_PARAM *param);
int getYIYUAN_DDNSParam(YIYUAN_ALARM_DDNS_PARAM *param);
int setYIYUAN_DDNSParam(YIYUAN_ALARM_DDNS_PARAM *param);
int getAlarmStatusParam(ALARM_STATUS_PARAM *param);
int setAlarmStatusParam(ALARM_STATUS_PARAM *param);
int getTimeRebootParam(TIME_REBOOT_PARAM *param);
int setTimeRebootParam(TIME_REBOOT_PARAM *param);


int getRemoteConnectParam(REMOTE_CONNECT_PARAM *param);
int setRemoteConnectParam(REMOTE_CONNECT_PARAM *param);
int setRemoteP2PConnectParam(DVSNET_REQCONBACK_INFO *param);
int getRemoteP2PConnectParam(DVSNET_REQCONBACK_INFO *param);
int setP2PBACKParam(DVSNET_ENDCONBACK_INFO *param);

int getPtzParam(int nChannel, PTZ_PARAM *param);
int setPtzParam(int nChannel, PTZ_PARAM *param);
int getRs485Param(int nChannel, COM_PARAM *param);
int setRs485Param(int nChannel, COM_PARAM *param);
int getRs232Param(int nChannel, COM_PARAM *param);
int setRs232Param(int nChannel, COM_PARAM *param);
int getRemoteTalkIPParam(char *param);
int setRemoteTalkIPParam(char *param);
int setTimeParam(DATE_PARAM param);
int getUpnpParam(UPNP_PORTMAPPING_INFO *param);
int setUpnpParam(UPNP_PORTMAPPING_INFO *param);
int getNewUpnpParam(DVSNET_UPNP_PARAM *param);
int setNewUpnpParam(DVSNET_UPNP_PARAM *param);
int getP2PParam(DVSNET_P2P_PARAM *param);
int setP2PParam(DVSNET_P2P_PARAM *param);
int setP2PRegisterParam(DVSNET_REGISTER_INFO* param);
int getWifiParam(WIFI_PARAM *param);
int setWifiParam(WIFI_PARAM *param);
int getVideoFlipParam(int nChannel, VIDEO_FLIP_PARAM *param);
int setVideoFlipParam(int nChannel, VIDEO_FLIP_PARAM *param);
int getVideoMirrorParam(int nChannel, VIDEO_MIRROR_PARAM *param);
int setVideoMirrorParam(int nChannel, VIDEO_MIRROR_PARAM *param);
int getVideoHzParam(int nChannel, VIDEO_HZ_PARAM *param);
int setVideoHzParam(int nChannel, VIDEO_HZ_PARAM *param);
int getAudioInPathParam(int nChannel, AUDIO_PATH_PARAM *param);
int setAudioInPathParam(int nChannel, AUDIO_PATH_PARAM *param);
int get3gParam(G3_PARAM *param);
int set3gParam(G3_PARAM *param);
int getScheduleSnapshotParam(int nChannel, SCHEDULE_SNAPSHOT_PARAM *param);
int setScheduleSnapshotParam(int nChannel, SCHEDULE_SNAPSHOT_PARAM *param);
int getPtzAutoCtrlParam(int nChannel, PTZ_AUTO_CTRL *param);
int setPtzAutoCtrlParam(int nChannel, PTZ_AUTO_CTRL *param);

int getParamFromFlash();

int saveParamToFlash();
int getParamStatusFromFlash();
int setParamStatusToFlash(int status);
int getBootStausFromFlash();
int setBootStatusToFlash(int status);

int saveParamToFile();
int getParamFromFile();
int getParamStatusFromFile();
int setParamStatusToFile(int status);
int getBootStausFromFile();
int setBootStatusToFile(int status);

// Add the code by lvjh, 2008-04-07
int setNetworkConfigure(int param);
int getNetworkConfigure();
int setDebugConfigure(int param);
int getDebugConfigure();
int setIDConfigure(char *param);
int getIDConfigure(char *param);
int setMACConfigure(char *param);
int getMACConfigure(char *param);
//add code by lvjh
int setRtspPortConfigure(int  param);
void writeConfigFile(char *filePath, int value, int defaultValue);
int setUSERConfigure(USER_INFO_PARAM *param);
int getUSERP2PPORTConfigure(UPNP_PORT_INFO *param);
int setP2PConfigure(P2PSERVER *P2P);
int getP2PConfigure(P2PSERVER *P2P);


#ifdef RECORD
int getRecordSDKParam(RECORD_SETUP *setup);
int getRecordParam(int nChannel, RECORD_PARAM *param);
int setRecordParam(int nChannel, RECORD_PARAM *param);
int getTimerRecordParam(int nChannel, TIMER_RECORD_CHANNEL_PARAM *param);
int setTimerRecordParam(int nChannel, TIMER_RECORD_CHANNEL_PARAM *param);
int getVideoMotionRecordParam(int nChannel, VIDEOMOTION_RECORD_CHANNEL_PARAM *param);
int getProbeRecordParam(int nChannel, DETECTOR_RECORD_CHANNEL_PARAM *param);
#define debugPrintf() {printf("FILE(%s) FUNC(%s):%d .\n",\
								__FILE__, __func__ , __LINE__); return;}
#endif

#endif

