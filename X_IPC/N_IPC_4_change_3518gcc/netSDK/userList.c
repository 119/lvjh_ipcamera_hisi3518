#include "userList.h"

int printf_bytes(char *buffer, int count)
{
	int i = 0;
	printf("\n");
	
	for (i=0; i<count; i++)
	{
		printf("%x ", buffer[i]);
	}

	printf("\n");
}

int UdpReceive(int hSock, char *precvBuf, int nSize, struct sockaddr_in addr)
{
	int ret = 0;
	fd_set fset;
	struct timeval to;
	//struct sockaddr_in addr;
	int nLen = 0;
	int left = 0;
	int flag = 0;

	printf("UdpReceive ...\n");

	memset(&to, 0, sizeof(to));

	// �������У��
	if (hSock<=0 || hSock>65535)
	{
		return -1;
	}
	if (precvBuf == NULL)
	{
		return -1;
	}
	if (nSize <= 0)
	{
		return -1;
	}

	while (left < nSize)
	{
		FD_ZERO(&fset);
		FD_SET(hSock, &fset);
		to.tv_sec = SOCK_TIME_OUT*2;
		to.tv_usec = 0;

		// SOCKET���ճ�ʱ�����,���˳�
		ret = select(hSock+1, &fset, NULL, NULL, &to);
		if ( ret == 0)
		{
			net_debug();
			return 0;
		}
		if (ret<0 && errno==EINTR)
		{
			net_debug();
			return 0;
		}
		if (ret < 0)
		{
			net_debug();
			return -1;
		}

		if (!FD_ISSET(hSock, &fset))
		{
			net_debug();
			return -1;
		}
		
		// ��������
		nLen = sizeof(addr);
		//ret = recv(hSock, precvBuf + left, nSize - left, 0);
		ret = recvfrom(hSock, precvBuf + left, nSize - left, MSG_WAITALL, (struct sockaddr*)&addr, (socklen_t *)&nLen);
		
		// add the code by lvjh, 2009-08-06
		if (flag > 20)
		{
			net_debug();
			return -1;
		}
		
		if ( (ret < 0) && (errno == EAGAIN || errno == EINTR))
		{
			flag++;
			net_debug();
			continue;
		}
		if (ret == 0 && errno == 115)
		{
			flag++;
			net_debug();
			continue;
			//return -1;
		}

		//if (ret < 0)
		//{
		//	printf("Socket(%d): %s %d\n", hSock, __FILE__, __LINE__);
		//	net_debug();
		//	return -1;
		//}
		
		if (ret <= 0)
		{
			net_debug();
			return -1;
		}
		
		left += ret;
	}
	
	//printf("TcpReceive OK\n");

	return left;
}

// TCP����

int TcpReceive(int hSock, char *precvBuf, int nSize)
{
	int ret = 0;
	fd_set fset;
	struct timeval to;
	int left = 0;
	int flag = 0;

	//printf("TcpReceive ...\n");

	memset(&to, 0, sizeof(to));

	// �������У��
	if (hSock<=0 || hSock>65535)
	{
		return -1;
	}
	if (precvBuf == NULL)
	{
		return -1;
	}
	if (nSize <= 0)
	{
		return -1;
	}

	while (left < nSize)
	{
		FD_ZERO(&fset);
		FD_SET(hSock, &fset);
		to.tv_sec = SOCK_TIME_OUT;
		to.tv_usec = 0;

		// SOCKET���ճ�ʱ�����,���˳�
		ret = select(hSock+1, &fset, NULL, NULL, &to);
		if ( ret == 0)
		{
			printf("TCP RECEIVE TIMEOUT... \n");
			net_debug();
			return 0;
		}
		if (ret<0 && errno==EINTR)
		{
			net_debug();
			return 0;
		}
		if (ret < 0)
		{
			net_debug();
			return -1;
		}

		if (!FD_ISSET(hSock, &fset))
		{
			net_debug();
			return -1;
		}
		
		// ��������
		ret = recv(hSock, precvBuf + left, nSize - left, 0);
		
		// add the code by lvjh, 2009-08-06
		if (flag > 20)
		{
			net_debug();
			return -1;
		}
		
		if ( (ret < 0) && (errno == EAGAIN || errno == EINTR))
		{
			flag++;
			net_debug();
			continue;
		}
		if (ret == 0 && errno == 115)
		{
			flag++;
			net_debug();
			continue;
			//return -1;
		}

		//if (ret < 0)
		//{
		//	printf("Socket(%d): %s %d\n", hSock, __FILE__, __LINE__);
		//	net_debug();
		//	return -1;
		//}
		
		if (ret <= 0)
		{
			net_debug();
			return -1;
		}
		
		left += ret;
	}
	
	//printf("TcpReceive OK\n");

	return left;
}


