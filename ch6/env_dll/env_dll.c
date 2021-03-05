/* ************************************
*����ͨWindows API��
* ʾ������
* env_dll.c
* 6.2.3  ��ȡ�����û�������
**************************************/

/* ͷ�ļ���*/
#include <windows.h>
#include <stdio.h>
#include "env.h"
/* Ԥ���塡*/
#define BUFSIZE 4096
/*************************************
* DWORD WINAPI EnumEnvironmentVariables()
* ����	��ʾ���̵����л�������
**************************************/
DWORD WINAPI EnumEnvironmentVariables()
{
	// ��ȡ��������
	PVOID pEv = GetEnvironmentStrings();
	LPSTR szEnvs;
	// ��ʾ
	for (szEnvs = (LPSTR) pEv; *szEnvs;) 
	{ 
		printf("%s\n",szEnvs);
		while (*szEnvs++);
	}
	// �ͷ�
	FreeEnvironmentStrings(pEv);
	return 0;
}
/*************************************
* DWORD WINAPI ChangeEnviromentVariables(LPSTR szName, 
LPSTR szNewValue, 
DWORD dwFlag)
* ����	�ı价������
*
* ����	LPSTR szName	��Ҫ�ı�Ļ���
*		LPSTR szNewValue	�µı���ֵ
*		DWORD dwFlag	���ӡ����û������㣬��س�����env.h�ڶ���
**************************************/
DWORD WINAPI ChangeEnviromentVariables(LPSTR szName, 
									   LPSTR szNewValue, 
									   DWORD dwFlag)
{
	DWORD dwErr;
	PVOID szVal;
	DWORD dwReturn; 
	DWORD dwNewValSize;
	// �����־Ϊ���������Ȼ�ȡ��Ȼ��szNewValue���ӵ�ĩβ
	if(dwFlag == VARIABLES_APPEND)
	{
		dwNewValSize = lstrlen(szNewValue)+1;	// �±���ֵ�Ĵ�С
		// �����ڴ�
		szVal = HeapAlloc(GetProcessHeap(),0,BUFSIZE+dwNewValSize);
		// ��ȡֵ
		dwReturn = GetEnvironmentVariable(szName,szVal,BUFSIZE);
		if(dwReturn == 0)	// ����
		{
			dwErr = GetLastError();
			if( ERROR_ENVVAR_NOT_FOUND == dwErr )
			{
				printf("Environment variable %s does not exist.\n", szName);
			}
			else
			{
				printf("error: %d",dwErr);
			}
			return FALSE;
		}
		else if(BUFSIZE < dwReturn)	// ������̫С
		{
			szVal = (LPTSTR) HeapReAlloc(GetProcessHeap(), 0,szVal, dwReturn+dwNewValSize);
			if(NULL == szVal)
			{
				printf("Memory error\n");
				return FALSE;
			}
			dwReturn = GetEnvironmentVariable(szName, szVal, dwReturn);
			if(!dwReturn)
			{
				printf("GetEnvironmentVariable failed (%d)\n", GetLastError());
				return FALSE;
			}
		}
		lstrcat(szVal,";");		// �ָ���
		lstrcat(szVal,szNewValue);	// ����
		//����
		if(!SetEnvironmentVariable(szName,szVal))
		{
			printf("Set Value Error %d",GetLastError());
		}
		// �ͷ��ڴ�
		HeapFree(GetProcessHeap(),0,szVal);
		return TRUE;
	}
	// ��������ã���ֱ������
	else if(dwFlag == VARIABLES_RESET)
	{
		if(!SetEnvironmentVariable(szName,szNewValue))
		{
			printf("Set value error %d",GetLastError());
		}
	}
	// ���㣬����szNewValue
	else if(dwFlag == VARIABLES_NULL)
	{
		if(!SetEnvironmentVariable(szName,NULL))
		{
			printf("Set value error %d",GetLastError());
		}
	} 
	return TRUE;
}