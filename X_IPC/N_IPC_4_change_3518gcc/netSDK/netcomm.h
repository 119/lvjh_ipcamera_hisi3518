//����Э���ʽ:
//	NET_HEADER+DATA;
//	1.��½
//		C: NET_HEAD+USER_INFO;
//		S: NET_HEAD+SERVER_INFO/NULL(ʧ��)
//	2.������ͨ��
//		C: NET_HEAD+OPEN_CHANNEL
//		S: NET_HEAD(�ɹ���ʧ�ܾ��᷵��)
//	3.�Խ�����
//		C: NET_HEAD;
//		S: NET_HEAD(�ɹ���ʧ�ܾ��᷵��)
//	4.��֧�ֵ�����
//		C: NET_HEAD;
//		S: NET_HEAD(ʧ�ܻ᷵��)
//	5.��������(ÿ3��һ��)
//		C: NET_HEAD;
//		S: NET_HEAD;
//	6.���ݶಥ����
//		C: NET_HEAD;
//		S: NET_HEAD;
//	7.�ر�����ͨ��
//		C: NET_HEAD;
//		S: NET_HEAD;
//	8.����
//		C: NET_HEAD;
//		S: NET_HEAD;
//	9.TCP��������
//		C: NET_HEAD;
//		S: NET_HEAD;
//	10.TCP�Խ���������
//		C: NET_HEAD;
//		S: NET_HEAD+TALK_PARAM;
//	11.�������ݰ�
//		S: NET_HEAD+DATA_PACKET;

#ifndef __NET_COMM_H_
#define __NET_COMM_H_

#define HDVS_FLAG						0x53564448
#define HSS_HDVS_FLAG					0X123AB678
#define HSS_HDVS_FLAG2					0X876CD321

#define NET_CMD_LINK					0x01
#define NET_CMD_PTZCTRL					0x06
#define NET_ERR_USEROVER				-103



 
#define MIN_SOCK_PORT					1024
#define MAX_SOCK_PORT					65535

#define USER_NAME_LEN					32	
#define USER_PSW_LEN					32

#define NETCMD_BUF_SIZE					2048

#define PACK_SIZE						1300
//#define NETCMD_MAX_SIZE					102400
#define NETCMD_MAX_SIZE					512000	// Change the code by lvjh, 2009-12-07
#define NETCMD_SNDFILE_SIZE				51200
#define NETCMD_TALKBACK_SIZE			51200

//��������
#define NETCMD_BASE						0x8000
#define NETCMD_LOGON					NETCMD_BASE+1
#define NETCMD_LOGOFF					NETCMD_BASE+2
#define NETCMD_GET_AV_INFO				NETCMD_BASE+3
#define NETCMD_OPEN_CHANNEL				NETCMD_BASE+4
#define NETCMD_CLOSE_CHANNEL			NETCMD_BASE+5
#define NETCMD_OPEN_MULTI_CHANNEL		NETCMD_BASE+6
#define NETCMD_CLOSE_MULTI_CHANNEL		NETCMD_BASE+7
#define NETCMD_OPEN_TALK				NETCMD_BASE+8
#define NETCMD_CLOSE_TALK				NETCMD_BASE+9
#define NETCMD_NAT_CONNECT				NETCMD_BASE+10
#define NETCMD_OPEN_CHANNEL_R			NETCMD_BASE+11
#define NETCMD_CLOSE_CHANNEL_R			NETCMD_BASE+12
#define NETCMD_OPEN_TALK_R				NETCMD_BASE+13
#define NETCMD_CLOSE_TALK_R				NETCMD_BASE+14
#define NETCMD_KEEP_ALIVE				NETCMD_BASE+15
#define NETCMD_UPDATE					NETCMD_BASE+16
#define NETCMD_SERVER_MSG				NETCMD_BASE+17		//�������������͵���Ϣ
#define NETCMD_DEV_DATA					NETCMD_BASE+18
#define NETCMD_AV_DATA					NETCMD_BASE+19
#define NETCMD_TALK_DATA				NETCMD_BASE+20
#define NETCMD_REC_FILE					NETCMD_BASE+21
#define NETCMD_UPDATE_PROCESS			NETCMD_BASE+22