/*
int TcpReceive(int hSock, char *precvBuf, int nSize)
{
	int ret = 0;
	fd_set fset;
	struct timeval to;

	memset(&to, 0, sizeof(to));

	// �������У��
	if (hSock<=0 || hSock>65535)
	{
		return -1;
	}
	if (precvBuf == NULL)
	{
		return -1;
	}
	if (nSize <= 0)
	{
		return -1;
	}

	FD_ZERO(&fset);
	FD_SET(hSock, &fset);
	to.tv_sec = SOCK_TIME_OUT*2;
	to.tv_usec = 0;

	//printf("TcpReceive ...\n");

	// SOCKET���ճ�ʱ�����,���˳�
	ret = select(hSock+1, &fset, NULL, NULL, &to);
	if ( ret == 0)
	{
		net_debug();
		return 0;
	}
	if (ret<0 && errno==EINTR)
	{
		net_debug();
		return 0;
	}
	if (ret < 0)
	{
		net_debug();
		return -1;
	}

	if (!FD_ISSET(hSock, &fset))
	{
		net_debug();
		return -1;
	}
		
	// ��������
	ret = recv(hSock, precvBuf, nSize, 0);
	if (ret < 0)
	{
		//printf("TcpReceive Failed\n");
		return -1;
	}
	
	//printf("TcpReceive OK\n");

	return ret;
}
*/

