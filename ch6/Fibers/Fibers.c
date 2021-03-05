/* ************************************
*����ͨWindows API�� 
* ʾ������
* Fibers.c
* 6.3.4  ������ɾ���˳̡������˳�
* 6.3.5  �˳����̵߳Ļ���ת��
**************************************/
/* Ԥ���塡*/
#define _WIN32_WINNT 0x0501
/* ͷ�ļ���*/
#include <windows.h>
#include <stdio.h>
/* ����������*/
VOID WINAPI ReadFiberFunc( LPVOID lpParameter );
VOID WINAPI WriteFiberFunc( LPVOID lpParameter );
/* �ṹ���塡*/
// �������˳̴��ݲ���
// ��ʵ����ʹ�ö���д�ļ�����ʾ�˳̵ĵ���
// �û��ɸ���ʵ��������ж���
typedef struct _FIBERDATASTRUCT
{
	DWORD dwParameter;		// Ԥ�������˳̴��ݴ�������
	DWORD dwFiberResultCode;	// GetLastError() ֵ
	HANDLE hFile;				// �˳��������ļ��ľ��
	DWORD dwBytesProcessed;		// �Ѿ������˵��ֽ�
}FIBERDATASTRUCT, *LPFIBERDATASTRUCT;
/* �������塡*/
#define RTN_OK 0			// ����ֵ �ɹ�
#define RTN_USAGE 1			// ����ֵ ��������ȷ
#define RTN_ERROR 2		// ����ֵ ����
#define BUFFER_SIZE 32768	// ��������С
#define FIBER_COUNT 3		// ���˳̡����˳̡�д�˳̣�������
#define PRIMARY_FIBER 0		// ���˳�
#define READ_FIBER 1		// ���˳�
#define WRITE_FIBER 2		// д�˳�

LPVOID g_lpFiber[FIBER_COUNT];	// �˳̵�ַ������
LPBYTE g_lpBuffer;				// ������
DWORD g_dwBytesRead;			// �Ѷ����ֽ�

int main( int argc, char *argv[] )
{
	LPFIBERDATASTRUCT fs;
	// �÷�˵��
	if (argc != 3)
	{
		printf("Usage: %s <SourceFile> <DestinationFile>\n", argv[0]);
		return RTN_USAGE;
	}
	// ����FIBERDATASTRUCT�ռ䣬FIBER_COUNT��
	fs = (LPFIBERDATASTRUCT)HeapAlloc(
		GetProcessHeap(), 0,
		sizeof(FIBERDATASTRUCT) * FIBER_COUNT);
	if (fs == NULL)
	{
		printf("HeapAlloc error! (rc%=lu)\n", GetLastError());
		return RTN_ERROR;
	}
	//	�������д������
	g_lpBuffer = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, BUFFER_SIZE);
	if (g_lpBuffer == NULL)
	{
		printf("HeapAlloc error! (rc=%lu)\n", GetLastError());
		return RTN_ERROR;
	}
	// ��Դ�ļ����������ֵ��fs�ṹ��hFile��Ա��ʹ�˳̿���ʹ�øþ��
	fs[READ_FIBER].hFile = CreateFile(
		argv[1], GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL
		);
	if (fs[READ_FIBER].hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile error! (rc=%lu)\n", GetLastError());
		return RTN_ERROR;
	}
	// ��Ŀ���ļ�
	fs[WRITE_FIBER].hFile = CreateFile(
		argv[2], GENERIC_WRITE,
		0, NULL, CREATE_NEW,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL
		);
	if (fs[WRITE_FIBER].hFile == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile error! (rc=%lu)\n", GetLastError());
		return RTN_ERROR;
	}

	// �����߳��л�Ϊ�˳̣�Ϊ���˳̣�ֻ��ת��Ϊ�˳̺�ſ����л��������˳�
	g_lpFiber[PRIMARY_FIBER]=ConvertThreadToFiber(&fs[PRIMARY_FIBER]);
	if (g_lpFiber[PRIMARY_FIBER] == NULL)
	{
		printf("ConvertThreadToFiber failed! rc=%lu\n", GetLastError());
		return RTN_ERROR;
	}
	// ���˳�����
	fs[PRIMARY_FIBER].dwParameter = 0;
	fs[PRIMARY_FIBER].dwFiberResultCode = 0;
	fs[PRIMARY_FIBER].hFile = INVALID_HANDLE_VALUE;
	// �������˳�
	g_lpFiber[READ_FIBER]=CreateFiber(0,ReadFiberFunc,&fs[READ_FIBER]);
	if (g_lpFiber[READ_FIBER] == NULL)
	{
		printf("CreateFiber error! (rc=%lu)\n", GetLastError());
		return RTN_ERROR;
	}
	// ���˳�ָ����Ϊ���������˳̣�û��ʵ�����壬Ϊ����ʾ�����Ϣʱ������˳�
	fs[READ_FIBER].dwParameter = (DWORD)g_lpFiber[READ_FIBER];
	// ����д�˳�
	g_lpFiber[WRITE_FIBER]=CreateFiber(0,WriteFiberFunc,&fs[WRITE_FIBER]);

	if (g_lpFiber[WRITE_FIBER] == NULL)
	{
		printf("CreateFiber error! (rc=%lu)\n", GetLastError());
		return RTN_ERROR;
	}
	fs[WRITE_FIBER].dwParameter = (DWORD)g_lpFiber[WRITE_FIBER];
	// �л���������ִ��
	SwitchToFiber(g_lpFiber[READ_FIBER]);

	// �ɶ��˳̻�д�˳��л������˳�
	// ��ʾ�����Ϣ
	printf("ReadFiber result == %lu Bytes Processed == %lu\n",
		fs[READ_FIBER].dwFiberResultCode, fs[READ_FIBER].dwBytesProcessed);

	printf("WriteFiber result == %lu Bytes Processed == %lu\n",
		fs[WRITE_FIBER].dwFiberResultCode, fs[WRITE_FIBER].dwBytesProcessed);
	// ɾ����д�˳�
	DeleteFiber(g_lpFiber[READ_FIBER]);
	DeleteFiber(g_lpFiber[WRITE_FIBER]);
	// �ر��ļ�������ͷ��ڴ桢����
	CloseHandle(fs[READ_FIBER].hFile);
	CloseHandle(fs[WRITE_FIBER].hFile);
	HeapFree(GetProcessHeap(), 0, g_lpBuffer);
	HeapFree(GetProcessHeap(), 0, fs);
	return RTN_OK;
}

