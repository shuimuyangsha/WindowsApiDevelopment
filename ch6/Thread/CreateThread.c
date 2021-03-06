/* ************************************
*《精通Windows API》
* 示例代码
* CreateThread.c
* 6.3.1  创建线程、退出线程、获取线程信息
**************************************/
/* 头文件　*/
#include <windows.h>
#include <stdio.h>
/* 常量定义　*/
#define MAX_THREADS 5
/* 结构类型　*/
typedef struct _THREAD_PARAM {
	DWORD i;
	DWORD dwRandom;
	DWORD dwData; 
} THREAD_PARAM, *LPTHREAD_PARAM;

/*************************************
* DWORD WINAPI ThreadProc( LPVOID lpParam )
* 功能	线程函数
*		将参数打印出
*
* 参数	LPVOID lpParam	THREAD_PARAM 结构类型
**************************************/
DWORD WINAPI ThreadProc( LPVOID lpParam ) 
{ 
	LPTHREAD_PARAM pData;
	// 参数数据类型
	pData = (LPTHREAD_PARAM)lpParam;
	// 显示参数
	printf("TID = %u,\t Parameters = %u, %u,  %u\n", 
		GetCurrentThreadId(), 
		pData->i, pData->dwRandom, pData->dwData); 
	// 释放保存参数的内存（在主线程中分配的）.
	HeapFree(GetProcessHeap(), 0, pData);
	return 0; 
} 
/*************************************
* void main()
* 功能	主线程函数，创建了多个线程
**************************************/
void main()
{
	LPTHREAD_PARAM pData;
	DWORD dwThreadId[MAX_THREADS];
	HANDLE hThread[MAX_THREADS]; 
	int i;

	// 创建MAX_THREADS 个线程.
	for( i=0; i<MAX_THREADS; i++ )
	{
		// 为线程函数参数分配内存
		pData = (LPTHREAD_PARAM)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
			sizeof(THREAD_PARAM));
		if( pData == NULL )
		{
			printf("HeapAlloc error;\n");
			ExitProcess(2);
		}
		// 设置参数
		pData->i = i;
		pData->dwRandom = rand();
		pData->dwData = 100;
		// 创建线程
		hThread[i] = CreateThread( 
			NULL,              // 默认安全属性
			0,                 // 默认堆栈大小 
			ThreadProc,        // 线程函数
			pData,             // 参数
			0,                 // 默认创建标志
			&dwThreadId[i]);   // 返回TID
		// 判断是否创建成功
		if (hThread[i] == NULL) 
		{
			ExitProcess(i);
		}
	}
}