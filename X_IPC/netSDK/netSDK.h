#ifndef __NET_SDK_H_
#define __NET_SDK_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <pthread.h>

#include "netcomm.h"

// ������������ͨ��

#ifdef QUAD_CHANNEL
	#define SERVER_MAX_CHANNEL		4
#else
	#define SERVER_MAX_CHANNEL		1
#endif

#define TRUE						1
#define FALSE						0
#define BOOL						int
#define WORD						unsigned short
#define DWORD						unsigned long
#define BYTE						unsigned char
#define __int64						unsigned long long

#define SOCK_SNDRCV_LEN				(1024*512)
#define SOCK_TIME_OUT				5
//#define P2P_SOCK_TIME_OUT			3
#define P2P_SOCK_TIME_OUT			60*3

#define MAX_POOL_SIZE				1024


#define UPD_RCV_BUFFER_LEN          (1024*256)
#define TCP_RCV_BUFFER_LEN          (1024)

#define TCP_FLAG					0
#define UDP_FLAG					1


#define MAX_USER_CMD_CHANNEL_NUM	20

#define MAX_USER_DATA_CHANNEL_NUM	MAX_USER_CMD_CHANNEL_NUM	// ����û�����ͨ����

#define MAX_UPDATE_FILE_LEN			8388608

#define  WAVE_FORMAT_ALAW       0x0006  /*  Microsoft Corporation  */
#define  WAVE_FORMAT_MULAW      0x0007  /*  Microsoft Corporation  */


typedef enum
{
	READY = 0,
	RUN = 1,
	STOP = 2
}NODE_STATUS;

typedef struct _MSG_HEAD
{
	int nSock;						// �ͻ�������������SOCKET
	int nCmd;						// NET����
	int nRight;						// �ͻ���Ȩ��
	int nErrorCode;					// �������
	int nBufSize;					// ��������BUFFER���ݵĴ�С
	int nflag;						// UDP TCP �����ʾ
	struct sockaddr_in addr;        // UDP addr;
	
    
}MSG_HEAD,*PMSG_HEAD;

// �������ݵĽڵ�
typedef struct _SEND_NODE
{
	PROTOCOL_TYPE nType;			// ����Э������
	unsigned int nStreamType;		// �������ͣ���������С����
	unsigned int nSerChn;			// Ҫ�򿪵ķ����ͨ����
	unsigned int nTcpAddr;			// �ͻ��˵�IP��ַ
	unsigned int nTcpCmdSock;		// ����ͨ��SOCKET
	unsigned int hTcpDataSock;		// TCP����ͨ��SOCKET
	unsigned int nUdpDataIP;		// UDP�Ͷಥ��IP
	unsigned int nUdpDataPort;		// UDP�Ͷಥ�Ķ˿�
	unsigned int nClientID;			// �ͻ�Ψһ��ID,	// Add the code by lvjh, 2008-03-28
	NODE_STATUS	status;				// ״̬
	
	struct _SEND_NODE *pNext;
	
}SEND_NODE, *PSEND_NODE;

typedef struct _READ_POSITION
{
	int nBegin;
	int nEnd;
	BOOL bLost;
}READ_POSITION, *PREAD_POSITION;

typedef struct _CHANNEL_STREAM
{
	int nPoolSize;
	int nWritePos;

	pthread_mutex_t hReadBlockMutex;
	pthread_cond_t  hReadBlockCond;
  	pthread_mutex_t hReadWriteLock;

	NET_DATA_PACKET *pPackPool[MAX_POOL_SIZE];
}CHANNEL_STREAM, *PCHANNEL_STREAM;

typedef struct _UDP_STREAM_SEND
{
	pthread_t hUdpSendThreadID[SERVER_MAX_CHANNEL];
	SEND_NODE *pUdpSendList[SERVER_MAX_CHANNEL];
	pthread_mutex_t hUdpSendListMutex[SERVER_MAX_CHANNEL];
}UDP_STREAM_SEND, *PUDP_STREAM_SEND;

#if 0
typedef struct _CLIENT_INFO
{
	unsigned int ip;
	unsigned int port;
	char szUserName[USER_NAME_LEN];
	char szUserPsw[USER_PSW_LEN];

	unsigned int level;
	unsigned int hMsgSocket;

	unsigned int nKeepAlive;
	NODE_STATUS status;
	struct _CLIENT_INFO *pNext;
}CLIENT_INFO, *PCLIENT_INFO;
#endif

typedef struct _CLIENT_INFO
{
	unsigned int ip;
	unsigned int port;
	unsigned int nflag;
	char szUserName[USER_NAME_LEN];
	char szUserPsw[USER_PSW_LEN];

	unsigned int level;
	unsigned int hMsgSocket;
	struct sockaddr_in addr;

	unsigned int nKeepAlive;
	NODE_STATUS status;
	struct _CLIENT_INFO *pNext;
}CLIENT_INFO, *PCLIENT_INFO;



typedef struct _CLIENT_LIST
{
	unsigned int nTotalNum;
	pthread_mutex_t hClientMutex;
	CLIENT_INFO *pNext;
}CLIENT_LIST, *PCLIENT_LIST;

