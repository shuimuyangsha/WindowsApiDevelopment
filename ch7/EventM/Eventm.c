/* ************************************
*《精通Windows API》 
* 示例代码
* Eventm.c
* 7.2.1  演示使用Event同步线程
**************************************/

/* 头文件　*/
#include <windows.h>
#include <stdio.h>
/* 常量定义　*/
#define NUMTHREADS	3 
#define BUFFER_SIZE	16
#define FOR_TIMES	5
/* 全局变量　*/
HANDLE hWriteEvent[NUMTHREADS];		// 写Event 表示写操作是否完成
HANDLE hReadEvents[NUMTHREADS];	// 读Event 表示读操作是否完成
BYTE lpSharedBuffer[16] = {0};	// 共享内存
/* 函数声明　*/
void MultiEvents(void);
VOID WriteToBuffer(VOID);
DWORD WINAPI ThreadFunction(LPVOID lpParam);

/*************************************
* int main(void)
* 功能	演示
*
* 参数	未使用
**************************************/
int main()
{
	MultiEvents();
}

/*************************************
* void UseEvents(void) 
* 功能	演示Event的使用方法
*
* 参数	未使用
**************************************/
void MultiEvents(void) 
{
	HANDLE hThread; 
	DWORD i; 
	// 创建多个线程，读共享内存，主线程写共享内存。
	// 每个线程都有对应的读写同步事件
	for(i = 0; i < NUMTHREADS; i++) 
	{
		// 每个线程都有一个Event表示写入操作完成
		hWriteEvent[i] = CreateEvent( 
			NULL,			// 默认安全属性
			FALSE,			// 自动重置
			FALSE,			// 初始为未置位的
			NULL			// 未命名
			);
		// 判断是否创建成功
		if (hWriteEvent[i] == NULL) 
		{ 
			printf("CreateEvent failed (%d)\n", GetLastError());
			return;
		}
		// 每个读线程有一个Event表示读操作已经完成
		hReadEvents[i] = CreateEvent( 
			NULL,     // 默认安全属性
			FALSE,	  // 自动重置
			FALSE,    // 初始化为未置位的
			NULL);    // 未命名
		if (hReadEvents[i] == NULL) 
		{
			printf("CreateEvent failed (%d)\n", GetLastError());
			return;
		}
		// 创建线程
		hThread = CreateThread(NULL, 0, 
			ThreadFunction, 
			(LPVOID)i,  // Event对象句柄作为
			0, NULL); 
		if (hThread == NULL) 
		{
			printf("CreateThread failed (%d)\n", GetLastError());
			return;
		}
	}
	WriteToBuffer();
}

/*************************************
* VOID WriteToBuffer(INT iContent) 
* 功能	由主线程调用，向共享内存中写入数据
*		等待所有读线程读完后函数返回
*
* 参数	未使用
**************************************/
VOID WriteToBuffer(VOID) 
{
	DWORD dwWaitResult, j,i;
	// 完成 FOR_TIMES 次读写
	for (j = 0; j < FOR_TIMES; j++)
	{		
		Sleep(rand()%100);	// 写入需要的时间随机
		// 写入共享内存
		wsprintf(lpSharedBuffer,"shared %d",j);
		// 将线程对应的写Event置为“标志的”，表示写操作完成，
		// 其他线程可以开始读
		for(i=0; i<NUMTHREADS; i++)
		{
			if (! SetEvent(hWriteEvent[i]) ) 
			{ 
				printf("SetEvent failed (%d)\n", GetLastError());
				return;
			}
		}
		// 等待所有的线程读完，开始下次写入
		dwWaitResult = WaitForMultipleObjects( 
			NUMTHREADS,   // Event句柄的个数
			hReadEvents,  // Event句柄数组
			TRUE,         // 等到所有的Event都被标志
			INFINITE);    // 无限等待
		// 判断等待结果
		if (dwWaitResult != WAIT_OBJECT_0) 
		{
			printf("Wait error: %d\n", GetLastError()); 
			ExitProcess(0); 
		}
	}
}

/*************************************
* DWORD WINAPI ThreadFunction(LPVOID lpParam)
* 功能	线程函数，读共享内存
*
* 参数	LPVOID lpParamt 实际为指向Event句柄的指针
**************************************/
DWORD WINAPI ThreadFunction(LPVOID lpParam) 
{
	DWORD dwWaitResult;
	BYTE lpRead[16];
	DWORD j = 0;
	DWORD dwThreadIndex  = (DWORD)lpParam;
	// 完成 FOR_TIMES 次读写
	for(; j<FOR_TIMES; j++)
	{
		// 等待写事件置位，表示数据已经写入
		dwWaitResult = WaitForSingleObject( 
			hWriteEvent[dwThreadIndex], // Event 句柄
			INFINITE);					// 无限等待
		switch (dwWaitResult) 
		{
		case WAIT_OBJECT_0:
			Sleep(rand()%10);			// 模拟数据处理所需的时间随机
			CopyMemory(lpRead,lpSharedBuffer,16);
			break; 
			// 发生错误
		default: 
			printf("Wait error: %d\n", GetLastError()); 
			ExitThread(0); 
		}
		// 将读Event置位，表示读操作完成
		if (! SetEvent(hReadEvents[dwThreadIndex]) ) 
		{ 
			printf("SetEvent failed (%d)\n", GetLastError());
			return 0;
		}
		//打印读到的内容
		printf("线程 %u\t第 %d 次读，内容：%s\n",
			dwThreadIndex,j,(LPSTR)lpRead);
	}
	return 1;
}