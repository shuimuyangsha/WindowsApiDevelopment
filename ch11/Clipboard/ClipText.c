/* ************************************
*����ͨWindows API��
* ʾ������
* ClipText.c
* 11.3.2 ����ճ����
**************************************/
/* ͷ�ļ� */
#include "cliptext.h"
#include <string.h>
/* Ԥ���� */
#ifdef UNICODE// UNICODE
#define MY_CLIP_FORMAT CF_UNICODETEXT
#else
#define MY_CLIP_FORMAT CF_TEXT
#endif
/* ȫ�ֱ��� */
HINSTANCE hInst;
HACCEL hAccTable;
HWND hwnd;
LPTSTR lpszText = NULL;
LPTSTR lpData = NULL; // ճ��������ָ��
HANDLE hHeap;
LPTSTR szInitialClientAreaText =
TEXT("This program demonstrates the use of the Edit menu to copy and \n")
TEXT("paste text to and from the clipboard. Try using the Copy command \n")
TEXT("to move this text to the clipboard, and the Paste command to replace \n")
TEXT("this text with data from another application. \r\n\r\n")
TEXT("You might want to try running Notepad and Clipbrd alongside this \n")
TEXT("application so that you can watch the data exchanges take place. \n");

/* �������� */
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, INT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
VOID OutOfMemory(VOID);

