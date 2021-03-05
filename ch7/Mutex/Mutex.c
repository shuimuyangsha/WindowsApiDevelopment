/* ************************************
*����ͨWindows API�� 
* ʾ������
* Mutex.c
* 7.2.2  ��ʾʹ��Mutexͬ���߳�
**************************************/

/* ͷ�ļ���*/
#include <windows.h>
#include <stdio.h>
/* �������塡*/
#define NUM_THREADS	4 
/* ȫ�ֱ�����*/
DWORD dwCounter = 0;
HANDLE hMutex; 
/* ����������*/
void UseMutex(void);
DWORD WINAPI MutexThread(LPVOID lpParam);

/*************************************
* int main(void)
* ����	��ʾ
*
* ����	δʹ��
**************************************/
int main()
{
	UseMutex();
}
/*************************************
* void UseMutex(void)
* ����	��ʾ Mutex ��ʹ�÷���
*
* ����	δʹ��
**************************************/
void UseMutex(void) 
{
	INT i;
	HANDLE hThread;

#ifdef MUTEX
	// ���� Mutex
	hMutex = CreateMutex( 
		NULL,                       // Ĭ�ϰ�ȫ����
		FALSE,                      // ��ʼ��Ϊδ��ӵ��
		NULL);                      // δ����
	if (hMutex == NULL) 
	{
		printf("CreateMutex error: %d\n", GetLastError());
	}
#endif
	// ��������߳�
	for(i = 0; i < NUM_THREADS; i++) 
	{		
		hThread = CreateThread(NULL, 0, 
			MutexThread, 
			NULL, 
			0, NULL); 
		if (hThread == NULL) 
		{
			printf("CreateThread failed (%d)\n", GetLastError());
			return;
		}
	}
	Sleep(1000);
}

/*************************************
* DWORD WINAPI MutexThread(LPVOID lpParam) 
* ����	�̺߳������������ڴ�
*
* ����	δʹ��
**************************************/
DWORD WINAPI MutexThread(LPVOID lpParam) 
{
	
#ifdef MUTEX 
	DWORD dwWaitResult;
	dwWaitResult = WaitForSingleObject( 
		hMutex,			// ���
		INFINITE);		// ���޵ȴ�

	switch (dwWaitResult) 
	{
	case WAIT_OBJECT_0: 
#endif
		// �ȴ��������ʱ�䣬�����̵߳ȴ���ʱ�佫��һ��
		Sleep(rand()%100);
		// �õ��������� ���ʹ�������
		printf("counter: %d\n",dwCounter);
		// �������֤ͬһʱ��ֻ��һ���߳��ڷ���dwCounter
		dwCounter++;

#ifdef MUTEX
		// �ͷ� Mutex
		if(!ReleaseMutex(hMutex))
		{
			printf("Release Mutex error: %d\n", GetLastError()); 
		}
		break; 
	default: 
		printf("Wait error: %d\n", GetLastError()); 
		ExitThread(0); 
	}
#endif
	return 1;
}