VOID WINAPI ReadFiberFunc( LPVOID lpParameter )
{
	LPFIBERDATASTRUCT fds = (LPFIBERDATASTRUCT)lpParameter;

	// �жϲ���
	if (fds == NULL)
	{
		printf("Passed NULL fiber data. Exiting current thread.\n");
		return;
	}
	// ��ʾ�˳���Ϣ
	printf("Read Fiber (dwParameter == 0x%lx)\n", fds->dwParameter);
	// ��ʼ��������ֽ���Ϊ0
	fds->dwBytesProcessed = 0;
	// ѭ����
	while (1)
	{
		if (!ReadFile(fds->hFile, g_lpBuffer, BUFFER_SIZE,
			&g_dwBytesRead, NULL))
		{
			break;
		}
		// �ж��ļ��Ƿ��Ѿ�����
		if (g_dwBytesRead == 0) break;

		// �Ѿ�������ֽڣ��ۼ�
		fds->dwBytesProcessed += g_dwBytesRead;

		// ��һ�κ��л���д�˳̣�������������д�뵽Ŀ���ļ�
		printf("Switch To Write");
		SwitchToFiber(g_lpFiber[WRITE_FIBER]);
	}
	// ��������ɣ�׼������ִ�У����ص����˳���
	fds->dwFiberResultCode = GetLastError();
	SwitchToFiber(g_lpFiber[PRIMARY_FIBER]);
}

VOID WINAPI WriteFiberFunc( LPVOID lpParameter )
{
	LPFIBERDATASTRUCT fds = (LPFIBERDATASTRUCT)lpParameter;
	DWORD dwBytesWritten;

	// �жϲ���
	if (fds == NULL)
	{
		printf("Passed NULL fiber data. Exiting current thread.\n");
		return;
	}
	// ��ʾ�˳���Ϣ
	printf("Write Fiber (dwParameter == 0x%lx)\n", fds->dwParameter);
	// ��ʼ����ע��Ͷ��˳̵Ĳ�ͬ
	fds->dwBytesProcessed = 0;
	fds->dwFiberResultCode = ERROR_SUCCESS;

	while (1)
	{
		// д������
		if (!WriteFile(fds->hFile, g_lpBuffer, g_dwBytesRead,
			&dwBytesWritten, NULL))
		{
			// ������������˳�ѭ��
			break;
		}
		fds->dwBytesProcessed += dwBytesWritten;

		// д����ɣ��л������˳�
		printf("Switch To Write");
		SwitchToFiber(g_lpFiber[READ_FIBER]);
	}
	// �����л������˳�
	// ���д�����������ǲ�������д�˳��л������˳̵�
	fds->dwFiberResultCode = GetLastError();
	SwitchToFiber(g_lpFiber[PRIMARY_FIBER]);
}