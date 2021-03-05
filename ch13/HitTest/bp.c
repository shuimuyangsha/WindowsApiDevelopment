/* ************************************
*����ͨWindows API��
* ʾ������
* bp.c
* 13.8.4 ��������
**************************************/
/* ͷ�ļ� */
#include <Windows.h>
/* ȫ�ֱ��� */
RECT rctGrid; // ����
RECT rctBrush; // ��ʾ��ʽ��ˢ�ĸ���
RECT rctWindow;
UINT bBrushBits[8]; // ���ڴ�����ʽ��ˢ
RECT rect[64]; // ��ʮ�ĸ�С����
HBITMAP hbm; // λͼ��ˢ
HBRUSH hbrush; // ��ǰ��ˢ
HBRUSH hBrushGrid[8]; // �������С����ʽ�Ļ�ˢ
HBRUSH hbrushOld; // Ĭ�ϻ�ˢ
HRGN hrgnCell; // ����
HDC hdc; // DC���
POINTS ptlHit; // �������
int i;

/*************************************
* VOID InitGridBurshAndRect()
* ���� ��ʼ��������Ҫʹ�õ��Ļ�ˢ
* ��ʼ�����ɸ�RECT
**************************************/
VOID InitGridBurshAndRect()
{

	int x, y, deltaX, deltaY; // ���ڻ�С����

	// ���������ֲ�ͬ�Ļ�ˢ
	hBrushGrid[0] = CreateHatchBrush(HS_BDIAGONAL,RGB(255,0,0));
	hBrushGrid[1] = CreateHatchBrush(HS_CROSS,RGB(0,255,0));
	hBrushGrid[2] = CreateHatchBrush(HS_DIAGCROSS,RGB(0,0,255));
	hBrushGrid[3] = CreateHatchBrush(HS_FDIAGONAL,RGB(255,0,255));
	hBrushGrid[4] = CreateHatchBrush(HS_HORIZONTAL,RGB(255,255,0));
	hBrushGrid[5] = CreateHatchBrush(HS_BDIAGONAL,RGB(0,255,255));
	hBrushGrid[6] = CreateHatchBrush(HS_VERTICAL,RGB(0,0,0));
	hBrushGrid[7] = CreateSolidBrush(RGB(200,200,200));

	// ����ӱ߽�����꣬����ᱻ����Ϊ8*8��С�����ӣ����ڵ������
	rctGrid.top = rctGrid.left = 10;
	rctGrid.bottom = rctGrid.right = 330;// ����
	rctBrush.top = 10; rctBrush.left = 340;
	rctBrush.bottom = 330;rctBrush.right = 660;// ����

	// ����С��ʽ�е����꣬������������
	deltaX = (rctGrid.right - rctGrid.left)/8;
	deltaY = (rctGrid.bottom - rctGrid.top)/8;

	for (y=rctGrid.top, i=0; y < rctGrid.bottom; y += deltaY)
	{
		for(x=rctGrid.left; x < (rctGrid.right - 8) && i < 64;
			x += deltaX, i++)
		{
			rect[i].left = x; rect[i].top = y;
			rect[i].right = x + deltaX;
			rect[i].bottom = y + deltaY;
		}
	}
}

/*************************************
* VOID DrawGridLine(HWND hwnd)
* ���� ����InitGridBurshAndRect����ʼ����С����ı߽�
* ����С����
**************************************/
VOID DrawGridLine(HWND hwnd)
{
	hdc = GetDC(hwnd);
	// ���ߺ�γ��
	for (i=rctGrid.left; i<=rctGrid.right;
		i+=(rctGrid.right - rctGrid.left)/8)
	{
		MoveToEx(hdc, i, rctGrid.top, NULL);
		LineTo(hdc, i, rctGrid.bottom);
	}
	for (i=rctGrid.top; i<=rctGrid.bottom;
		i+=(rctGrid.bottom - rctGrid.top)/8)
	{
		MoveToEx(hdc, rctGrid.left, i, NULL);
		LineTo(hdc, rctGrid.right, i);
	}
	ReleaseDC(hwnd, hdc);
	DeleteDC( hdc );
}

