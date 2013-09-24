#include <pthread.h>
#include "onvif.nsmap"
#include "soapH.h"
#include "soapStub.h"
#include "openssl/ssl.h"


/* ����ȫ�ֱ����Ķ��� */
#define BACKLOG (100) 
#define MAX_THR (10)   
#define MAX_QUEUE (1000)


pthread_mutex_t queue_cs;//������
pthread_cond_t queue_cv;//��������
SOAP_SOCKET queue[MAX_QUEUE];//�������
int head =0, tail =0;//����ͷ����β��ʼ��         


void * process_queue(void *);//�߳���ں���
int enqueue(SOAP_SOCKET);//����к���
SOAP_SOCKET dequeue(void);//�����к���


/* �߳���ں��� */
void * process_queue(void * soap)
{
	struct soap * tsoap = (struct soap *)soap;
	for(;;)
	{
		tsoap->socket = dequeue();
		if(!soap_valid_socket(tsoap->socket))
		{
			break;
		}
		soap_serve(tsoap);
		soap_destroy(tsoap);
		soap_end(tsoap);
	}
	return NULL;
}
/* ����в��� */
int enqueue(SOAP_SOCKET sock)
{
	int status = SOAP_OK;
	int next;

	pthread_mutex_lock(&queue_cs);
	next = tail +1;
	if (next >= MAX_QUEUE) 
	{
		next = 0;
	}
	if (next == head) 
	{
		status = SOAP_EOM;
	}
	else
	{
		queue[tail] =sock;
		tail = next;
	}
	pthread_cond_signal(&queue_cv);
	pthread_mutex_unlock(&queue_cs);

	return status;
}

/* �����в��� */
SOAP_SOCKET dequeue()
{
	SOAP_SOCKET sock;

	pthread_mutex_lock(&queue_cs);
	while (head == tail )
	{
		pthread_cond_wait(&queue_cv,&queue_cs);
	}

	sock = queue[head++];
	if (head >= MAX_QUEUE)
	{
		head =0;
	}
	pthread_mutex_unlock(&queue_cs);

	return sock;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//������
int main(int argc,char ** argv)
{
	struct soap ServerSoap;
	struct ip_mreq mcast;
	int count = 0;

//	soap_ssl_init();
	//��ʼ������ʱ����
	soap_init(&ServerSoap);
	ServerSoap.accept_timeout = 10;
	ServerSoap.recv_timeout = 10;
	ServerSoap.send_timeout = 10;

	soap_set_namespaces(&ServerSoap, namespaces);

	if(!soap_valid_socket(soap_bind(&ServerSoap, NULL, 4000, 10)))
	{
		soap_print_fault(&ServerSoap, stderr);
		exit(1);
	}

	//���û�в���������CGI������
	struct soap * soap_thr[MAX_THR];
	pthread_t tid[MAX_THR];
	int i = 0, port = 3702;//atoi(argv[1]);
	SOAP_SOCKET m, s;

	//��������������ʼ��
	pthread_mutex_init(&queue_cs,NULL);
	pthread_cond_init(&queue_cv,NULL);

	//���ɷ����߳�
	for(i = 0; i < MAX_THR; i++)
	{
		soap_thr[i] = soap_copy(&ServerSoap);
		fprintf(stderr,"Starting thread %d \r\n",i);
		pthread_create(&tid[i],NULL,(void*(*)(void*))process_queue,(void*)soap_thr[i]);
	}

	for(;;)
	{
#if 0
		if(soap_serve(&ServerSoap))
		{
			soap_print_fault(&ServerSoap, stderr);
		}

		soap_destroy(&ServerSoap);
		soap_end(&ServerSoap);
		printf("1aaaaaaaaaaaaaaaaaaa %x\r\n", ServerSoap.ip);
		printf("Accepted count %d, connection from IP = %d.%d.%d.%d socket = %d \r\n",
				count, ((ServerSoap.ip)>>24)&0xFF, ((ServerSoap.ip)>>16)&0xFF, ((ServerSoap.ip)>>8)&0xFF, (ServerSoap.ip)&0xFF, (ServerSoap.socket));
#else
		count++;
		//���ܿͻ��˵�����
		s = soap_accept(&ServerSoap);
		printf("accept_flags = %d, s = %d\r\n", ServerSoap.accept_flags, s);
		if(!soap_valid_socket(s)) 
		{
			if(ServerSoap.errnum) 
			{
				soap_print_fault(&ServerSoap,stderr);
				continue;
			}else
			{
				printf("Server timed out \r\n");
				continue;
			}
		}

		//�ͻ��˵�IP��ַ
		printf("Accepted count %d, connection from IP = %lu.%lu.%lu.%lu socket = %d \r\n",
				count, ((ServerSoap.ip)>>24)&0xFF, ((ServerSoap.ip)>>16)&0xFF, ((ServerSoap.ip)>>8)&0xFF, (ServerSoap.ip)&0xFF, (ServerSoap.socket));
		//������׽��ֽ�����У��������������ѭ���ȴ�
		while(enqueue(s) == SOAP_EOM)
		{
			usleep(1000);
		}
#endif
	}

	//����������������
	for(i = 0; i < MAX_THR; i++)
	{
		while (enqueue(SOAP_INVALID_SOCKET) == SOAP_EOM) 
		{
			usleep(1000);
		}
	}

	for(i=0; i< MAX_THR; i++)
	{
		fprintf(stderr,"Waiting for thread %d to terminate ..",i);
		pthread_join(tid[i],NULL);
		fprintf(stderr,"terminated ");
		soap_done(soap_thr[i]);
		free(soap_thr[i]);
	}

	pthread_mutex_destroy(&queue_cs);
	pthread_cond_destroy(&queue_cv);

	//��������ʱ�Ļ���
	soap_done(&ServerSoap);

	return 0;
}