#define NETCMD_USER_BASE				NETCMD_BASE+0x500
#define NETCMD_USER_CMD					NETCMD_USER_BASE+0x01
#define NETCMD_P2P_CMD					(NETCMD_USER_BASE+2)
#define NETCMD_P2P_BASE					0x60000
#define NETCMD_P2P_REGDATAPORT			(NETCMD_P2P_BASE+1)
#define NETCMD_P2P_REQHOLE				(NETCMD_P2P_BASE+3)	//PC������IPC�� PC---->P2P Server----->IPC
#define NETCMD_P2P_ENDHOLE				(NETCMD_P2P_BASE+4) //����� IPC----->P2P Server----->PC
#define NETCMD_P2P_CMDRECEIVED			(NETCMD_P2P_BASE+301)	//ȷ���յ�����


#define NETCMD_P2P_REQCONBACK			(NETCMD_P2P_BASE+5)
#define NETCMD_P2P_ENDCONBACK			(NETCMD_P2P_BASE+6)
#define GET_REMOTE_P2P_CONNECT_PARAM	(NETCMD_P2P_BASE+7)
#define NETERR_TALK_OPENED				 NETERR_BASE+9	//�Խ��ѱ������û�����


//�û���Ϣ

//����������
#define NETERR_BASE					0x9000
#define NETERR_NOT_USER				NETERR_BASE+1	//�û�������
#define NETERR_USER_PSW				NETERR_BASE+2	//�û��������
#define NETERR_LOGON_FULL				NETERR_BASE+3	//�û���½����
#define NETERR_CHANNEL_FULL			NETERR_BASE+4	//ͨ��������
#define NETERR_NOT_HDVS_DATA			NETERR_BASE+5	//����HIDVS�����ݰ�
#define NETERR_NOT_SUPPORT			NETERR_BASE+6	//��֧�ֵ�����
#define NETERR_ILLEGAL_PARAM			NETERR_BASE+7	//�Ƿ����������
#define NETERR_NOT_LOGON				NETERR_BASE+8	//�û�û�е�½
#define NETERR_TALK					NETERR_BASE+9	//�û�û�е�½
#define NETERR_NOT_SUPPORT_PROTOCAL	NETERR_BASE+10	//��֧�ֵ�����Э��
#define NETERR_NOT_RIGHT			NETERR_BASE+11	

//����֡ͷ
typedef struct
{
	unsigned long nFlag;			//��־
	unsigned long nCommand;		//����
	unsigned long nErrorCode;		//������,�ɹ�ֻ����0
	unsigned long nBufSize;		//���ݵĴ�С
	unsigned long nReserve;		//����
	
}NET_HEAD, *PNET_HEAD;


//�Ͱ���Ѷ ��ϢSOCKET����ͷ
typedef struct
{
	int   pMask1;		//MSGHEAD_MASK1
	char  sername[24];	    //����������
	short nTypeMain;      //��ϢID�����ݲ�ͬID��,��������ݲ�ͬ
	short nTypeSub;       //���ӱ���������ʱ�����жϷ���ֵ.
	short nTypeThird;      //���ӱ���
	short nTypeFour;       //���ӱ���
	int   nChannel;       //ͨ��
	int	  dwDataSize;    //���ݳ��ȣ�����������ͷ����
	int   pMask2;		//MSGHEAD_MASK2
}NET_MSG_HEAD;

typedef struct
{
	char lpUserName[20];             //�û���
	char lpPassword[20];              //����
}NETSDK_USER;

 typedef struct
{
	int server_sock;			           //�豸���ص�����ֵ
	int rights;					           //�û�Ȩ��
}NETSDK_USERINFO;				   //�û����Ӻ󷵻صĽṹ��Ϣ




