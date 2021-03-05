/* ************************************
*����ͨWindows API��
* ʾ������
* curves.c
* 13.4.6 ����
**************************************/
/* ͷ�ļ� */
#include <windows.h>
/* Ԥ���� */
#define MAXGUIDESEGMENTS 1000
#define MyAlloc(dwSize) HeapAlloc(GetProcessHeap(),0,dwSize)
#define MyFree(lpMem) HeapFree(GetProcessHeap(),0,lpMem);
/* �������� */
BOOL GetGuideLine(HWND, LPPOINT*, LPDWORD);
BOOL ShowGuide(HDC, LPPOINT, DWORD);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD GetLastErrorBox(HWND hWnd, LPSTR lpTitle);
/* ȫ�ֱ���*/
HINSTANCE hInst;
LPSTR szAppName = "Curves";
LPSTR szTitle = "Curves Application";

/*************************************
* WinMain
* ����	��������
**************************************/
int WINAPI WinMain(
				   HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	MSG msg;
	HWND hWnd;
	WNDCLASS wc;

	hInst = hInstance;
	// ע�ᴰ����
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
		GetLastErrorBox(NULL, "Error in RegisterClass");
		return (FALSE);
	}
	// ��������
	hWnd = CreateWindow(
		szAppName, 
		szTitle, 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
		NULL, 
		NULL,
		hInstance, 
		NULL 
		);

	if (!hWnd)
	{
		GetLastErrorBox(hWnd, "Error in CreateWindow");
		return (FALSE);
	}
	// ��ʾ����
	ShowWindow(hWnd, nCmdShow); 
	UpdateWindow(hWnd); 
	// ��Ϣѭ��
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)(msg.wParam);

	UNREFERENCED_PARAMETER(lpCmdLine); 
}