/*************************************
*VOID PaintGrid(HWND hwnd,POINTS ptlHit)
* ���� ʹ�û�ˢ���ptlHit�����ڵ�С����
**************************************/
VOID PaintGrid(HWND hwnd,POINTS ptlHit)
{
	// �������򣬴󷽸�
	hrgnCell = CreateRectRgn(rctGrid.left, rctGrid.top,
		rctGrid.right, rctGrid.bottom);
	// ��ô��ڵ�DC
	hdc = GetDC(hwnd);
	// ������ѡ����DC
	SelectObject(hdc, hrgnCell);
	// ���Ե��Ƿ���������
	if (PtInRegion(hrgnCell, ptlHit.x, ptlHit.y))
	{
		// ������ڴ󷽸��У�ѭ�����Ե����ĸ�С������
		for (i=0; i<8; i++)
			bBrushBits[i] = 0xFF;
		for(i=0; i<64; i++)
		{
			DeleteObject(hrgnCell);
			// ÿһ��С���񴴽�һ������
			hrgnCell = CreateRectRgn(rect[i].left,
				rect[i].top,
				rect[i].right, rect[i].bottom);
			// �����Ƿ���С������
			if (PtInRegion(hrgnCell, ptlHit.x, ptlHit.y))
			{
				// ����ǵĻ�����䣬��һ��ʹ�õ���仭ˢ��һ���ġ�
				FillRgn(hdc,hrgnCell,hBrushGrid[i/8]);
			}
		}
		// �ͷ�
		SelectObject(hdc, hbrushOld);
		DeleteObject(hbrush);
		DeleteObject(hbm);
		DeleteDC( hdc);
	}
}

/*************************************
*VOID PaintPattern(HWND hwnd, POINTS ptlHit)
* ���� ����ʾ��ʽ��ˢ�ķ�����ʹ�ø�ʽ��ˢ���л���
**************************************/
VOID PaintPattern(HWND hwnd, POINTS ptlHit)
{
	// ��ȡDC
	hdc = GetDC(hwnd);
	// ���ݵ���Ĳ�ͬλ�ô�����ͬ��λͼ
	i = ptlHit.x % 8;
	bBrushBits[i] += 0x50;
	// ����λͼ
	hbm = CreateBitmap(8, 8, 1, 1, (LPBYTE)bBrushBits);
	// ������ʽ��ˢ
	hbrush = CreatePatternBrush(hbm);
	// ΪDCѡ��ˢ
	hbrushOld = SelectObject(hdc, hbrush);
	// ���ƾ���
	Rectangle(hdc, rctBrush.left, rctBrush.top,
		rctBrush.right, rctBrush.bottom);
	// �ͷ�DC
	ReleaseDC(hwnd,hdc);
	DeleteDC( hdc );
}

/*************************************
* MainWndProc
* ���� ������Ϣ��������
**************************************/
LRESULT CALLBACK MainWndProc(
							 HWND hwnd,
							 UINT uMsg,
							 WPARAM wParam,
							 LPARAM lParam)
{

	switch (uMsg)
	{
		// �ͷ���Դ���˳�
	case WM_DESTROY:
		for(i=0; i<8; i++)
			DeleteObject(hBrushGrid[i]);
		ExitProcess(0);
		// ��ʼ����ˢ������
	case WM_CREATE:
		InitGridBurshAndRect();
		break;
		// ����
	case WM_PAINT:
		DrawGridLine(hwnd);
		break;
		// ������
	case WM_LBUTTONDOWN:
		ptlHit = MAKEPOINTS((LPPOINTS)lParam);
		// ���һ��С����
		PaintGrid(hwnd, ptlHit);
		// ����Ҳ�ĸ�ʽ��ˢ����
		PaintPattern(hwnd, ptlHit);
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// WinMain
int WINAPI WinMain(HINSTANCE hinstance,
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

	// ע�ᴰ���࣬���������ڣ�������ʾ��ȡ��λͼ
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = MainWndProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hinstance;
	wcx.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = "MainWClass";
	wcx.hIconSm = NULL;

	if( !RegisterClassEx(&wcx))
		return 1;
	// ��������
	hwnd = CreateWindow(
		"MainWClass",
		"Brush_Pen",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
		WS_MAXIMIZE | WS_POPUPWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800, 600,
		(HWND) NULL, (HMENU) NULL, hinstance, (LPVOID) NULL);

	if (!hwnd)
		return 1;
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