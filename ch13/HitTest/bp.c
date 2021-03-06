/* ************************************
*《精通Windows API》
* 示例代码
* bp.c
* 13.8.4 遍历字体
**************************************/
/* 头文件 */
#include <Windows.h>
/* 全局变量 */
RECT rctGrid; // 格子
RECT rctBrush; // 演示格式画刷的格子
RECT rctWindow;
UINT bBrushBits[8]; // 用于创建格式画刷
RECT rect[64]; // 六十四个小格子
HBITMAP hbm; // 位图画刷
HBRUSH hbrush; // 当前画刷
HBRUSH hBrushGrid[8]; // 用于填充小方格式的画刷
HBRUSH hbrushOld; // 默认画刷
HRGN hrgnCell; // 区域
HDC hdc; // DC句柄
POINTS ptlHit; // 鼠标坐标
int i;

/*************************************
* VOID InitGridBurshAndRect()
* 功能 初始化程序需要使用到的画刷
* 初始化若干个RECT
**************************************/
VOID InitGridBurshAndRect()
{

	int x, y, deltaX, deltaY; // 用于画小格子

	// 创建了七种不同的画刷
	hBrushGrid[0] = CreateHatchBrush(HS_BDIAGONAL,RGB(255,0,0));
	hBrushGrid[1] = CreateHatchBrush(HS_CROSS,RGB(0,255,0));
	hBrushGrid[2] = CreateHatchBrush(HS_DIAGCROSS,RGB(0,0,255));
	hBrushGrid[3] = CreateHatchBrush(HS_FDIAGONAL,RGB(255,0,255));
	hBrushGrid[4] = CreateHatchBrush(HS_HORIZONTAL,RGB(255,255,0));
	hBrushGrid[5] = CreateHatchBrush(HS_BDIAGONAL,RGB(0,255,255));
	hBrushGrid[6] = CreateHatchBrush(HS_VERTICAL,RGB(0,0,0));
	hBrushGrid[7] = CreateSolidBrush(RGB(200,200,200));

	// 大格子边界的坐标，后面会被划分为8*8个小方格子，用于点击测试
	rctGrid.top = rctGrid.left = 10;
	rctGrid.bottom = rctGrid.right = 330;// 靠左
	rctBrush.top = 10; rctBrush.left = 340;
	rctBrush.bottom = 330;rctBrush.right = 660;// 靠右

	// 计算小格式中的坐标，保存在数组中
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
* 功能 根据InitGridBurshAndRect所初始化的小方格的边界
* 画出小方格。
**************************************/
VOID DrawGridLine(HWND hwnd)
{
	hdc = GetDC(hwnd);
	// 经线和纬线
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
* 功能 使用画刷填充ptlHit点所在的小方格
**************************************/
VOID PaintGrid(HWND hwnd,POINTS ptlHit)
{
	// 创建区域，大方格
	hrgnCell = CreateRectRgn(rctGrid.left, rctGrid.top,
		rctGrid.right, rctGrid.bottom);
	// 获得窗口的DC
	hdc = GetDC(hwnd);
	// 将区域选择入DC
	SelectObject(hdc, hrgnCell);
	// 测试点是否在区域中
	if (PtInRegion(hrgnCell, ptlHit.x, ptlHit.y))
	{
		// 如果点在大方格中，循环测试点在哪个小方格中
		for (i=0; i<8; i++)
			bBrushBits[i] = 0xFF;
		for(i=0; i<64; i++)
		{
			DeleteObject(hrgnCell);
			// 每一个小方格创建一个区域
			hrgnCell = CreateRectRgn(rect[i].left,
				rect[i].top,
				rect[i].right, rect[i].bottom);
			// 测试是否在小方格中
			if (PtInRegion(hrgnCell, ptlHit.x, ptlHit.y))
			{
				// 如果是的话则填充，第一行使用的填充画刷是一样的。
				FillRgn(hdc,hrgnCell,hBrushGrid[i/8]);
			}
		}
		// 释放
		SelectObject(hdc, hbrushOld);
		DeleteObject(hbrush);
		DeleteObject(hbm);
		DeleteDC( hdc);
	}
}

/*************************************
*VOID PaintPattern(HWND hwnd, POINTS ptlHit)
* 功能 在演示格式画刷的方格中使用格式画刷进行绘制
**************************************/
VOID PaintPattern(HWND hwnd, POINTS ptlHit)
{
	// 获取DC
	hdc = GetDC(hwnd);
	// 根据点击的不同位置创建不同的位图
	i = ptlHit.x % 8;
	bBrushBits[i] += 0x50;
	// 创建位图
	hbm = CreateBitmap(8, 8, 1, 1, (LPBYTE)bBrushBits);
	// 创建格式画刷
	hbrush = CreatePatternBrush(hbm);
	// 为DC选择画刷
	hbrushOld = SelectObject(hdc, hbrush);
	// 绘制矩形
	Rectangle(hdc, rctBrush.left, rctBrush.top,
		rctBrush.right, rctBrush.bottom);
	// 释放DC
	ReleaseDC(hwnd,hdc);
	DeleteDC( hdc );
}

/*************************************
* MainWndProc
* 功能 窗口消息处理函数
**************************************/
LRESULT CALLBACK MainWndProc(
							 HWND hwnd,
							 UINT uMsg,
							 WPARAM wParam,
							 LPARAM lParam)
{

	switch (uMsg)
	{
		// 释放资源，退出
	case WM_DESTROY:
		for(i=0; i<8; i++)
			DeleteObject(hBrushGrid[i]);
		ExitProcess(0);
		// 初始化画刷、方格
	case WM_CREATE:
		InitGridBurshAndRect();
		break;
		// 绘制
	case WM_PAINT:
		DrawGridLine(hwnd);
		break;
		// 鼠标左键
	case WM_LBUTTONDOWN:
		ptlHit = MAKEPOINTS((LPPOINTS)lParam);
		// 填充一个小方格
		PaintGrid(hwnd, ptlHit);
		// 填充右侧的格式画刷方格
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

	// 注册窗口类，并创建窗口，用于显示截取的位图
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
	// 创建窗口
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
	// 显示
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