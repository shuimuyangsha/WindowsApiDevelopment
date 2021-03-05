/* ************************************
*����ͨWindows API��
* ʾ������
* Service.c
* 6.6	����
* ��������SplSrv.exe ��Ϊ����������
**************************************/
/* ͷ�ļ���*/
#include <windows.h>
/* ȫ�ֱ�����*/
SERVICE_STATUS          SplSrvServiceStatus; 
SERVICE_STATUS_HANDLE   SplSrvServiceStatusHandle; 
/* ����������*/
VOID SvcDebugOut(LPSTR String, DWORD Status);
VOID WINAPI SplSrvServiceCtrlHandler (DWORD opcode); 
VOID WINAPI SplSrvServiceStart (DWORD argc, LPTSTR *argv); 
DWORD SplSrvServiceInitialization (DWORD argc, LPTSTR *argv, 
								   DWORD *specificError); 

/*************************************
* VOID WINAPI SplSrvServiceStart (DWORD argc, LPTSTR *argv)
* ����	������������
*
* ����	δʹ��
**************************************/
VOID WINAPI SplSrvServiceStart (DWORD argc, LPTSTR *argv) 
{ 
	DWORD status; 
	DWORD specificError; 
	// ���SERVICE_STATUS �ṹ
	SplSrvServiceStatus.dwServiceType        = SERVICE_WIN32; 
	SplSrvServiceStatus.dwCurrentState       
		= SERVICE_START_PENDING;		// ����������
	SplSrvServiceStatus.dwControlsAccepted   
		= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
	SplSrvServiceStatus.dwWin32ExitCode      = 0; 
	SplSrvServiceStatus.dwServiceSpecificExitCode = 0; 
	SplSrvServiceStatus.dwCheckPoint         = 0; 
	SplSrvServiceStatus.dwWaitHint           = 0; 
	// ע������������������
	SplSrvServiceStatusHandle = RegisterServiceCtrlHandler( 
		"Sample_Srv",	// ���������ڴ�������ʱʹ����
		// SERVICE_WIN32_OWN_PROCESS����˱����������ԡ�
		SplSrvServiceCtrlHandler);	// �������������̣�������

	if (SplSrvServiceStatusHandle == (SERVICE_STATUS_HANDLE)0) 
	{ 
		SvcDebugOut(" [SPLSRV_SERVICE] RegisterServiceCtrlHandler "
			"failed %d\n", GetLastError()); 
		return; 
	}  
	// ��ʼ����������ʾ��δʹ�ã�����Ϊ��
	status = SplSrvServiceInitialization(argc,argv, &specificError);  
	// ��ʼ�������û������޸�
	if (status != NO_ERROR) 
	{ 
		SplSrvServiceStatus.dwCurrentState       = SERVICE_STOPPED; 
		SplSrvServiceStatus.dwCheckPoint         = 0; 
		SplSrvServiceStatus.dwWaitHint           = 0; 
		SplSrvServiceStatus.dwWin32ExitCode      = status; 
		SplSrvServiceStatus.dwServiceSpecificExitCode = specificError; 

		SetServiceStatus (SplSrvServiceStatusHandle, &SplSrvServiceStatus); 
		return; 
	} 
	// ��ʼ����ɣ���������״̬
	SplSrvServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
	SplSrvServiceStatus.dwCheckPoint         = 0; 
	SplSrvServiceStatus.dwWaitHint           = 0; 

	if (!SetServiceStatus (SplSrvServiceStatusHandle, &SplSrvServiceStatus)) 
	{ 
		status = GetLastError(); 
		SvcDebugOut(" [SPLSRV_SERVICE] SetServiceStatus error %ld\n",status); 
	} 
	// �û������޸ģ�������ɷ���Ĺ���
	SvcDebugOut(" [SPLSRV_SERVICE] Returning the Main Thread \n",0); 

	return; 
} 


