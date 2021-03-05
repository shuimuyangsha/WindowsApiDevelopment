/* ************************************
*����ͨWindows API��
* ʾ������
* process.c
* 6.2.1  �������̡���ȡ���������Ϣ����ȡ��������
**************************************/
/* ͷ�ļ���*/
#include <windows.h>
#include <stdio.h>

DWORD CreateChildProcess(LPSTR szChildProcessCmd);

/*************************************
* int main(void)
* ����	��ʾ���̴���
*
* ����	δʹ��
**************************************/
int main()
{	
	CreateChildProcess("Child.exe abc 123");
}
/*************************************
* DWORD CreateChildProcess(LPSTR szChildProcessCmd)
* ����	��ʾ�����ӽ���
*
* ����	LPSTR szChildProcessCmd	�����ӽ��̵�������
* ����ֵ	�ɹ����أ�ʧ�ܷ���
**************************************/
DWORD CreateChildProcess(LPSTR szChildProcessCmd)
{	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	// ��������Ϣ�ṹ����
	ZeroMemory( &si, sizeof(si) );
	// ���ýṹ��С��cb����ӦΪ�ṹ�Ĵ�С
	si.cb = sizeof(si);
	// ��������Ϣ�ṹ����
	ZeroMemory( &pi, sizeof(pi) );

	// �����ӽ��̣����ж��Ƿ�ɹ�
	if( !CreateProcess( NULL,		// ʹ��������
		szChildProcessCmd,			// ������
		NULL,             // ���̳н��̾��
		NULL,             // ���̳��߳̾��
		FALSE,            // ���̳о��
		0,                // û�д�����־
		NULL,             // ʹ�ø����̻�������
		NULL,             // ʹ�ø�����Ŀ¼��Ϊ��ǰĿ¼
		&si,              // STARTUPINFO �ṹ
		&pi )             // PROCESS_INFORMATION ���������Ϣ
		) 
	{
		// ����ʧ��
		printf( "CreateProcess failed (%d).\n", GetLastError() );
		return 1;
	}
	// �ڴ����ɹ��󸸽���Ҳ��ֱ���˳�������ȴ��ӽ���ִ�н���

	// �ȴ��ӽ��̽���
	// ʹ�õ���ͨ��PROCESS_INFORMATION ���ص������Ϣ��hProcessΪ�ӽ��̾��
	// ������Ҳ���Բ��ȴ��ӽ���������ɶ�ֱ���˳�
	WaitForSingleObject( pi.hProcess, INFINITE );
	// �رս��̾�����߳̾��
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
	return 0;
}
