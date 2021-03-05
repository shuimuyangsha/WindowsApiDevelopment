/* ************************************
*����ͨWindows API��
* ʾ������
* PrtSrcn.c
* 13.7 ����
**************************************/
/* ͷ�ļ� */
#include <windows.h>
/* �������� */
#define PALVERSION 0x300
#define CAP_SHOW_MODE_STRTCH 1
#define CAP_SHOW_MODE_NOSTRTCH 0

/* ȫ�ֱ��� */
HBITMAP ghBitmap = NULL;
RECT rectShow;
// �޸������ȡ��ͬ�Ĵ��ڣ����ΪNULL�����ȡ��Ļ
LPSTR szCaptureWindowName = "Windows ���������";

/* �������� */
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
HBITMAP ScreenCapture(LPSTR filename ,WORD BitCount,LPRECT lpRect);
VOID DoPaint(HWND hWnd, DWORD dwMode);

/*************************************
* DWORD WINAPI WinMain(HINSTANCE hinstance,
HINSTANCE hPrevInstance,
LPSTR lpCmdLine,
int nCmdShow)
* ���� ����������Ϊ�ļ�������ʾ�ڴ�����
*
**************************************/
INT WINAPI WinMain(HINSTANCE hinstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;
	MSG msg;
	WORD wport = 80;
	BOOL fGotMessage;
	HWND hwndCap = NULL;

	// ��ȡȫ��Ļ���Ǵ���
	if(szCaptureWindowName != NULL)
	{
		hwndCap = FindWindow(NULL,"Windows ���������");
		// ��ȡ���ڵ�RECT���������޸ģ���ȡ��Ļ�е���������
		if(!GetWindowRect(hwndCap,&rectShow))
		{
			MessageBox(NULL,"Can not find window to capture", "erroe",MB_OK);
			return 0;
		}
	}
	// ע�ᴰ���࣬���������ڣ�������ʾ��ȡ��λͼ
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = MainWndProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hinstance;
	wcx.hIcon = LoadIcon(NULL,	MAKEINTRESOURCE(IDI_APPLICATION));
	wcx.hCursor = LoadCursor(NULL,	IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = "MainWClass";
	wcx.hIconSm = NULL;

	if( !RegisterClassEx(&wcx))
		return 1;
	// ��������
	hwnd = CreateWindow(
		"MainWClass",
		"CAP",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
		WS_MAXIMIZE | WS_POPUPWINDOW,
		CW_USEDEFAULT,	CW_USEDEFAULT,	CW_USEDEFAULT,	CW_USEDEFAULT,
		(HWND) NULL,	(HMENU) NULL,	hinstance, (LPVOID) NULL);

	if (!hwnd)
		return 1;

	// ��ȡ��Ļ���ɸ�����Ҫ���ò�ͬ�Ĳ���������ֻ��ʾ��ȡ�ض����ڡ�
	ghBitmap = ScreenCapture("taskmgr.bmp" ,32, &rectShow);
	// ��ʾ
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while ((fGotMessage = GetMessage(&msg, (HWND) NULL, 0, 0)) != 0 && fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
	UNREFERENCED_PARAMETER(lpCmdLine);
}

LRESULT CALLBACK MainWndProc(
							 HWND hwnd,
							 UINT uMsg,
							 WPARAM wParam,
							 LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
		// ��ʾ��ȡ����Ļ
		DoPaint(hwnd,CAP_SHOW_MODE_STRTCH);
		break;
	case WM_DESTROY:
		// ������BITMAP������Ҫɾ�������ͷ���Դ
		DeleteObject(ghBitmap);
		ExitProcess(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*************************************
* VOID DoPaint(HWND hWnd, DWORD dwMode)
* ���� ��λͼ��ȫ�ֱ���ghBitmap����ʾ�ڽ�����
*
* ���� HWND hWnd��������ʾλͼ�Ĵ���
* DWORD dwMode��ģʽ���Ƿ�����
*
* �޷���ֵ
**************************************/
VOID DoPaint(HWND hWnd, DWORD dwMode)
{
	PAINTSTRUCT ps;
	RECT rect;
	HDC hdcMem;
	BITMAP bm;
	// BeginPaint
	HDC hDC = BeginPaint(hWnd, &ps);
	// ��ȡ���ڵ�Client����������ʾλͼ
	GetClientRect(hWnd, &rect);

	// ��������ģʽ
	SetStretchBltMode(hDC, HALFTONE);
	// ��BITMAP����ѡ�����ڴ�DC
	hdcMem = CreateCompatibleDC(hDC);
	SelectObject(hdcMem, ghBitmap);
	if (ghBitmap)
	{
		// ��ȡDIB����
		if (GetObject(ghBitmap, sizeof(BITMAP), &bm))
		{
			// �жϲ���ʾ���Ƿ������ʾ���ڴ�С����λͼ
			// ���ò�ͬ�ķ��潫�ڴ�DC StretchBl t������Client����DC
			if(dwMode == CAP_SHOW_MODE_STRTCH)
			{
				StretchBlt(hDC, 0, 0, rect.right, rect.bottom,
					hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			}
			else
			{
				// �����죬������ʾ��λ�ã�������ʾ��Client������
				INT ixStart = (rect.right - rect.left - bm.bmWidth)/2;
				INT iyStart = (rect.bottom - rect.top - bm.bmHeight)/2;
				ixStart = ixStart < 0 ? 0 : ixStart;
				iyStart = iyStart < 0 ? 0 : iyStart;
				BitBlt(hDC, 0, 0, rect.right, rect.bottom,
					hdcMem,-ixStart,-iyStart, SRCCOPY);
			}
			DeleteDC(hdcMem);
		}
	}
	// ���û��λͼ����ʹ��Brush���
	else
	{
		PatBlt(hDC, 0, 0, rect.right, rect.bottom, BLACKNESS);
	}
	// EndPaint
	EndPaint(hWnd, &ps);
}

/*************************************
* BITMAP ScreenCapture(LPSTR filename ,WORD BitCount,LPRECT lpRect);
* ���� ��ȡָ���������Ļ��������Ϊ�ļ�
*
* ����	LPSTR filename ����λͼ�ļ����ļ�·�������ΪNULL���򲻱���
*		WORD BitCount Bit��ȣ����ڱ�ʾһ�����ص���ʹ�õ����ݳ���
*		LPRECT lpRect �����ȡ����Ļ�������ΪNULL�����ȡȫ��Ļ
*
* ���� HBITMAP ����ȡ��λͼ����
**************************************/
HBITMAP ScreenCapture(LPSTR filename ,WORD BitCount,LPRECT lpRect)
{
	HBITMAP hBitmap;
	// ��ʾ����ĻDC
	HDC hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	HDC hmemDC = CreateCompatibleDC(hScreenDC);
	// ��ʾ����Ļ�Ŀ�͸�
	int ScreenWidth = GetDeviceCaps(hScreenDC, HORZRES);
	int ScreenHeight = GetDeviceCaps(hScreenDC, VERTRES);
	// �ɵ�BITMAP�������������ȡ��λ�ý���
	HBITMAP hOldBM;
	// ����λͼ����
	PVOID lpvpxldata;
	// ������ȡ�ĳ������
	INT ixStart;
	INT iyStart;
	INT iX;
	INT iY;
	// λͼ���ݴ�С
	DWORD dwBitmapArraySize;
	// ������С
	DWORD nBitsOffset;
	DWORD lImageSize ;
	DWORD lFileSize ;
	// λͼ��Ϣͷ
	BITMAPINFO bmInfo;
	// λͼ�ļ�ͷ
	BITMAPFILEHEADER bmFileHeader;
	// д�ļ���
	HANDLE hbmfile;
	DWORD dwWritten;

	// ���LPRECT ΪNULL ��ȡ������Ļ
	if(lpRect == NULL)
	{
		ixStart = iyStart = 0;
		iX = ScreenWidth;
		iY =ScreenHeight;
	}
	else
	{
		ixStart = lpRect->left;
		iyStart = lpRect->top;
		iX = lpRect->right - lpRect->left;
		iY = lpRect->bottom - lpRect->top;
	}
	// ����BTIMAP
	hBitmap = CreateCompatibleBitmap(hScreenDC, iX, iY);
	// ��BITMAPѡ�����ڴ�DC��
	hOldBM = (HBITMAP)SelectObject(hmemDC, hBitmap);
	// BitBlt��ĻDC���ڴ�DC�����������ȡ�Ļ�ȡ���ò���
	BitBlt(hmemDC, 0, 0, iX, iY, hScreenDC, ixStart, iyStart, SRCCOPY);
	// ���ɵ�BITMAP����ѡ����ڴ�DC������ֵΪ���滻�Ķ��󣬼�����ȡ��λͼ
	hBitmap = (HBITMAP)SelectObject(hmemDC, hOldBM);
	if(filename == NULL)
	{
		DeleteDC( hScreenDC);
		DeleteDC(hmemDC);
		return hBitmap;
	}
	// Ϊλͼ���������ڴ�ռ�
	dwBitmapArraySize = ((((iX*32) + 31) & ~31)>> 3)* iY;
	lpvpxldata = HeapAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,dwBitmapArraySize);
	ZeroMemory(lpvpxldata,dwBitmapArraySize);

	// ��� BITMAPINFO �ṹ
	ZeroMemory(&bmInfo,sizeof(BITMAPINFO));
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = iX;
	bmInfo.bmiHeader.biHeight = iY;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = BitCount;
	bmInfo.bmiHeader.biCompression = BI_RGB;

	// ��� BITMAPFILEHEADER �ṹ
	ZeroMemory(&bmFileHeader,sizeof(BITMAPFILEHEADER));
	nBitsOffset = sizeof(BITMAPFILEHEADER) + bmInfo.bmiHeader.biSize;
	lImageSize =
		((((bmInfo.bmiHeader.biWidth * bmInfo.bmiHeader.biBitCount) + 31) & ~31)>> 3)
		* bmInfo.bmiHeader.biHeight;
	lFileSize = nBitsOffset + lImageSize;
	bmFileHeader.bfType = 'B'+('M'<<8);
	bmFileHeader.bfSize = lFileSize;
	bmFileHeader.bfOffBits = nBitsOffset;

	// ��ȡDIB����д�뵽�ļ�
	GetDIBits(hmemDC, hBitmap, 0, bmInfo.bmiHeader.biHeight,
		lpvpxldata, &bmInfo, DIB_RGB_COLORS);
	// д�ļ�
	hbmfile = CreateFile(filename,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(hbmfile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,"create file error","error",MB_OK);
	}

	WriteFile(hbmfile,&bmFileHeader,sizeof(BITMAPFILEHEADER),&dwWritten,NULL);
	WriteFile(hbmfile,&bmInfo,sizeof(BITMAPINFO),&dwWritten,NULL);
	WriteFile(hbmfile,lpvpxldata,lImageSize,&dwWritten,NULL);
	CloseHandle(hbmfile);

	// �ͷ��ڴ棬�����ͬ��DC��
	// ����û��ɾ��BITMAP����������ʾ��ɺ�ɾ��
	HeapFree(GetProcessHeap(),HEAP_NO_SERIALIZE,lpvpxldata);
	ReleaseDC(0, hScreenDC);
	DeleteDC(hmemDC);	
	return hBitmap;
}