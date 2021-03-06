/* ************************************
*《精通Windows API》 
* 示例代码
* start.c
* 1.1.1	第一个示例程序，弹出消息对话框
**************************************/

/* 预处理　*/
/* 头文件　*/
#include <windows.h>
//连接时使用User32.lib
#pragma comment (lib, "User32.lib")

/* ************************************
* WinMain
* 功能	Windows应用程序示例
**************************************/
int WinMain(
			HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR lpCmdLine,
			int nCmdShow
			)
{
	// 调用API函数 MessageBox
	MessageBox(NULL,
		TEXT("开始学习Windows编程"),
		TEXT("消息对话框"),
		MB_OK);

	MessageBox(NULL,
		TEXT("hello Windows！"),
		TEXT("对话框"),
		MB_OK);

	return 0;
}