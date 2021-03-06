/* ************************************
*《精通Windows API》 
* 示例代码
* TaskbarDemo.cpp
* 12.4 通知区域图标
**************************************/
/*  预编译声明　*/
#include "stdafx.h"
#include "resource.h"
#include "SystemTraySDK.h"

#define MAX_LOADSTRING 100
#define	WM_ICON_NOTIFY WM_APP+10

/*  全局变量　*/
HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];					
TCHAR szWindowClass[MAX_LOADSTRING];			
CSystemTray TrayIcon;

/*  函数声明　*/
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

/* ************************************
* WinMain
* 入口函数
**************************************/
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// 字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TASKBARDEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 初始化
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TASKBARDEMO);
	// 消息循环
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

/* ************************************
* ATOM MyRegisterClass(HINSTANCE hInstance)
* 功能	注册窗口类
**************************************/
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TASKBARDEMO);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_TASKBARDEMO;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TASKBARDEMO);

	return RegisterClassEx(&wcex);
}
/* ************************************
* BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
* 功能	创建窗口和图标
**************************************/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; 

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	// 创建通知区域图标
	if (!TrayIcon.Create(hInstance,
		hWnd,																	// 父窗口
		WM_ICON_NOTIFY,											// Notify
		_T("This is a Tray Icon - Right click on me!"),	// tooltip
		::LoadIcon(hInstance, (LPCTSTR)IDI_TASKBARDEMO),
		IDR_POPUP_MENU)) 
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

/* ************************************
* WndProc
* 功能	主窗口消息函数
**************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{

	case WM_ICON_NOTIFY:
		return TrayIcon.OnTrayNotification(wParam, lParam);

	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// 菜单
		switch (wmId)
		{
			// 图标右键菜单中选择 Action
		case IDM_ANIMATE:
			// 动画
			TrayIcon.SetIconList(IDI_ICON1, IDI_ICON4);
			TrayIcon.Animate(50, 2); 
			// 显示气泡通知
			TrayIcon.ShowBalloon("THIS IS A BALLOON",":)",0,10);
			// 从通知区域恢复原始大小
			TrayIcon.MaximiseFromTray(hWnd);
			break; 
			// 退出
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
			// 隐藏，最小化到通知区域
		case IDM_HIDE:
			TrayIcon.MinimiseToTray(hWnd);
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		RECT rt;
		GetClientRect(hWnd, &rt);
		DrawText(hdc, szHello, strlen(szHello), &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE |DT_WORDBREAK);
		EndPaint(hWnd, &ps);
		break;
		// 退出
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}