/* ************************************
*����ͨWindows API�� 
* ʾ������
* init.c
* 6.6	����
* ������Ƴ��򣬰�װ��ɾ������
**************************************/
/* ͷ�ļ���*/
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
/* ȫ�ֱ�����*/
SC_HANDLE schService;
SC_HANDLE schSCManager;
LPTSTR szServiceName = TEXT("Sample_Srv");

/*************************************
* BOOL CreateSampleService(LPTSTR szPath, LPSTR szServiceName) 
* ����	��������
*
* ����
*		SC_HANDLE schSCManager��SCM���
*		LPTSTR szPath�� ��������·��
*		LPSTR szServiceName�� ������
**************************************/
BOOL CreateSampleService( 
						 SC_HANDLE schSCManager,
						 LPTSTR szPath,
						 LPSTR szServiceName) 
{ 
	// ��������
	schService = CreateService( 
		schSCManager,				// SCM ���
		szServiceName,				// ������
		"Service sample",           // ��ʾ�ķ�����
		SERVICE_ALL_ACCESS,			// ��ȡȨ��
		SERVICE_WIN32_OWN_PROCESS,	// �������
		SERVICE_DEMAND_START,		// �������
		SERVICE_ERROR_NORMAL,		// ����������
		szPath,						// ����Ŀ�ִ���ļ�·��
		NULL,						// no load ordering group 
		NULL,						// no tag identifier 
		NULL,						// no dependencies 
		NULL,						// LocalSystem account 
		NULL);						// no password 

	if (schService == NULL) 
	{
		printf("CreateService failed (%d)\n", GetLastError()); 
		return FALSE;
	}
	else
	{
		printf("CreateService succeeded\n"); 
		CloseServiceHandle(schService); 
		return TRUE;
	}
}

/*************************************
* BOOL DeleteSampleService(LPTSTR szNameOfService) 
* ����	ɾ������
*
* ����	LPTSTR szNameOfService	���������
**************************************/
BOOL DeleteSampleService(LPTSTR szNameOfService) 
{ 
	schService = OpenService( 
		schSCManager,       // SCM ��� 
		szNameOfService,	// ������
		DELETE);            // ��ɾ��

	if (schService == NULL)
	{ 
		printf("OpenService failed (%d)\n", GetLastError()); 
		return FALSE;
	}
	// ɾ������
	if (! DeleteService(schService) ) 
	{
		printf("DeleteService failed (%d)\n", GetLastError()); 
		return FALSE;
	}
	else 
		printf("DeleteService succeeded\n"); 
	// �رվ��
	CloseServiceHandle(schService); 
	return TRUE;
}
/*************************************
* void main( int argc, TCHAR *argv[] )
* ����	��ʾ
*
* ����	δʹ��
**************************************/
void main( int argc, TCHAR *argv[] ) 
{
	TCHAR szBinFilePath[MAX_PATH];
	PTCHAR pTemp;
	DWORD dwStopError;
	// ��������ִ�г����·��
	GetModuleFileName(NULL,szBinFilePath,MAX_PATH);
	pTemp = szBinFilePath+lstrlen(szBinFilePath);
	while(*--pTemp!='\\');
	lstrcpy(pTemp,TEXT("\\SplSrv.exe"));

	// �� SCM
	schSCManager = OpenSCManager( 
		NULL,                    // local machine 
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager) 
		printf("OpenSCManager failed (%d)\n", GetLastError());

	// ��������
	CreateSampleService(schSCManager, szBinFilePath, szServiceName);
	// ��������
	StartSampleService(schSCManager,szServiceName);
	// �����������
	ControlSampleService(SERVICE_CONTROL_INTERROGATE);
	ControlSampleService(SERVICE_CONTROL_CONTINUE);
	// ֹͣ����
	dwStopError = StopService( schSCManager, szServiceName, 
		TRUE, 1000);

	if(ERROR_SUCCESS == dwStopError)
	{
		printf("Service Stoped\n"); 
	} 
	else
	{
		printf("Service stoped error (%u)\n",dwStopError); 
	}
	// ɾ������
	DeleteSampleService(szServiceName);
	CloseServiceHandle(schSCManager); 
}