// ��Ϣ�̴߳����Ƚ�������Э���ͷ��Ȼ���жϣ����ͷ��ȷ����������������������������ٴ���
int TcpMsgRecvThread()
{
	int ret = -1;
	char *recvBuf = NULL;
	NET_HEAD *netHead = NULL;
	char *pData = NULL;
	NET_HEAD keepalive;
	CLIENT_INFO *clientInfo = NULL;
	MSG_HEAD msgHead;
	int nRight = 0;

	fd_set fset;
	struct timeval to;

	pthread_detach(pthread_self());
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	// ���Keepalive����
	keepalive.nFlag = HDVS_FLAG;
	keepalive.nCommand = NETCMD_KEEP_ALIVE;
	keepalive.nErrorCode = 0;
	keepalive.nBufSize = 0;
	keepalive.nReserve = 0;

	// ���䷢��recvBuf
	recvBuf = (char *)malloc(NETCMD_MAX_SIZE);	// 100K
	if (recvBuf == NULL)
	{
		net_debug();
		restartSystem();
		return -1;
	}
	netHead = (NET_HEAD*)recvBuf;
	pData = recvBuf + sizeof(NET_HEAD);
	g_server_info.bServerExit = 0;

	while (!g_server_info.bServerExit)
	{
		pthread_mutex_lock(&g_server_info.msgThreadMutex);
		//printf("%s %d\n", __func__, __LINE__);
		// �ȴ��û���½�����������̳߳�
		while (g_server_info.msgWaitThreadNum == 0) 
		{
			pthread_cond_wait(&g_server_info.msgThreadCond, &g_server_info.msgThreadMutex);
			if (g_server_info.bServerExit)
			{
				break;
			}
		}

		// �ж��û��Ƿ��˳�
		if (g_server_info.bServerExit)
		{
			pthread_mutex_unlock(&g_server_info.msgThreadMutex);
			continue;
		}
		
		// �����ѵ�½���û�������δ�����̳߳�
		clientInfo = FindWaitProccessClient();
		if ((clientInfo == NULL)||(clientInfo->nflag == UDP_FLAG))
		{
			pthread_mutex_unlock(&g_server_info.msgThreadMutex);
			printf("udp message quit!\n");
			break;
		}
		if (clientInfo->hMsgSocket <= 0)
		{
			continue;
		}
		
	
		// �����û��̳߳ؼ���
		g_server_info.msgProcessThreadNum++;
		g_server_info.msgWaitThreadNum--;
		if (g_server_info.msgWaitThreadNum < 0)
		{
			g_server_info.msgWaitThreadNum = 0;
		}

		pthread_mutex_unlock(&g_server_info.msgThreadMutex);

		//
		FD_ZERO(&fset);
		memset(&to, 0, sizeof(to));

		clientInfo->nKeepAlive = 0;
		clientInfo->status = RUN;
		nRight = clientInfo->level;
		       
		while (STOP != clientInfo->status)
		{
			// ��������
			clientInfo->nKeepAlive ++;
			if (clientInfo->nKeepAlive >= 10)
			{
				net_debug();
				printf("TCP_CMD_ERROR(%d): NOT KEEPALIVE!\n", clientInfo->hMsgSocket);
				break;
			}
					
			//��������Э���ͷ
			ret = 0;
			ret = TcpReceive(clientInfo->hMsgSocket, (char *)netHead, sizeof(NET_HEAD));
			//ret = recv(clientInfo->hMsgSocket, (char *)netHead, sizeof(NET_HEAD), 0);
			if (ret < 0)
			{
				net_debug();
				printf("TCP_CMD_ERROR(%d): RECEIVE ERROR(NET_HEAD)!\n", clientInfo->hMsgSocket);
				break;
			}
			if (ret == 0)
			{
				printf("TCP_CMD_ERROR(%d): RECEIVE TIMEOUT(NET_HEAD)!\n", clientInfo->hMsgSocket);
				continue;
			}
			
			// �ж�����֡ͷ�Ƿ���ȷ
			if (netHead->nFlag != HDVS_FLAG)
			{
				printf("TCP_CMD_ERROR(%d): NET FLAG ERROR!\n", clientInfo->hMsgSocket);
				printf_bytes(recvBuf, 32);
				continue;
			}

			// �ж��Ƿ�������,����ǵĻ�����ظ�
			if (netHead->nCommand == NETCMD_KEEP_ALIVE)
			{
				//printf("NETCMD_KEEP_ALIVE(%d): %d\n", clientInfo->hMsgSocket, netHead->nReserve);

				clientInfo->nKeepAlive = 0;
				
				ret = send(clientInfo->hMsgSocket, &keepalive, sizeof(NET_HEAD), 0);
				if (ret < 0)
				{
					printf("TCP_CMD_ERROR(%d): SEND KEEPALIVE ERROR!\n", clientInfo->hMsgSocket);
					net_debug();
				}

				continue;
			}

			//printf("NetHead BufferSize: %d\n", netHead->nBufSize);

			// ���������������
			if (netHead->nBufSize>0 && netHead->nBufSize<NETCMD_MAX_SIZE-sizeof(NET_HEAD))
			{
				ret = TcpReceive(clientInfo->hMsgSocket, pData, netHead->nBufSize);
				//ret = recv(clientInfo->hMsgSocket, pData, netHead->nBufSize, 0);
				if (ret < 0)
				{
					net_debug();
					printf("TCP_CMD_ERROR(%d): RECEIVE ERROR(CONTEXT)!\n", clientInfo->hMsgSocket);
					break;
				}
				if (ret == 0)
				{
					net_debug();
					printf("TCP_CMD_ERROR(%d): RECEIVE TIMEOUT(CONTEXT)!\n", clientInfo->hMsgSocket);
					continue;
				}
			}

			// ���û�����������в�����������Ҳ�ɵ�����			
			clientInfo->nKeepAlive = 0;

			// ��������
			msgHead.nSock = clientInfo->hMsgSocket;
			msgHead.nCmd = netHead->nCommand;
			msgHead.nRight = nRight;
			msgHead.nErrorCode = 0;
			msgHead.nBufSize = netHead->nBufSize;
			msgHead.nflag = TCP_FLAG;
			g_server_info.nupdate_flag = TCP_FLAG;
			printf("%s %d %x\n", __func__, __LINE__, netHead->nCommand);
					

			ParseCommand(&msgHead, pData);
		}
		clientInfo->status = STOP;
		// �ͷ������̳߳صĽڵ�
		//printf("Cmd Exit(%d)!\n", clientInfo->hMsgSocket);

		StopUdpNode(clientInfo->hMsgSocket);
		StopTcpNode(clientInfo->hMsgSocket);
		pthread_cond_signal(&g_server_info.dataQuitThreadCond);
		WakeupStreamWait();
		usleep(1);

		// ˢ���̳߳صļ���
		pthread_mutex_lock(&g_server_info.msgQuitThreadMutex);
		g_server_info.msgQuitThreadNum ++;
		g_server_info.msgProcessThreadNum --;
		if (g_server_info.msgProcessThreadNum < 0)
		{
			g_server_info.msgProcessThreadNum = 0;
		}
		pthread_mutex_unlock(&g_server_info.msgQuitThreadMutex);

		pthread_cond_signal(&g_server_info.msgQuitThreadCond);

	}
	
	g_server_info.nMsgThreadCount--;
	
	// �ͷŽ���BUFFER
	if (recvBuf)
	{
		free(recvBuf);
		recvBuf = NULL;
	}
	
	return 0;
}

