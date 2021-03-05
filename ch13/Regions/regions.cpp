/* ************************************
*����ͨWindows API��
* ʾ������
* Regions.c
* 13.8 ��������ͷ�ת
**************************************/
/* ͷ�ļ� */
#include <windows.h>
/* ȫ�ֱ��� */
LPSTR szAppName = "RGN";
LPSTR szTitle = "RGN Sample";
/* �������� */
LRESULT CALLBACK WndProc( HWND ,  UINT ,  WPARAM ,  LPARAM );

/*************************************
* ElliRgns
* ����	������Բ���򣬲��������ͷ�ת
**************************************/
HRGN ElliRgns(HWND hwnd, POINT point)
{
	// RECT ���򡢻�ˢ
	RECT rect, rectClient;
	HRGN hrgn;
	HBRUSH hBrushOld, hBrush;
	// DC
	HDC hdc = GetDC(hwnd);
	GetClientRect(hwnd,&rectClient);
	// �����Χһ������
	rect.left = point.x - 40;
	rect.right = point.x + 40;
	rect.top = point.y - 30;
	rect.bottom = point.y + 30;
	// ͨ��RECT ������Բ����
	hrgn = CreateEllipticRgnIndirect(&rect);
	// ������ˢ
	hBrush = CreateSolidBrush(RGB(0,255,0));
	// ΪDC ѡ��ˢ
	hBrushOld = (HBRUSH)SelectObject(hdc,hBrush);
	// ʹ�õ�ǰ��ˢ��������
	PaintRgn(hdc,hrgn);
	// �ȴ�һ��ʱ��󣬽��ղŻ��Ƶ�������з�ת
	Sleep(3000);
	InvertRgn(hdc,hrgn);
	// �ͷ���Դ
	hBrush = (HBRUSH)SelectObject(hdc,hBrushOld);
	DeleteObject(hBrush);
	DeleteObject(hrgn);
	DeleteDC( hdc );
	return 0;
}

int WINAPI WinMain(
				   HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	MSG msg;
	HWND hWnd;
	WNDCLASS wc;

	wc.style = CS_OWNDC; 
	wc.lpfnWndProc = (WNDPROC)WndProc; 
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hInstance = hInstance;
	wc.hIcon = NULL; 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szAppName; 

	if (!RegisterClass(&wc))
	{
		return (FALSE);
	}

	hWnd = CreateWindow(
		szAppName, 
		szTitle, 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
		NULL, 	NULL,	hInstance, NULL 
		);

	if (!hWnd)
	{
		return (FALSE);
	}

	ShowWindow(hWnd, nCmdShow); 
	UpdateWindow(hWnd); 

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)(msg.wParam);

	UNREFERENCED_PARAMETER(lpCmdLine); 
}


LRESULT CALLBACK WndProc(
						 HWND hWnd, 
						 UINT message, 
						 WPARAM uParam, 
						 LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
		break;

	case WM_LBUTTONDOWN:
		{
			// ��õ����λ�ã����ݸ�ElliRgns
			POINT point;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			ElliRgns(hWnd, point);
		}
		break;

	case WM_DESTROY: 
		PostQuitMessage(0);
		break;

	default: 
		return (DefWindowProc(hWnd, message, uParam, lParam));
	}
	return (0);
}