#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "indexFile.h"

/******************************************************************************
* �������ƣ�write_record_index_file
* ����������д�����ļ�
* ���������char *file_name		�ļ���
*			int minute			����
*			int file_type		�ļ�����
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int write_record_index_file(char *file_name, int minute, int file_type)
{
	FILE * file = NULL;
	RECORD_FILE_INDEX index;
	int num = 0;

	printf("write_record_index_file: %s %d %d\n", file_name, minute, file_type);

	if (file_name == NULL)
	{
		return -1;
	}
	
	if (minute < 0 || minute >= MAX_RECORD_FILE_NUM)
	{
		return -1;
	}
	
	memset(&index, 0, sizeof(RECORD_FILE_INDEX));
	
	file = fopen(file_name, "r+b");
	if (file == NULL)
	{
		file = fopen(file_name, "w+b");
		if (file == NULL)
		{
			printf("write_record_index_file(Error): %s %d %d\n", file_name, minute, file_type);
			return -1;
		}
	}

	num = fread(&index, 1, sizeof(RECORD_FILE_INDEX), file);
	if (num < sizeof(RECORD_FILE_INDEX))
	{
		printf("fread index error(%d %d)!\n", num, sizeof(RECORD_FILE_INDEX));
		memset(&index, 0, sizeof(RECORD_FILE_INDEX));
	}

	index.file_type[minute] = file_type;
	fwrite(&index, 1, sizeof(RECORD_FILE_INDEX), file);
	fflush(file);
	fsync(file);
	fclose(file);
	
	return 0;
}

/******************************************************************************
* �������ƣ�search_record_index_file_by_type
* ����������ͨ�������ļ�����ѯ�ļ�����
* ���������char *file_name		�ļ���
*			int file_type		�ļ�����
* �����������
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int search_record_index_file_by_type(char *file_name, int file_type)
{
	FILE * file = NULL;
	struct stat buf;
	RECORD_FILE_INDEX index;
	int i,num = 0;

	//printf("%s %x\n", file_name, file_type);

	if (file_name == NULL)
	{
		return -1;
	}
	/*
	if (stat(file_name, &buf) != 0) // �ļ������� 
	{
		printf("stat: %s error!\n", file_name);
		return -1;
	}
	*/
	file = fopen(file_name, "rb");
	if (file == NULL)
	{
		//printf("fopen: %s error!\n", file_name);
		return -1;
	}
	
	num = fread(&index, 1, sizeof(RECORD_FILE_INDEX), file);
	
	fclose(file);
	
	if (num < sizeof(RECORD_FILE_INDEX))
	{
		//printf("fread(%d): %s error!\n", num, file_name);
		return -1;
	}
		
	for (i=0; i<MAX_RECORD_FILE_NUM; i++)
	{
		//printf("%d---%x---%x\n", i,  index.file_type[i], file_type);

		if ((index.file_type[i] & file_type) != 0)
		{
			return 1;
		}
	}	
	
	return -1;
}

/******************************************************************************
* �������ƣ�read_record_index_file
* �����������������ļ�
* ���������char *file_name				�ļ���		
* ���������RECORD_FILE_INDEX *index	�ļ�������Ϣ
* �� �� ֵ���ɹ�: 0; ʧ��: -1
* �޸ļ�¼: ��
* ����˵��: ��
********************************************************************************/
int read_record_index_file(char *file_name, RECORD_FILE_INDEX *index)
{
	FILE * file = NULL;
	int num = 0;

	if (file_name==NULL || index==NULL)
	{
		return -1;
	}
	
	file = fopen(file_name, "rb");
	if (file == NULL)
	{
		return -1;
	}
	
	num = fread(index, 1, sizeof(RECORD_FILE_INDEX), file);
	
	fclose(file);
	
	if(num < sizeof(RECORD_FILE_INDEX))
	{
		return -1;
	}
	
	return 0;
}



















