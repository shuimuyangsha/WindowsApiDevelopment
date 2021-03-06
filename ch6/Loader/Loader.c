/* ************************************
*《精通Windows API》
* 示例代码
* Loader.c
* 6.5  动态链接库
**************************************/
/* 头文件　*/ 
#include <stdio.h> 
#include <windows.h> 
/* 类型定义　*/

// 指向函数的指针类型
typedef DWORD (*MY_EXPORT_PROC)(LPSTR, DWORD);

// main
VOID main(VOID) 
{ 
	HINSTANCE hinstLib; 
	MY_EXPORT_PROC lpProcAdd; 
	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE; 

	// 获得Dll模块句柄
	hinstLib = LoadLibrary(TEXT("msg.dll")); 

	// 是否加载成功
	if (hinstLib != NULL) 
	{ 
		// 获得指定导出函数的地址
		lpProcAdd = (MY_EXPORT_PROC) GetProcAddress(hinstLib, "ExportExample"); 

		// 判断是否成功
		if (NULL != lpProcAdd) 
		{
			fRunTimeLinkSuccess = TRUE;
			(lpProcAdd) ("hello",123); 
		} 
		// 释放DLL 
		fFreeResult = FreeLibrary(hinstLib); 
	} 

	// 出错
	if (! fRunTimeLinkSuccess) 
		printf("error; %u\n",GetLastError()); 
}