/*************************************
* DWORD SplSrvServiceInitialization(DWORD   argc, 
*	LPTSTR  *argv, 
*	DWORD *specificError)
* ����	��ʼ��������δ�����κι��������������޸�
*
* ����	
**************************************/
DWORD SplSrvServiceInitialization(DWORD   argc, 
								  LPTSTR  *argv, 
								  DWORD *specificError) 
{ 
	return(0); 
}

/*************************************
* VOID WINAPI SplSrvServiceCtrlHandler (DWORD Opcode)
* ����	�����������Ĵ���������ControlService������ϡ�
*
* ����	���������
**************************************/
VOID WINAPI SplSrvServiceCtrlHandler (DWORD Opcode) 
{ 
	DWORD status; 
	switch(Opcode) 
	{ 
	case SERVICE_CONTROL_PAUSE: 
		// �����ع���
		SplSrvServiceStatus.dwCurrentState = SERVICE_PAUSED; 
		break; 
	case SERVICE_CONTROL_CONTINUE: 
		// �����ع���
		SplSrvServiceStatus.dwCurrentState = SERVICE_RUNNING; 
		break; 
	case SERVICE_CONTROL_STOP: 
		// �����ع���
		SplSrvServiceStatus.dwWin32ExitCode = 0; 
		SplSrvServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
		SplSrvServiceStatus.dwCheckPoint    = 0; 
		SplSrvServiceStatus.dwWaitHint      = 0; 

		if (!SetServiceStatus (SplSrvServiceStatusHandle, 
			&SplSrvServiceStatus))
		{ 
			status = GetLastError(); 
			SvcDebugOut(" [SPLSRV_SERVICE] SetServiceStatus error %ld\n", 
				status); 
		}  
		SvcDebugOut(" [SPLSRV_SERVICE] Leaving SplSrvService \n",0); 
		return; 
	case SERVICE_CONTROL_INTERROGATE: 
		// �յ�������󷢳����죬��ʾ�����������Ĵ�����̣����߿������޸�
		MessageBeep(MB_OK);
		break; 
	default: 
		SvcDebugOut(" [SPLSRV_SERVICE] Unrecognized opcode %ld\n", 
			Opcode); 
	} 
	// ��ǰ״̬
	if (!SetServiceStatus (SplSrvServiceStatusHandle,  &SplSrvServiceStatus)) 
	{ 
		status = GetLastError(); 
		SvcDebugOut(" [SPLSRV_SERVICE] SetServiceStatus error %ld\n", 
			status); 
	} 
	return; 
}
/*************************************
* void main( ) 
* ����	������ں�����ע��������������ȡ�
*
* ����	���������
**************************************/
void main( ) 
{ 
	// ����SERVICE_TABLE_ENTRY ���ݽṹ����NULL �ṹ������
	// ��ΪStartServiceCtrlDispatcher �����Ĳ�����
	SERVICE_TABLE_ENTRY   DispatchTable[] = 
	{ 
		{ "Sample_Srv", (LPSERVICE_MAIN_FUNCTION) SplSrvServiceStart }, 
		{ NULL, NULL } 
	}; 
	if (!StartServiceCtrlDispatcher( DispatchTable)) 
	{ 
		SvcDebugOut(" [SPLSRV_SERVICE] StartServiceCtrlDispatcher (%d)\n", 
			GetLastError()); 
	} 
} 
/*************************************
* VOID SvcDebugOut(LPSTR String, DWORD Status) 
* ����	��ʾ��Ϣ����������
*
* ����	LPSTR String	��Ϣ�ַ���
*		DWORD Status	״̬
**************************************/
VOID SvcDebugOut(LPSTR String, DWORD Status) 
{ 
	CHAR  Buffer[1024]; 
	if (strlen(String) < 1000) 
	{ 
		wsprintf(Buffer, String, Status); 
		OutputDebugString(Buffer); 
	} 
}
