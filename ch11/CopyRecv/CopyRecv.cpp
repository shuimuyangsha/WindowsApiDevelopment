/* ************************************
*����ͨWindows API��
* ʾ������
* CopyRecv.c
* 11.4 ͨ��WM_COPYDATA���̼�ͨ��
**************************************/
/* ͷ�ļ� */
#include <windows.h>
#include <commctrl.h>
/* ȫ�ֱ��� */
HINSTANCE hinst;
HWND hwnd,hwndEdit;
/* �������� */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

/* ************************************
* void main()
* ���� ���̼�mailslotͨ�ſͻ���
**************************************/
int WINAPI WinMain(
				   HINSTANCE hinstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	MSG msg;
	WNDCLASSEX wcx;
	// ���WNDCLASSEX
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = MainWndProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hinstance;
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = "copydata2";
	wcx.hIconSm = NULL;
	// ע�ᴰ����
	if(RegisterClassEx(&wcx) == 0)
	{
		return 0;
	}
	// ��������
	hwnd = CreateWindow(
		"copydata2",
		"RECV COPY DATA",
		WS_TILEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
		(HWND) NULL, (HMENU) NULL, hinstance, (LPVOID) NULL
		);

	if (!hwnd)
		return 0;
	// ��ʾˢ�´���
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	// ��Ϣ
	BOOL fGotMessage;
	while ((fGotMessage = GetMessage(&msg, (HWND) NULL, 0, 0)) != 0 && fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
	UNREFERENCED_PARAMETER(lpCmdLine);
	return 0;
}

// ��Ϣ������
LONG APIENTRY MainWndProc(HWND hwnd,
						  UINT message,
						  WPARAM wParam,
						  LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		{
			// ����������ʱ������һ��EDIT�ؼ�
			RECT rectMain;
			GetClientRect(hwnd,&rectMain);
			hwndEdit = CreateWindow("EDIT",
				NULL,
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER |
				ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
				0., 0, rectMain.right, rectMain.bottom,
				hwnd,
				NULL,
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);
			if(hwndEdit == NULL)
			{
				MessageBox(hwnd, "Create Window Error","ERROR",MB_OK);
				ExitProcess(0);
			}
			break;
	case WM_COPYDATA:
		{
			// ���յ�WM_COPYDATA��Ϣʱ����lParam����ȡ��
			// COPYDATASTRUCT
			PCOPYDATASTRUCT lpcds = (PCOPYDATASTRUCT)lParam;
			// �����Ƶ������ڽ�������ʾ
			SetWindowText(hwndEdit,(LPSTR)lpcds->lpData);
			break;
		}

	case WM_DESTROY: // �˳�
		PostQuitMessage(0);
		return 0;

	default: // ������Ϣ����
		break;
		}
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}