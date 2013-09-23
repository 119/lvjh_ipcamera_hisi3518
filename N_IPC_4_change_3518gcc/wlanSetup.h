#ifndef __WIFI_SETUP_H_
#define __WIFI_SETUP_H_

#ifndef MAX_IP_LEN
#define MAX_IP_LEN 16
#endif

typedef struct
{
	unsigned long nOnFlag;
	
	// Add the code by lvjh
	unsigned long nDhcpOnFlag;
	char byServerIp[MAX_IP_LEN];	//������IP��ַ
	char byServerMask[MAX_IP_LEN];	//��������
	char byGateway[MAX_IP_LEN];		//����
	char byDnsAddr[MAX_IP_LEN];		//DNS��ַ
	unsigned char strPhyAddr[6];	//MAC��ַ
	unsigned char pReserve[2];		//����

	// Basic Setup
	///////////////////////////////////
	// Mode:
	// 0: Infra
	// 1: Ad-hoc
	//////////////////////////////////
	unsigned long Mode;
	
	///////////////////////////////////
	// Authendication:
	// 0 : Open system
	// 1 : Shared key
	// 2 : Both
	// 3 : TKIP
	// 4 : AES
	// 0��1��2 - WEP  3��4-WPA
	///////////////////////////////////
	unsigned long Authen;
	
	///////////////////////////////////
	// Encryption:
	// 0 : Off
	// 1 : WEP
	// 2 : WPAPSK	 AP Only
	// 3 : WPA2PSK	 AP Only
	// 4 : WPANONE - Ad-hoc Only
	///////////////////////////////////
	unsigned long Encryption;
	
	///////////////////////////////////
	// SSID:
	///////////////////////////////////
	unsigned char SSID[20];
	
	// WEP SETUP
	///////////////////////////////////
	// WEP Format:
	// 0 : WEP 64  Ascii
	// 1 : WEP 128 Ascii
	// 2 : WEP 64  Hex
	// 3 : WEP 128 Hex
	///////////////////////////////////
	unsigned long WEPFormat;
	
	///////////////////////////////////
	// WEP key:
	///////////////////////////////////
	unsigned long WEPKeyChoose;
	
	///////////////////////////////////
	// 64bits WEP Key:
	///////////////////////////////////
	unsigned char WEPKey641[10];
	unsigned char WEPKey642[10];
	unsigned char WEPKey643[10];
	unsigned char WEPKey644[10];
	
	///////////////////////////////////
	// 128bits WEP Key:
	///////////////////////////////////
	unsigned char WEPKey1281[26];
	unsigned char WEPKey1282[26];
	unsigned char WEPKey1283[26];
	unsigned char WEPKey1284[26];
	
	// WPA Setup
	///////////////////////////////////
	// WPA Format:
	// 0 : Passphrase
	// 1 : Hex
	///////////////////////////////////
	unsigned long WPAFormat;
	
	///////////////////////////////////
	// WPA Pre-shared Key:
	///////////////////////////////////
	unsigned char WPAPSK[20];
	
	///////////////////////////////////
	//  Reserve for new parameter:
	///////////////////////////////////
	unsigned long Reserve;
}WIFI_PARAM;

/*
//WIFI����
typedef struct tagDVSNET_WIFI_PARAM
{
	unsigned long	bEnabled;
	unsigned long	bDHCP;										//DHCP���� 
	char			sServerIp[MAX_IP_LEN];						//������IP��ַ
	char			sServerMask[MAX_IP_LEN];					//��������
	char			sGateway[MAX_IP_LEN];						//����
	char			sDnsAddr[MAX_IP_LEN];						//DNS��ַ
	unsigned char	bPhyAddr[6];								//MAC��ַ(ֻ��)
	unsigned char	bReserve[2];								//����
	// Basic Setup
	///////////////////////////////////
	// Mode:
	// 0: Infra
	// 1: Ad-hoc
	//////////////////////////////////
	unsigned long Mode;
	
	///////////////////////////////////
	// Authendication:
	// 0 : Open system
	// 1 : Shared key
	// 2 : Both
	// 3 : TKIP
	// 4 : AES
	//0��1��2 - WEP  3��4-WPA
	///////////////////////////////////
	unsigned long Authen;
	
	///////////////////////////////////
	// Encryption:
	// 0 : Off
	// 1 : WEP
	// 2 : WPAPSK	 AP Only
	// 3 : WPA2PSK	 AP Only
	// 4 : WPANONE - Ad-hoc Only
	///////////////////////////////////
	unsigned long Encryption;
	
	///////////////////////////////////
	// SSID:
	///////////////////////////////////
	unsigned char SSID[20];
	
	// WEP SETUP
	///////////////////////////////////
	// WEP Format:
	// 0 : WEP 64  Ascii
	// 1 : WEP 128 Ascii
	// 2 : WEP 64  Hex
	// 3 : WEP 128 Hex
	///////////////////////////////////
	unsigned long WEPFormat;
	
	///////////////////////////////////
	// WEP key:
	///////////////////////////////////
	unsigned long WEPKeyChoose;
	
	///////////////////////////////////
	// 64bits WEP Key:
	///////////////////////////////////
	unsigned char WEPKey641[10];
	unsigned char WEPKey642[10];
	unsigned char WEPKey643[10];
	unsigned char WEPKey644[10];
	
	///////////////////////////////////
	// 128bits WEP Key:
	///////////////////////////////////
	unsigned char WEPKey1281[26];
	unsigned char WEPKey1282[26];
	unsigned char WEPKey1283[26];
	unsigned char WEPKey1284[26];
	
	// WPA Setup
	///////////////////////////////////
	// WPA Format:
	// 0 : Passphrase
	// 1 : Hex
	///////////////////////////////////
	unsigned long WPAFormat;
	
	///////////////////////////////////
	// WPA Pre-shared Key:
	///////////////////////////////////
	unsigned char WPAPSK[20];
	
	///////////////////////////////////
	//  Reserve for new parameter:
	///////////////////////////////////
	unsigned long Reserve;
}DVSNET_WIFI_PARAM;
*/

int wifi_setup(WIFI_PARAM param);


#endif