typedef struct _TALKTHRD_PARAM
{
	int hSock;
	unsigned int ip;
	unsigned int port;
	struct sockaddr_in addr;
	int nflag;
}TALKTHRD_PARAM;

typedef struct _RECTHRD_PARAM
{
	int hSock;
	unsigned int ip;
	unsigned int port;
	struct sockaddr_in addr;
	char buffer[UPD_RCV_BUFFER_LEN];
}RECTHRD_PARAM;

//typedef int (*CheckUserPsw)(const char *pUserName, const char *pPsw);
typedef int (*CheckUserPsw)(MSG_HEAD *pMsgHead, const char *pUserName, const char *pPsw);
typedef int (*CheckUdpUserPsw)(MSG_HEAD *pMsgHead, const char *pUserName, const char *pPsw, struct sockaddr_in addr);

typedef int (*ServerRecv)(MSG_HEAD *pMsgHead, char *pRecvBuf);
typedef int (*ClientRequestTalk)(unsigned long ip, unsigned short port);
typedef int (*ClientUdpRequestTalk)(unsigned long ip, unsigned short port,struct sockaddr_in addr);

typedef int (*ClientStreamTalk)(unsigned long ip, unsigned short port, char *pbuf, int len);
typedef int (*ClientUdpStreamTalk)(unsigned long ip, unsigned short port, char *pbuf, int len, struct sockaddr_in addr);

typedef int (*FileTransfer)(int sockFd);
typedef int (*UdpFileTransfer)(int sockFd, struct sockaddr_in addr, char *buffer);


typedef struct _SER_INFO
{
	unsigned int nChnNum;		//������ͨ����
	unsigned int nBasePort;		//�������˿ں�
	char szMultiIP[16];			//�ಥIP
	unsigned int multiAddr;		//�ಥIP

	AV_INFO avInfoStream1[SERVER_MAX_CHANNEL];	//��һ����
	AV_INFO avInfoStream2[SERVER_MAX_CHANNEL];	//�ڶ�����

	unsigned int nSendBufferNum;			//���ͻ���BUFFER�ߴ�

	int hTcpListenSock;						//TCP����SOCKET
	int hUdpListenSock;						//UDP����SOCKET

	pthread_t hTcpListenThread;				//TCP�����߳�ID
	pthread_t hUdpListenThread;				//UDP�����߳�ID

	SEND_NODE *pTcpSendList;				//TCP���������̳߳�
	pthread_mutex_t hTcpSendListMutex;		//TCP���������̳߳���

	UDP_STREAM_SEND udpStreamSend1;			//UDP��������1
	UDP_STREAM_SEND udpStreamSend2;			//UDP��������2

	CHANNEL_STREAM *pChannelStream1[SERVER_MAX_CHANNEL];	//TCP��������1
	CHANNEL_STREAM *pChannelStream2[SERVER_MAX_CHANNEL];	//TCP��������2

	char *pSendBuf;					//����AV���ݹ���BUFFER
	pthread_mutex_t sendBufMutex;	//����AV���ݹ���BUFFER����
    
	char *pRecvBuf;					//����AV���ݹ���BUFFER
	char *pUpdateFileBuf;			//�����ļ�����BUFFER
	int nCurUpdateSock;				//��ǰ�û�����SOCKET
	int nUpdateFlag;				//������־��������û����������������û���������������ֱ�����û��������

	pthread_key_t hReadPosKey;		//

	pthread_t msgThreadID[MAX_USER_CMD_CHANNEL_NUM];	//�����߳�ID
	pthread_mutex_t msgThreadMutex;			//�����߳���
	pthread_cond_t msgThreadCond;			//�����߳�����

	int msgProcessThreadNum;				//Ҫ����������߳���
	int msgWaitThreadNum;					//�ȴ��������߳���
	int msgQuitThreadNum;					//Ҫ�˳��������߳���

	pthread_t msgQuitThread;				//���������˳����߳�ID
	pthread_mutex_t msgQuitThreadMutex;		//���������˳��߳���
	pthread_cond_t msgQuitThreadCond;		//���������˳��߳�����

	pthread_t dataThreadID[MAX_USER_DATA_CHANNEL_NUM];	//�����߳�ID
	pthread_mutex_t dataThreadMutex;		//�����߳���
	pthread_cond_t dataThreadCond;			//�����߳�����

	int dataProcessThreadNum;				//Ҫ����������߳���
	int dataWaitThreadNum;					//�ȴ��������߳���
	int dataQuitThreadNum;					//Ҫ�˳��������߳���

	pthread_t dataQuitThread;				//���������˳����߳�ID
	pthread_mutex_t dataQuitThreadMutex;	//���������˳��߳���	
	pthread_cond_t dataQuitThreadCond;		//���������˳��߳�����

	CheckUserPsw funcCheckUserPsw;			//�û�У��ص�����
	CheckUdpUserPsw funcUdpCheckUserPsw;    //UDP�û�У��ص�����
	ServerRecv funcServerRecv;				//ҵ����ص�����
	
	FileTransfer funcFileTransfer;			//�ļ�����
	UdpFileTransfer UdpFuncFileTransfer;	//�ļ�����udp
	

	int bServerStart;							//����SDK���ñ�־
	int bServerExit;						//����SDK�˳���־
	int nMsgThreadCount;					//�����̳߳ؼ���
	int nDataThreadCount;					//�����̳߳ؼ���
    	
	int nAudioChannels;						//�Խ���Ƶͨ����
	int nAudioBits;							//�Խ���Ƶ����λ
	int nAudioSamples;						//�Խ���Ƶ������	
	ClientRequestTalk pCallbackRequestTalk;	//�Խ�����ص���
	ClientUdpRequestTalk pUdpCallbackRequestTalk;	//UDP�Խ�����ص�����
	
	ClientStreamTalk pCallbackStreamTalk;	//�Խ���Ƶ��������ص�����
	ClientUdpStreamTalk pUdpCallbackStreamTalk;	//UDP�Խ���Ƶ��������ص�����
	
	int hTalkbackSocket;					//�Խ���SOCKET
	int hUdpTalkbackSocket; 				//udp �Խ���SOCKET
	struct sockaddr_in addr ;
	int nupdate_flag;
	int nflag;
	int curReadPos;
	FILE *hFile;
	char cur_file_name[128];
	int size;
	unsigned long	dwUserID;
}NET_SER_INFO, *PNET_SER_INFO;