/*************************************
* WndProc
* ����	������Ϣ������
**************************************/
LRESULT CALLBACK WndProc(
						 HWND hWnd, 
						 UINT message, 
						 WPARAM uParam, 
						 LPARAM lParam)
{
	static BOOL bOn = TRUE; 
	static LPPOINT lpBlue = NULL;
	static LPPOINT lpRed = NULL;
	static DWORD dwBlue = 0;
	static DWORD dwRed = 0;
	static BOOL bOutlineOnly = FALSE;
	static BOOL bShowGuides = TRUE;
	static HPEN hpenBlue, hpenRed;

	switch (message)
	{
	case WM_CREATE:
		{
			//  ��ȡDC
			HDC hDC = GetDC(hWnd);
			// �����ʶ���
			hpenBlue = CreatePen(PS_SOLID, 1, RGB(0,0,255));
			hpenRed = CreatePen(PS_SOLID, 1, RGB(255,0,0));
		}
		GetLastErrorBox(hWnd, "Error in WM_CREATE");
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);
			RECT rect;
			// ���ͻ�������Ϊ��ɫ
			GetClientRect(hWnd, &rect);
			PatBlt(hDC, 0, 0, rect.right, rect.bottom, WHITENESS);
			// �����ں��ߣ��ִ�������
			if (dwBlue && dwRed)
			{
				// ��ʾ����
				if (dwBlue && bShowGuides)
				{
					SelectObject(hDC, hpenBlue);
					ShowGuide(hDC, lpBlue, dwBlue);
					SelectObject(hDC, GetStockObject(BLACK_PEN));
				}
				// ��ʾ����
				if (dwRed && bShowGuides)
				{
					SelectObject(hDC, hpenRed);
					ShowGuide(hDC, lpRed, dwRed);
					SelectObject(hDC, GetStockObject(BLACK_PEN));
				}
			}
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			HDC hDC = GetDC(hWnd);
			RECT rect;

			if (bOn)// ������������
			{
				// ���ͻ������Ϊ��ɫ				
				GetClientRect(hWnd, &rect);
				PatBlt(hDC, 0, 0, rect.right, rect.bottom, WHITENESS);

				// �ͷ���Դ
				if (lpBlue)
					MyFree(lpBlue);
				if (lpRed)
					MyFree(lpRed);
				dwRed = 0;
				dwBlue = 0;

				// ��ʼ��������ƶ�����������
				SelectObject(hDC, hpenBlue); 
				GetGuideLine(hWnd, &lpBlue, &dwBlue);
			}
			else//������
			{
				// ��ʼ��������ƶ������ƺ���
				SelectObject(hDC, hpenRed);
				GetGuideLine(hWnd, &lpRed, &dwRed);
			}
			// �������ſ����ָ��ʶ���
			SelectObject(hDC, GetStockObject(BLACK_PEN));
			// ȡ�����ں�ɫ����ɫ�佻��
			bOn = !bOn;
		}
		GetLastErrorBox(hWnd, "Error in WM_LBUTTONDOWN");
		break;

	case WM_DESTROY: 
		// �ͷ���Դ���˳�
		if (lpBlue) MyFree(lpBlue);
		if (lpRed) MyFree(lpRed);
		PostQuitMessage(0);
		break;

	default: 
		return (DefWindowProc(hWnd, message, uParam, lParam));
	}
	return (0);
}
/*************************************
* BOOL GetGuideLine(HWND hWnd, LPPOINT *lpPoint, LPDWORD lpdwNumPts)
* ����	������꣬�������켣
* ����	hWnd������
*			lpPoint�����ڱ��������飬����ú�������
*			lpdwNumPts�����ص�����Ĵ�С
**************************************/
BOOL GetGuideLine(HWND hWnd, LPPOINT *lpPoint, LPDWORD lpdwNumPts)
{
	MSG msg;
	HDC hDC = GetDC(hWnd);
	BOOL bFirstTime = TRUE;
	DWORD dwPos = 0;
	RECT rect;

	SetCapture(hWnd);	// ������겶����
	GetClientRect(hWnd, &rect);
	// Ϊ���������ռ�
	*lpPoint = (LPPOINT)MyAlloc(MAXGUIDESEGMENTS * sizeof(POINT));

	for (;;)
	{
		// �������������Ϣ
		WaitMessage();
		if (PeekMessage(&msg,NULL,WM_MOUSEFIRST,WM_MOUSELAST,PM_REMOVE))
		{
			// �ж��Ƿ��ڿͻ�����
			if ((LOWORD(msg.lParam) < rect.right) && (HIWORD(msg.lParam) < rect.bottom))
			{
				// �Ƿ��һ���յ���Ϣ
				if (bFirstTime)
				{
					bFirstTime = FALSE;
					// ����ǵ�һ�ν��ʵ�����ƶ����������λ��
					MoveToEx(hDC, LOWORD(msg.lParam), HIWORD(msg.lParam), NULL);
				}
				// �Ƿ�ﵽ��������
				if (dwPos < MAXGUIDESEGMENTS)
				{
					// �����ƶ�����������Ϣ��ÿ�յ�һ����Ϣ����һ���㡣
					(*lpPoint)[dwPos].x = LOWORD(msg.lParam);
					(*lpPoint)[dwPos].y = HIWORD(msg.lParam);
					// ���Ƶ�������ڵĵ�
					LineTo(hDC, (*lpPoint)[dwPos].x, (*lpPoint)[dwPos].y);
					dwPos++;
				}
			}
			if (msg.message == WM_LBUTTONUP)
				break;
		}
		else
			continue;
	}

	*lpdwNumPts = dwPos;
	ReleaseDC(hWnd, hDC);
	ReleaseCapture();
	DeleteDC( hDC );

	return TRUE;
}

/*************************************
* BOOL ShowGuide(HDC hDC, LPPOINT lpPoints, DWORD dwNumPts)
* ����	���ݱ���ĵ�������ػ�����
* ����	hWnd������
*			lpPoint������ĵ�����飬
*			dwNumPts������Ĵ�С
**************************************/
BOOL ShowGuide(HDC hDC, LPPOINT lpPoints, DWORD dwNumPts)
{
	Polyline(hDC, lpPoints, dwNumPts);
	return TRUE;
}

// ��ʾ������Ϣ
DWORD GetLastErrorBox(HWND hWnd, LPSTR lpTitle)
{
	LPVOID lpv;
	DWORD dwRv;

	if (GetLastError() == 0) return 0;

	dwRv = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(LPVOID)&lpv,
		0,
		NULL);

	MessageBox(hWnd, lpv, lpTitle, MB_OK);

	if(dwRv)
		LocalFree(lpv);

	SetLastError(0);
	return dwRv;
}