//����ͷ
typedef struct tagCOMMAND_HEAD
{
	unsigned long	nCmdLen;
	unsigned long	nCmdID;
	unsigned long	nChannel;
	unsigned long	nReserve;
}COMMAND_HEAD;



// �û���Ϣ
typedef struct _USER_INFO
{
	char szUserName[USER_NAME_LEN];
	char szUserPsw[USER_PSW_LEN];
	char szServerName[USER_PSW_LEN];	// add the code by zhb, 2007-08-18
}NET_USER_INFO, *PNET_USER_INFO;

//��������
typedef struct
{
	NET_HEAD netHead;
	char pBuf[NETCMD_BUF_SIZE];
}NET_DATA, *PNET_DATA;			//�ڲ�ʹ��

//����Э�������
typedef enum
{
	PROTOCOL_TCP = 0,
	PROTOCOL_UDP = 1,
	PROTOCOL_MULTI = 2
}PROTOCOL_TYPE;

//��ͨ���Ĳ���
typedef struct
{
	unsigned long nID;
	PROTOCOL_TYPE nProtocolType;	//�򿪷�����ͨ��������Э��
	unsigned long nStreamType;		//����˫�����ϣ�0�������룬1��С����
	unsigned long nSerChn;			//������ͨ��
	unsigned long nClientID;		// add the code by zhb, 2007-08-18
	
}OPEN_CHANNEL, *POPEN_CHANNEL;

//AV��Ϣ
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
    
}AV_INFO, *PAV_INFO;

//����Ƶ
typedef struct
{
	unsigned long nTimeTick;		//ʱ���
	unsigned long nVideoSize;		//��Ƶ���ݴ�С
	unsigned short nAudioSize;		//��Ƶ���ݴ�С
	unsigned short nImageWidth;		//��Ƶ����
	unsigned short nImageHeight;	//��Ƶ���
	//unsigned short bKeyFrame;		//��ƵI(�ؼ�)֡��־
	// Add the code by lvjh, 2009-04-24
	unsigned char bKeyFrame;		//��ƵI(�ؼ�)֡��־
	unsigned char nReserve;			// unsigned char bKeyFrame;
    
}AV_FRAME_HEAD, *PAV_FRAME_HEAD;

//���ݰ�
typedef struct
{
	unsigned char bIsDataHead;			//���ݵ�ͷ
	unsigned char nChannel;				//AV����ͨ��
	unsigned short nBufSize;			//���ݵĴ�С
	unsigned long byPacketID;			//AV�������к�
	AV_FRAME_HEAD FrameHeader;			//AV֡ͷ
	unsigned char PackData[PACK_SIZE];	//AV����
}DATA_PACKET, *PDATA_PACKET;			//���͡�����ʱ������/��BufSize��PackData

//�������ݰ�ͷ
typedef struct _NET_DATA_HEAD
{
	unsigned long nFlag;
	unsigned long nSize;
}NET_DATA_HEAD, *PNET_DATA_HEAD;

//�������ݰ�
typedef struct _NET_DATA_PACKET
{
	NET_DATA_HEAD packHead;
	DATA_PACKET packData;
}NET_DATA_PACKET, *PNET_DATA_PACKET;

//�����ļ������ݰ�ͷ
typedef struct _NET_UPDATE_FILE_HEAD
{
	char strFileName[32];
	char strFilePath[128];
	unsigned long nFileLen;
	unsigned long nFileOffset;
	unsigned long nBlockNum;
	unsigned long nBlockNo;
	unsigned long nBlockSize;
	unsigned long nFileCRC;
	unsigned long nReserve;
}NET_UPDATE_FILE_HEAD;

// �Խ�����
typedef struct _TALK_PARAM
{
	unsigned long nEncType;		//
	unsigned long nAinChnNum;	//
	unsigned long nAinBits;		//
	unsigned long nAinSamples;	//
	unsigned long nDecType;		//
	unsigned long nAoutChnNum;	//
	unsigned long nAoutBits;	//
	unsigned long nAoutSamples;	//
}TALK_PARAM;

#endif
