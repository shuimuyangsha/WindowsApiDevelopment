/* ************************************
*《精通Windows API》 
* 示例代码
* Semaphorec
* 7.2.3  演示使用信号量同步线程
**************************************/
/* 头文件　*/
#include <windows.h>
#include <stdio.h>
/* 常量定义　*/
#define NUMTHREADS	4 
/* 全局变量　*/
HANDLE hSemaphore;
/* 函数声明　*/
void UseSemaphore(void);
DWORD WINAPI SemaphoreThread(LPVOID lpParam);

/*************************************
* int main(void)
* 功能	演示
*
* 参数	未使用
**************************************/
int main()
{
	 UseSemaphore() ;
}
/*************************************
* DWORD WaitForAllThread(HANDLE hThread[], DWORD dwNumThread)
* 功能	等待指定的线程都结束
*
* 参数	HANDLE hThread[]	需要等待结束的线程句柄数组
*		DWORD dwNumThread	线程句柄数组的大小
**************************************/
DWORD WaitForAllThread(HANDLE hThread[], DWORD dwNumThread)
{
	// 等待所有线程结束
	DWORD dwWaitResult = WaitForMultipleObjects(
		dwNumThread,
		hThread,				// 线程句柄作为等待对象
		TRUE,
		INFINITE);
	switch (dwWaitResult) 
		{
		case WAIT_OBJECT_0: 
			printf("\nAll thread exit\n"); 
			break; 
		default: 
			printf("\nWait error: %u", GetLastError()); 
		}
	return 0;

}

/*************************************
* void UseSemaphore(void) 
* 功能	演示UseSemaphore的使用方法
*
* 参数	未使用
**************************************/
void UseSemaphore(void) 
{
	HANDLE hThread[NUMTHREADS];
	INT i;
	LONG lMax;
	CHAR cMax;
	// 打印信息获取输入
	printf("将创建%d个进程，获得信号量的进程可以向屏幕打印。\n"
		"请输入信号量的最大计数1~%d：",NUMTHREADS,NUMTHREADS);	
	// 获得输入的字符
	cMax = getch();
	printf("%c\n",cMax);
	// 将字符转换为数字
	lMax = cMax & 0xF;
	if(lMax<0 || lMax>NUMTHREADS)
	{
		printf("请输入1-%d",NUMTHREADS);
	}
	// 创建信号量
	hSemaphore = CreateSemaphore( 
		NULL,   // 默认安全属性
		lMax,   // 初始化计数器，用户输入
		lMax,   // 最大计数，用户输入
		NULL);  // 未命名
	if (hSemaphore == NULL) 
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
	}
	// 创建多个线程 访问共享资源
	for(i = 0; i < NUMTHREADS; i++) 
	{		
		hThread[i] = CreateThread(NULL, 0, 
			SemaphoreThread, 
			&i,			// 进程序号作为参数
			0, NULL); 
		if (hThread[i] == NULL) 
		{
			printf("CreateThread failed (%d)\n", GetLastError());
			return;
		}
	}
	// 等待所有线程都执行完成并退出
	WaitForAllThread(hThread,NUMTHREADS);
}

/*************************************
* DWORD WINAPI SemaphoreThread(LPVOID lpParam) 
* 功能	线程函数，读共享内存
*
* 参数	未使用
**************************************/
DWORD WINAPI SemaphoreThread(LPVOID lpParam) 
{
	DWORD dwWaitResult;
	BYTE lpRead[16];
	//DWORD i = *(LPWORD)lpParam;		// 线程的编号
	DWORD dwPreviousCount;

	DWORD j = 0;
	// 每个线程都将访问3次受限资源
	for(; j<3; j++)
	{		
		// 线程可以在此进行一些操作
		// 以暂停随机长的时间模拟真实情况
		Sleep(rand()%1000);
		// 等待信号量
		dwWaitResult = WaitForSingleObject( 
			hSemaphore,      // 信号量句柄
			INFINITE);		 // 无限等待
		switch (dwWaitResult) 
		{
		case WAIT_OBJECT_0: 
			printf("\nProcess %d Gets Semaphore",GetCurrentThreadId());
			break; 
		default: 
			printf("\nprocess %u Wait error: %u",GetCurrentThreadId(), GetLastError()); 
		}
		// 获得信息量后访问受限资源
		// 以等待随机长时间模块真实情况
		Sleep(rand()%1000);
		// 释放信号量
		if (!ReleaseSemaphore( 
			hSemaphore,				// 信号量句柄
			1,						// 释放后计数器减1 
			&dwPreviousCount) )		// 获得计数
		{
			printf("\nprocess %u ReleaseSemaphore error: %d", GetCurrentThreadId(), GetLastError());
		}
		else
		{
			printf("\nProcess %u release Semaphore, previous count is %u",
				GetCurrentThreadId(), dwPreviousCount);
		}
	}

	return 1;
}