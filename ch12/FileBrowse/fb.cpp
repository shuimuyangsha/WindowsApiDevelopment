/* ************************************
*《精通Windows API》 
* 示例代码
* fb.c
* 12.1.4  浏览文件夹对话框
**************************************/
/* 头文件　*/
#include <Windows.h>
#include <shlobj.h>
/* 函数申明　*/
DWORD Browse(HWND hwnd) ;

/*************************************
* WinMain
* 功能	程序入口点，调用Browse
*
* 参数	未使用
**************************************/
int WINAPI WinMain(
				   HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow
				   )
{
	Browse(NULL); 
}

/*************************************
* WinMain
* 功能	弹出“浏览文件夹”对话框，
并获取用户选择的文件夹目录
*
* 参数	HWND hwnd	父窗口句柄
**************************************/
DWORD Browse(HWND hwnd) 
{
	// 用于保存路径
	CHAR szRoot[MAX_PATH];
	CHAR szChoose[MAX_PATH];
	CHAR szDisplayName[MAX_PATH];
	// 相关变量
	LPITEMIDLIST pidlRoot = NULL;
	LPITEMIDLIST pidlSelected = NULL;
	BROWSEINFO bi = {0};
	LPMALLOC pMalloc = NULL;

	// “浏览文件夹”的根路径，开发人员可根据情况选择，比如只浏览“我的文档”。
	SHGetFolderLocation(NULL, CSIDL_DESKTOP, NULL, 0, &pidlRoot);
	SHGetPathFromIDList(pidlRoot,szRoot);
	// 填充 BROWSEINFO 结构
	bi.hwndOwner = hwnd;
	bi.pidlRoot = pidlRoot;
	bi.pszDisplayName = szDisplayName;
	bi.lpszTitle = "Choose a target";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	// 弹出对话框
	pidlSelected = SHBrowseForFolder(&bi);
	// DisplayName
	MessageBox(NULL,szDisplayName,"Display Name:",MB_OK);
	// 选择的文件夹
	SHGetPathFromIDList( pidlSelected, szChoose );
	MessageBox(NULL,szChoose,"Choose:",MB_OK);
	// 释放
	ILFree(pidlRoot);
	return 0;
}