// ������Ϣ
#ifdef NET_SDK_DEBUG
	#define net_debug()\
	{             \
		printf(" %s(%d) - %d , %s \n", __FILE__, __LINE__, errno, strerror(errno));\
	}
#else
	#define net_debug()
#endif

// ȫ�ֱ���
NET_SER_INFO g_server_info;
CLIENT_LIST g_client_list;
int g_talkbackSock;
char g_wire_ip[16];
char g_wireless_ip[16];

//����SDK��API
int NETSDK_ServerInit(int flag);
int NETSDK_ServerOpen();
int NETSDK_ServerSetup(int nChnNum, int nPort, char *pMultiIP, int bufSize);
int NETSDK_GetServerSetup(int *nChnNum, int *nPort, char *pMultiIP, int *bufSize);
int NETSDK_ServerStart();
int NETSDK_ServerStop();
int NETSDK_ServerClose();
int NETSDK_SendMsg(MSG_HEAD	*pMsgHead, char *pSendBuf);
int Udp_NETSDK_SendMsg(MSG_HEAD	*pMsgHead, char *pSendBuf, struct sockaddr_in addr);

int NETSDK_SendAllMsg(char *pMsgBuf, int nLen);
int NETSDK_SendSpecMsg(int nUserID, char *pMsgBuf, int nLen);
int NETSDK_GetUserId(unsigned long ipAddr);
int NETSDK_SetAVInfo1(int nChannel, AV_INFO *pAVInfo);
int NETSDK_SetAVInfo2(int nChannel, AV_INFO *pAVInfo);
int NETSDK_GetAVInfo1(int nChannel, AV_INFO *pAVInfo);
int NETSDK_GetAVInfo2(int nChannel, AV_INFO *pAVInfo);
int NETSDK_WriteOneFrame1(int nChannel, char *pFrame);
int NETSDK_WriteOneFrame2(int nChannel, char *pFrame);
int NETSDK_SetSendBufNum(int nBufNum);
int NETSDK_SetUserCheckFunc(CheckUserPsw pFunc);
int NETSDK_SetUdpUserCheckFunc(CheckUdpUserPsw pFunc);
int NETSDK_SetServerRecvFunc(ServerRecv	pFunc);
int NETSDK_SetFileTransferFunc(FileTransfer pFunc);
int NETSDK_SetFileUdpTransferFunc(UdpFileTransfer pFunc);

int NETSDK_GetUserLoginNum();
int NETSDK_GetOpenChannelNum();
int NETSDK_SetBandWidth(int nBandWidth);

int open_talk(void);
int close_talk(void);
int NETSDK_SetCallback(ClientRequestTalk  fun1,ClientStreamTalk fun2);
int UDPNETSDK_SetCallback(ClientUdpRequestTalk  fun1,ClientUdpStreamTalk fun2);

int NETSDK_SetTalkParam(int nChannel, int nBits, int nSamples);
int NETSDK_Talk_Begin(char *pszIP, unsigned short port);
int NETSDK_Talk_End();
int NETSDK_Talk_Send(char *pBuf, int nLen);
int UDPNETSDK_Talk_Send(char *pBuf, int nLen);
int NETSDK_Talk_Send_Ext(char *pBuf,int nLen);
int NETSDK_Talk_Start(int socket);
int UDPNETSDK_Talk_Start(int socket, struct sockaddr_in addr);
int NETSDK_Talk_Stop();

int NETSDK_NatSetup(char *remoteIP, int remotePort, int interval, char *data, int len);
int NETSDK_NatStart();
int NETSDK_NatStop();
int NETSDK_NatPause();
int NETSDK_NatResume();

int NETSDK_SetWireIP(char *addr);
int NETSDK_SetWirelessIP(char *addr);

#endif


