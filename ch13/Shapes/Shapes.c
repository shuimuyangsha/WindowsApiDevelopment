/* ************************************
*����ͨWindows API��
* ʾ������
* Shapes.c
* 13.6 ����ͼ��
**************************************/
/* ͷ�ļ� */
#include <Windows.h>
#include "resource.h"
/* ȫ�ֱ��� */
HDC hdcCompat; // handle to DC for bitmap 
POINT pt;      // ���λ��
RECT rcTarget; // �������� 
RECT rcClient; // �ͻ���RECT
// ���Ƶ�״̬
BOOL fSizeEllipse; 
BOOL fDrawEllipse;  
BOOL fDrawRectangle; 
BOOL fSizeRectangle; 
BOOL fSizeRoundRect;
BOOL fDrawRoundRect; 
// Բ����Բ��͸ߣ�
int nEllipseWidth;   
int nEllipseHeight; 

BOOL CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, 
					  LPARAM lParam) 
{ 
	HDC hdc;          // DC ��� 
	PAINTSTRUCT ps;   
	POINT ptClientUL; // �ͻ������Ͻ�
	POINT ptClientLR; // �ͻ������½�

	switch (uMsg) 
	{ 
	case WM_COMMAND: 
		switch (wParam)	// �˵����룬������Ҫ���Ƶ�ͼ��
		{ 
		case ID_SHAPE_ELLIPSE: 
			fSizeEllipse = TRUE; 
			break;  
		case ID_SHAPE_RECTANGLE: 
			fSizeRectangle = TRUE; 
			break;  
		case ID_SHAPE_ROUNDRECT: 
			fSizeRoundRect = TRUE; 
			break; 

		default: 
			return DefWindowProc(hwnd, uMsg, wParam, 
				lParam); 
		} 
		break;  

	case WM_CREATE: 
		// ��ʼ��Բ�ǵĳ��Ϳ�
		nEllipseWidth = 20; 
		nEllipseHeight = 20;  
		return 0; 

	case WM_PAINT:// ����
		BeginPaint(hwnd, &ps); 
		// ѡ��ˢ
		SelectObject(ps.hdc, GetStockObject(GRAY_BRUSH)); 

		if (fDrawEllipse) // �������Բ
		{
			// ����Ellipse
			Ellipse(ps.hdc, rcTarget.left, rcTarget.top, 
				rcTarget.right, rcTarget.bottom); 
			fDrawEllipse = FALSE; 
			rcTarget.left = rcTarget.right = 0; 
			rcTarget.top = rcTarget.bottom = 0; 
		} 

		if (fDrawRectangle) // ����о���
		{
			// ����Rectangle
			Rectangle(ps.hdc, rcTarget.left, rcTarget.top, 
				rcTarget.right, rcTarget.bottom); 
			fDrawRectangle = FALSE; 
			rcTarget.left = rcTarget.right = 0; 
			rcTarget.top = rcTarget.bottom = 0; 
		} 

		if (fDrawRoundRect) // �����Բ�Ǿ���
		{
			// ����RoundRect
			RoundRect(ps.hdc, rcTarget.left, rcTarget.top, 
				rcTarget.right, rcTarget.bottom, 
				nEllipseWidth, nEllipseHeight); 
			fDrawRectangle = FALSE; 
			rcTarget.left = rcTarget.right = 0; 
			rcTarget.top = rcTarget.bottom = 0; 
		} 

		EndPaint(hwnd, &ps); 
		break; 

	case WM_SIZE: // ��С�ı�
		GetClientRect(hwnd, &rcClient); 
		ptClientUL.x = rcClient.left; 
		ptClientUL.y = rcClient.top; 
		ptClientLR.x = rcClient.right; 
		ptClientLR.y = rcClient.bottom; 
		ClientToScreen(hwnd, &ptClientUL); 
		ClientToScreen(hwnd, &ptClientLR);
		// ����RECT�Ĵ�С��λ��
		SetRect(&rcClient, ptClientUL.x, ptClientUL.y, 
			ptClientLR.x, ptClientLR.y); 
		return 0; 

	case WM_LBUTTONDOWN: 

		// ʹ����궨��һ�����η�Χ
		ClipCursor(&rcClient); 

		// ����λ��
		pt.x = (LONG) LOWORD(lParam); 
		pt.y = (LONG) HIWORD(lParam); 

		if (fDrawEllipse) 
			fSizeEllipse = TRUE; 

		return 0; 

	case WM_MOUSEMOVE: 

		if ((wParam && MK_LBUTTON) 
			&& (fSizeEllipse || fSizeRectangle 
			|| fSizeRoundRect)) 
		{  

			hdc = GetDC(hwnd); 
			// Ŀ��RECT�Ƿ�գ�
			if (!IsRectEmpty(&rcTarget)) 
			{
				Rectangle(hdc, rcTarget.left, rcTarget.top, 
					rcTarget.right, rcTarget.bottom); 
			}

			// ��������ƶ���������õ���Ҫ���Ƶķ�Χ 
			// ��������ƶ����յ���������λ�ã������յ�������ľ��εĽǲ�ͬ
			if ((pt.x < (LONG) LOWORD(lParam)) && 
				(pt.y > (LONG) HIWORD(lParam))) 
			{
				SetRect(&rcTarget, pt.x, HIWORD(lParam), 
					LOWORD(lParam), pt.y); 
			} 

			else if ((pt.x > (LONG) LOWORD(lParam)) && 
				(pt.y > (LONG) HIWORD(lParam))) 
			{
				SetRect(&rcTarget, LOWORD(lParam), 
					HIWORD(lParam), pt.x, pt.y); 
			}

			else if ((pt.x > (LONG) LOWORD(lParam)) && 
				(pt.y < (LONG) HIWORD(lParam))) 
			{
				SetRect(&rcTarget, LOWORD(lParam), pt.y, 
					pt.x, HIWORD(lParam)); 
			}
			else 
			{
				SetRect(&rcTarget, pt.x, pt.y, LOWORD(lParam), 
					HIWORD(lParam)); 
			}
			// ������ƶ��Ĺ����У�����һ������
			Rectangle(hdc, rcTarget.left, rcTarget.top, 
				rcTarget.right, rcTarget.bottom); 
			DeleteDC( hdc); 
		} 
		return 0; 

	case WM_LBUTTONUP: 

		// ���� 
		if (fSizeEllipse) 
		{ 
			fSizeEllipse = FALSE; 
			fDrawEllipse = TRUE; 
		} 
		if (fSizeRectangle) 
		{ 
			fSizeRectangle = FALSE; 
			fDrawRectangle = TRUE; 
		} 
		if (fSizeRoundRect) 
		{ 
			fSizeRoundRect = FALSE; 
			fDrawRoundRect = TRUE; 
		} 
		// �Ƿ��л��Ƶ�ͼ��
		if (fDrawEllipse || fDrawRectangle || fDrawRoundRect) 
		{ 
			InvalidateRect(hwnd, &rcTarget, TRUE); 
			UpdateWindow(hwnd); // WM_PAINT
		} 

		// �ͷ���� 
		ClipCursor((LPRECT) NULL); 
		return 0; 

	case WM_DESTROY: 
		DeleteDC(hdcCompat); 
		PostQuitMessage(0); 
		break; 

	default: 
		return DefWindowProc(hwnd, uMsg, wParam, lParam); 
	} 
	return (LRESULT) NULL; 
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
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_SHAPE);
	wc.lpszClassName = "shape"; 

	if (!RegisterClass(&wc))
		return (FALSE);

	hWnd = CreateWindow(
		"shape", 
		"shape", 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
		NULL, 
		NULL,
		hInstance, 
		NULL 
		);

	if (!hWnd)
		return (FALSE);

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