// TCP�����˳��߳�
int TcpMsgQuitThread()
{
	int hFind = 0;
    
	pthread_detach(pthread_self());

	while (!g_server_info.bServerExit)
	{
		pthread_mutex_lock(&g_server_info.msgQuitThreadMutex);
		while (g_server_info.msgQuitThreadNum == 0)
		{
			pthread_cond_wait(&g_server_info.msgQuitThreadCond, &g_server_info.msgQuitThreadMutex);
		}
            
		if (g_server_info.bServerExit)
		{
			pthread_mutex_unlock(&g_server_info.msgQuitThreadMutex);
			break;
		}

		while((hFind = GetExitClient()) > 0)
		{
			ClientLogoff(hFind);
		}

		g_server_info.msgQuitThreadNum = 0;
		pthread_mutex_unlock(&g_server_info.msgQuitThreadMutex);
	}
    
	return 0;
}

// �ͻ���½
int ClientLogon(struct sockaddr_in addr, NET_USER_INFO userInfo, int nID, int nRight, int nflag)
{
	CLIENT_INFO *pTmp = NULL;
	CLIENT_INFO *pClientInfo = NULL;

	if (nID < 0)
	{
		return -1;
	}
	
	pClientInfo = (CLIENT_INFO *)malloc(sizeof(CLIENT_INFO));
	if (pClientInfo == NULL)
	{
		return -1;
	}
	memset(pClientInfo, 0, sizeof(CLIENT_INFO));

	pClientInfo->ip = addr.sin_addr.s_addr;
	pClientInfo->port = addr.sin_port;
	pClientInfo->addr = addr;
	pClientInfo->nflag = nflag; //1����UDP 0 ����TCP
	strcpy(pClientInfo->szUserName,userInfo.szUserName);
	strcpy(pClientInfo->szUserPsw,userInfo.szUserPsw);
	pClientInfo->level = nRight;
	pClientInfo->hMsgSocket = nID;
	pClientInfo->status = READY;
	pClientInfo->nKeepAlive = 0;
	pClientInfo->pNext = NULL;
	
	

	pthread_mutex_lock(&g_client_list.hClientMutex);
	
	pTmp = g_client_list.pNext;
	if (pTmp == NULL)
	{
		g_client_list.pNext = pClientInfo;
	}
	else
	{
		while(pTmp->pNext)
		{
			pTmp = pTmp->pNext;
		}
		pTmp->pNext = pClientInfo;
	}
	g_client_list.nTotalNum ++;
	
	pthread_mutex_unlock(&g_client_list.hClientMutex);

	return 0;
}

// �ͻ��˳�
int ClientLogoff(int nID)
{
	int ret = -1;
	CLIENT_INFO *pTmp = NULL;
	CLIENT_INFO *pPre = NULL;

	pthread_mutex_lock(&g_client_list.hClientMutex);
	
	pTmp = g_client_list.pNext;
	if (NULL != pTmp)
	{
		if (nID == pTmp->hMsgSocket)
		{
			printf("Close TCP CMD Socket: %d<%d>\n", pTmp->hMsgSocket, __LINE__);

			g_client_list.pNext = pTmp->pNext;
			shutdown(pTmp->hMsgSocket, 2);
			close(pTmp->hMsgSocket);
			pTmp->hMsgSocket = -1;		
			free(pTmp);
			g_client_list.nTotalNum --;
			ret = 0;
		}
		else
		{
			while (pTmp)
			{
				pPre = pTmp;
				pTmp = pTmp->pNext;
				if (pTmp && (nID == pTmp->hMsgSocket))
				{
					printf("Close TCP CMD Socket: %d<%d>\n", pTmp->hMsgSocket, __LINE__);

					pPre->pNext = pTmp->pNext;
					shutdown(pTmp->hMsgSocket, 2);
					close(pTmp->hMsgSocket);
					pTmp->hMsgSocket = -1;	
					free(pTmp);
					g_client_list.nTotalNum --;
					ret = 0;
					break;
				}
			}
		}
	}
	
	pthread_mutex_unlock(&g_client_list.hClientMutex);

	return ret;
}

