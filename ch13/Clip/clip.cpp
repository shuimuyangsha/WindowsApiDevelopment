/* ************************************
*����ͨWindows API��
* ʾ������
* clip.cpp
* 13.8 ����·�����޼�
**************************************/
/* ͷ�ļ� */
#include <Windows.h>
#include <math.h>
#include "resource.h"
/* ȫ�ֱ��� */
CHOOSEFONT cf; // ѡ������
LOGFONT lf; // �߼�����
HFONT hfont; // �������
HFONT hfontOld; // DC��ԭ������
HDC hdc; // DC���
int nXStart, nYStart; // ���ڻ�ֱ��
RECT rc; // ���ڿͻ���RECT
double aflSin[90]; // ���ڻ��Ʒ���ֱ�ߴ�
double aflCos[90]; // ���ڻ��Ʒ���״ֱ�ߴ�
double flRadius,a; // �뾶
int iMode; // �޼�·�����ģʽ
HRGN hrgn; // �޼�����
int i;
COLORREF crOld;

// ������Ϣ������
LRESULT APIENTRY MainWndProc(
							 HWND hwnd,
							 UINT message,
							 WPARAM wParam,
							 LPARAM lParam)

{

	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;

	case WM_COMMAND: // �˵�����
		switch (wParam)
		{
		case IDM_VANISH: // �����ͻ��������������޼�ȫ�������ΧҲ�ᱻ���ƣ�
			hdc = GetDC(hwnd);
			GetClientRect(hwnd, &rc);
			FillRect(hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
			DeleteDC( hdc);
			break;
			// �����û��Ĳ˵����룬�����޼�·�������ģʽ
		case IDM_AND:
			iMode = RGN_AND;
			break;
		case IDM_COPY:
			iMode = RGN_COPY;
			break;
		case IDM_DIFF:
			iMode = RGN_DIFF;
			break;
		case IDM_OR:
			iMode = RGN_OR;
			break;
		case IDM_XOR:
			iMode = RGN_XOR;
			break;

		case IDM_CLIP:
			{
				// ��ȡ���ڿͻ�����DC
				hdc = GetDC(hwnd);
				// ������ΪDCѡ������
				// ��Ϣ�ڲ˵��е������ǰ����ѡ������
				hfont = CreateFontIndirect(cf.lpLogFont);
				hfontOld = (HFONT)SelectObject(hdc, hfont);

				// ����㣬���ڴ������������
				POINT point[5]= {{0,200},{600,200},{100,600},{300,0},{500,600}};
				// ������������������
				hrgn = CreatePolygonRgn(point, 5, WINDING);
				// ������ѡ��Ϊ�޼�����
				SelectClipRgn(hdc, hrgn);
				// ���������
				LPSTR szOut = "Lorem ipsum dolor sit amet, consectetur \n"
					"adipisicing elit, sed do eiusmod tempor \n"
					"incididunt ut labore et dolore magna \n"
					"aliqua. Ut enim ad minim veniam, quis \n"
					"nostrud exercitation ullamco laboris nisi \n"
					"ut aliquip ex ea commodo consequat. Duis \n"
					"aute irure dolor in reprehenderit in \n"
					"voluptate velit esse cillum dolore eu \n"
					"fugiat nulla pariatur. Excepteur sint \n"
					"occaecat cupidatat non proident, sunt \n"
					"in culpa qui officia deserunt mollit \n"
					"anim id est laborum.\n";
				// ���ڿͻ����������������
				RECT rect;
				GetClientRect(hwnd,&rect);
				// �������ֱ���Ϊ͸��
				SetBkMode(hdc, TRANSPARENT);
				// ��ʼ����·��
				BeginPath(hdc);
				// ·������״Ϊ���������
				DrawText(hdc, szOut, lstrlen(szOut),&rect , DT_CENTER);
				EndPath(hdc);// ·���Ѿ�ΪDC�ĵ�ǰ·��
				// ΪDCѡ���޼�·����ʹ���û�ͨ���˵������ģʽ
				// ע���ڽ��д˲���ǰ��ͨ���˵�ѡ�����ģʽ
				SelectClipPath(hdc, iMode);

				// �������״ֱ�ߣ������Ͻ�Ϊԭ��
				// �����ߵ��յ�
				for (i = 0; i < 90; i++)
				{
					aflSin[i] = sin( (((double)i) / 180.0)
						* 3.14159);
				}
				for (i = 0; i < 90; i++)
				{
					aflCos[i] = cos( (((double)i) / 180.0)
						* 3.14159);
				}
				flRadius = 1000;// �ߵĳ��ȣ����ڴ�СΪ600*650��
				// ���ߣ���һ�Ȼ�һ����
				for (i = 0; i < 90; i++)
				{
					MoveToEx(hdc, nXStart, nYStart,
						(LPPOINT) NULL);
					LineTo(hdc, nXStart + ((int) (flRadius
						* aflCos[i])),
						nYStart + ((int) (flRadius
						* aflSin[i])));
				}
				// ѡ������壬�ͷ�
				SelectObject(hdc, hfontOld);
				DeleteObject(hfont);
				DeleteDC( hdc);
				// ˢ�´���
				UpdateWindow(hwnd);

				break;
			}

		case IDM_FONT:
			// �û�ѡ������
			cf.lStructSize = sizeof (CHOOSEFONT);
			cf.hwndOwner = hwnd;
			cf.lpLogFont = &lf;
			cf.Flags = CF_SCREENFONTS | CF_EFFECTS;
			cf.rgbColors = RGB(0, 255, 255);
			cf.nFontType = SCREEN_FONTTYPE;

			ChooseFont(&cf);
			break;

		default:
			return DefWindowProc(hwnd, message, wParam,
				lParam);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

// WinMain
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
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_CLIP);//�˵�
	wc.lpszClassName = "clip";

	if (!RegisterClass(&wc))
	{
		return (FALSE);
	}

	hWnd = CreateWindow(
		"clip",
		"clip",
		WS_OVERLAPPEDWINDOW,
		100, 100, 600, 650,
		NULL,
		NULL,
		hInstance,
		NULL
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