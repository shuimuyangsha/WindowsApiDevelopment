/* ************************************
*����ͨWindows API�� 
* ʾ������
* msg.c
* 6.5  ��̬���ӿ�
**************************************/
/* ͷ�ļ���*/
#include <Windows.h>
#include <Psapi.h>
/* ���ӡ�*/
#pragma comment (lib, "Psapi.lib") 
/* ����������*/

// ʹ��__declspec(dllexport)������������
__declspec(dllexport) DWORD ExportExample(LPSTR szMsg, DWORD dwCode);

/*************************************
* DllMain
**************************************/
BOOL WINAPI DllMain(
					HINSTANCE hinstDLL,  // DLLģ��ľ��
					DWORD fdwReason,     // ���õ����
					LPVOID lpReserved )  // reserved
{
	// �ڲ�ͬ������¶������DllMain�������ֱ���
	switch( fdwReason ) 
	{ 
		// ����Dll
	case DLL_PROCESS_ATTACH:
		{
			CHAR lpMainMoudleName[MAX_PATH];
			CHAR lpMessage[MAX_PATH+64];
			// ��ȡPID ����ģ��������������Ϣ��
			DWORD dwPID = GetCurrentProcessId();
			GetModuleBaseName(GetCurrentProcess(),NULL,lpMainMoudleName,MAX_PATH);
			wsprintf(lpMessage,"Process name: %s, PID: %u ",lpMainMoudleName,dwPID);
			MessageBox(NULL,lpMessage,"msg.dll",MB_OK);
			break;
		}
		// �½��߳�
	case DLL_THREAD_ATTACH:
		break;
		// �߳��˳�
	case DLL_THREAD_DETACH:
		break;
		// �ͷ�Dll
	case DLL_PROCESS_DETACH:

		break;
	}
	return TRUE;
}

/*************************************
* DWORD ExportExample(LPSTR szMsg, DWORD dwCode)
* ����	������������ʾ��Ϣ
*
* ����	LPSTR szMsg	�ַ����� DWORD dwCode ����
**************************************/
DWORD ExportExample(LPSTR szMsg, DWORD dwCode)
{
	LPVOID lpShowOut = HeapAlloc(GetProcessHeap(), 0, lstrlen(szMsg)+100);
	wsprintf(lpShowOut,"%s,%d",szMsg,dwCode);
	MessageBox(NULL,lpShowOut,"�ɵ���������������Ϣ��",MB_OK);
	HeapFree(GetProcessHeap(), 0, lpShowOut);
	return 0;
}