// ���Ҵ��ڵȴ�����Ŀͻ�
CLIENT_INFO *FindWaitProccessClient()
{
	int bFind = 0;
	CLIENT_INFO *pClient = NULL;

	pthread_mutex_lock(&g_client_list.hClientMutex);
	
	pClient = g_client_list.pNext;
	while(pClient)
	{
		if(READY == pClient->status)
		{
			bFind = 1;
			break;
		}
		pClient = pClient->pNext;
	}
	
	pthread_mutex_unlock(&g_client_list.hClientMutex);
	
	if (!bFind)
	{
		pClient = NULL;
	}

	return pClient;
}

// ��ȡ�˳����û�
int GetExitClient()
{
	int nRet = 0;
	CLIENT_INFO *pTmp = NULL;

	pthread_mutex_lock(&g_client_list.hClientMutex);
	
	pTmp = g_client_list.pNext;
	while (pTmp)
	{
		if (STOP == pTmp->status)
		{
			nRet = pTmp->hMsgSocket;
			break;
		}
		pTmp = pTmp->pNext;
	}
	
	pthread_mutex_unlock(&g_client_list.hClientMutex);

	return nRet;
}

int ClientExist(unsigned long ip, unsigned long port)
{
	int nRet = 0;
	CLIENT_INFO *pTmp = NULL;

	pthread_mutex_lock(&g_client_list.hClientMutex);
	
	pTmp = g_client_list.pNext;
	while (pTmp)
	{
		if ((pTmp->ip == ip) && (pTmp->port == port))
		{
			nRet = pTmp->hMsgSocket;
			break;
		}
		pTmp = pTmp->pNext;
	}
	
	pthread_mutex_unlock(&g_client_list.hClientMutex);

	return nRet;
}

int GetClient(int nID, CLIENT_INFO *pClientInfo)
{
	int bRet = FALSE;
	CLIENT_INFO *pTmp = NULL;	

	if (NULL == pClientInfo)
	{
		return FALSE;
	}

	pthread_mutex_lock(&g_client_list.hClientMutex);
	
	pTmp = g_client_list.pNext;
	while (pTmp)
	{
		if (pTmp->hMsgSocket == nID)
		{
			memcpy(pClientInfo,pTmp,sizeof(CLIENT_INFO));
			bRet = TRUE;
			break;                     
		}
		pTmp = pTmp->pNext;
	}
	
	pthread_mutex_unlock(&g_client_list.hClientMutex);
	
	return bRet;
}

int StopAllClient()
{
    int nNum = 0;
    CLIENT_INFO *pClient = NULL;

    pthread_mutex_lock(&g_client_list.hClientMutex);
    
    pClient = g_client_list.pNext;
    while (pClient)
    {
        pClient->status = STOP;
        nNum ++;
        pClient = pClient->pNext;
    }
    
    pthread_mutex_unlock(&g_client_list.hClientMutex);

    return nNum;
}

int FreeAllClient()
{
	int nFreeNum = 0;
	CLIENT_INFO *pTmp = NULL;

	pthread_mutex_lock(&g_client_list.hClientMutex);
    
	while (g_client_list.pNext)
	{
		printf("Close TCP CMD Socket: %d<%d>\n", pTmp->hMsgSocket, __LINE__);

		pTmp = g_client_list.pNext;
		shutdown(pTmp->hMsgSocket, 2);
		close(pTmp->hMsgSocket);
		pTmp->hMsgSocket = -1;
		g_client_list.pNext = pTmp->pNext;
		free(pTmp);
		nFreeNum ++;
	}
	if (nFreeNum != g_client_list.nTotalNum)
	{
		net_debug();
	}

	g_client_list.nTotalNum = 0;
    
	pthread_mutex_unlock(&g_client_list.hClientMutex);
    
	return 0;
}
