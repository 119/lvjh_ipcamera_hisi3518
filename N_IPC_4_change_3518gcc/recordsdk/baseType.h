/******************************************************************************
* ��Ȩ��Ϣ��(C) 2006-2010, ������̩���˿Ƽ����޹�˾�з�����Ȩ����
* ϵͳ���ƣ�¼��SDK
* �ļ����ƣ�baseType.h
* �ļ�˵�������ļ����������ݻ�������
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

#ifndef	__BASETYPE_H_
#define	__BASETYPE_H_

typedef	char				int8;
typedef	unsigned char		uint8;
typedef	short			int16;
typedef	unsigned short	uint16;
typedef	long				int32;
typedef	unsigned long		uint32;

typedef	char				INT8;
typedef	unsigned char		UINT8;
typedef	short			INT16;
typedef	unsigned short	UINT16;
typedef	long				INT32;
typedef	unsigned long		UINT32; 

typedef unsigned long		DWORD;
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef float				FLOAT;
typedef void*				LPVOID;
typedef int				BOOL;
typedef void* 			HANDLE;

#ifndef	TRUE
#define	TRUE	(1 == 1)
#endif

#ifndef	FALSE
#define	FALSE	(1 == 0)
#endif

#ifndef	true
#define	true	(1 == 1)
#endif
#ifndef	false
#define	false	(1 == 0)
#endif

#endif