/* ************************************
* WinMain
* ���� ������ڵ㣬����InitApplication��InitInstance
* ��Ϣѭ��
* ���� hinstance��Ӧ�ó��򱾴�����ʵ��
* hPrevInstance��Ӧ�ó���֮ǰ��ʵ����ʼ��ΪNULL
* lpCmdLine�������в���
* nCmdShow��������ڣ�SW_SHOW
* ����ֵ ʧ�ܷ���FALSE
**************************************/
int WINAPI WinMain(
				   HINSTANCE hinstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	MSG msg;
	BOOL fGotMessage;
	// InitApplication
	if (!InitApplication(hinstance))
	{
		return FALSE;
	}
	// InitInstance
	if (!InitInstance(hinstance, nCmdShow))
	{
		return FALSE;
	}
	// ��Ϣѭ����ֻ�������ڵ���Ϣ
	while ((fGotMessage = GetMessage(&msg, (HWND) hwnd, 0, 0)) != 0 && fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
	UNREFERENCED_PARAMETER(lpCmdLine);
}

/* ************************************
* BOOL InitApplication(HINSTANCE hinstance)
* ���� ע����������
* ���� hinstance��Ӧ�ó��򱾴�����ʵ��
* ����ֵ �Ƿ�ɹ�
**************************************/
BOOL InitApplication(
					 HANDLE hInstance)
{
	WNDCLASS wc;

	wc.lpfnWndProc = (WNDPROC) WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("CliptextWClass");
	wc.style = 0 ;
	wc.cbClsExtra =0;
	wc.cbWndExtra =0;

	return (RegisterClass(&wc));
}

/* ************************************
* BOOL InitInstance(HINSTANCE hinstance, int nCmdShow)
* ���� ����������
* ���� hinstance��Ӧ�ó��򱾴�����ʵ��
* nCmdShow�������ʾ
* ����ֵ �Ƿ�ɹ�
**************************************/
BOOL InitInstance(
				  HINSTANCE hInstance,
				  INT nCmdShow)
{
	// ����ʵ��
	hInst = hInstance;
	// ����Ѿ��������ĳ�������ڴ�
	hHeap = GetProcessHeap();
	// Accelerators
	hAccTable = LoadAccelerators(hInst, TEXT("ClipTextAcc"));
	// ΪlpszText�����ڴ棬lpszText�������ڽ�������ʾ���ı�
	lpszText = HeapAlloc(hHeap,0,lstrlen(szInitialClientAreaText)+1);
	// ���Ƴ�ʼ��ʾ���ַ���
	lstrcpy(lpszText, szInitialClientAreaText);
	// ��������
	hwnd = CreateWindow(
		TEXT("CliptextWClass"),
		TEXT("Cliptext Sample Application"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0,
		NULL,
		(HMENU)LoadMenu(hInstance,MAKEINTRESOURCE(ID_CLIP_MENU)),
		hInstance,
		NULL );
	if (!hwnd)
	{
		return FALSE;
	}
	// ��ʾ��ˢ�´���
	ShowWindow(hwnd, nCmdShow);
	return UpdateWindow(hwnd);
}

/* ************************************
* WndProc
* ������Ϣ����
**************************************/
LRESULT CALLBACK WndProc(
						 HWND hWnd,
						 UINT message,
						 WPARAM wParam,
						 LPARAM lParam)
{
	HDC hDC = NULL;
	PAINTSTRUCT ps = {0};
	RECT rectClient = {0};
	LPTSTR lpClipData = NULL;

	switch (message)
	{
	case WM_INITMENU: // ��ʼ���˵�
		if (wParam == (WPARAM)GetMenu(hWnd))
		{
			if (OpenClipboard(hWnd))
			{
				// ��ʽ�Ƿ����
				if (IsClipboardFormatAvailable(MY_CLIP_FORMAT) || 
					IsClipboardFormatAvailable(CF_OEMTEXT))
					// �˵���
				{
					EnableMenuItem((HMENU)wParam, IDM_PASTE, MF_ENABLED);
				}
				else
				{
					EnableMenuItem((HMENU)wParam, IDM_PASTE, MF_GRAYED);
				}
				// �ر�ճ����
				CloseClipboard();
				return (TRUE);
			}
			else /* Clipboard is not available */
			{
				return (FALSE);
			}
		}
		return (TRUE);

	case WM_COMMAND: // �˵�����
		switch(LOWORD(wParam))
		{
		case IDM_ABOUT:
			break;
		case IDM_NEW:
		case IDM_OPEN:
		case IDM_SAVE:
		case IDM_SAVEAS:
		case IDM_PRINT:
		case IDM_UNDO:
		case IDM_CLEAR:
			MessageBox (GetFocus (),
				TEXT("Command not implemented."),
				TEXT("ClipText Sample Application"),
				MB_ICONASTERISK | MB_OK);
			// ���ϲ�����

		case IDM_EXIT: // �˳�
			DestroyWindow(hWnd);
			break;

		case IDM_CUT:// ���к͸��ƽ�������ʾ������
		case IDM_COPY:
			if (lpszText != NULL) // �����Ƿ�����ʾ������
			{
				if(lpData != NULL)
				{
					HeapFree(hHeap,0,lpData);
				}
				// Ϊճ�������ݷ���ռ�
				lpData = HeapAlloc(hHeap,0,HeapSize(hHeap,0,lpszText));
				// ����
				lstrcpy(lpData, lpszText);

				if (OpenClipboard(hWnd)) // ��ճ����
				{
					// ����ճ��������
					EmptyClipboard();
					SetClipboardData(MY_CLIP_FORMAT, lpData);
				}
				CloseClipboard();// �ر�

				if (LOWORD(wParam) == IDM_CUT)
				{
					//����Ǽ��У������Ļ���ݣ����ò˵�
					HeapFree(hHeap,0,lpszText);
					lpszText = 0;
					EnableMenuItem(GetMenu (hWnd), IDM_CUT, MF_GRAYED);
					EnableMenuItem(GetMenu(hWnd), IDM_COPY, MF_GRAYED);
					InvalidateRect (hWnd, NULL, TRUE);
					UpdateWindow (hWnd);
				}
			}
			return (TRUE);

		case IDM_PASTE:
			if (OpenClipboard(hWnd))
			{
				// ��ճ�������ı�
				if (!(lpClipData = GetClipboardData(MY_CLIP_FORMAT)))
				{
					CloseClipboard();
					break;
				}
				// Ϊ�µ���ʾ�ı�����ռ�
				if(lpszText != NULL)
				{
					HeapFree(hHeap,0,lpszText);
					lpszText = NULL;
				}
				lpszText = HeapAlloc(hHeap,0,lstrlen(lpClipData)+1);
				//����
				lstrcpy(lpszText, lpClipData);
				CloseClipboard();// �ر�
				// ���ò˵���
				EnableMenuItem(GetMenu(hWnd), IDM_CUT, MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd), IDM_COPY, MF_ENABLED);

				// ��ʾ
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
				return (TRUE);
			}
			else
			{
				return (FALSE);
			}
		}
		break;

	case WM_SIZE:
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		// ���ı���ʾ�ڽ�����
		hDC = BeginPaint (hWnd, &ps);

		if (lpszText != NULL)
		{
			GetClientRect (hWnd, &rectClient);
			DrawText (hDC, lpszText, -1, &rectClient,
				DT_EXTERNALLEADING | DT_NOPREFIX | DT_WORDBREAK);

		}
		EndPaint (hWnd, &ps);
		break;

	case WM_DESTROY:
		//�ͷ��ڴ棬�˳�
		if (lpszText != NULL)
		{
			HeapFree(hHeap,0,lpszText);
		}
		if(lpData != NULL)
		{
			HeapFree(hHeap,0,lpData);
		}
		PostQuitMessage(0);
		break;

	default:
		return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	return (0);
}