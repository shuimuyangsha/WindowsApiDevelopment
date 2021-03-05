/* ************************************
*����ͨWindows API��
* ʾ������
* gdi.c
* 13.3  ��򵥵�GDI����
**************************************/
/* ͷ�ļ� */
#include <Windows.h>
/* �������� */
void GdiOut(HDC hdc);

// WinMain
int WINAPI WinMain(
				   HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow
				   )
{
	HDC hdc = GetDC(NULL);
	GdiOut(hdc);
	ReleaseDC(NULL, hdc);
}
/*************************************
* VOID GdiOut(HDC hdc) 
* ���� ��ʾGDI�������ܵ�ʹ��
* ���� HDC hdc��������ʾ�����Ƶ�ͼ��
* �޷���ֵ
**************************************/
VOID GdiOut(HDC hdc) 
{
	HPEN hpen, hpenOld;
	HBRUSH hbrush, hbrushOld;
	// ��ʼ����ɫ
	BYTE bRed  = 0;
	BYTE bGreen  = 0;
	BYTE bBlue = 0;
	// �ʵ���ɫ ����
	COLORREF cPen = RGB(bRed, bGreen, bBlue);
	// ��COLORREF���Ͳ����ɫ�����ñ�ˢ����ɫ������Ϊ��ƫ��
	COLORREF cBrush = RGB(233, GetGValue(cPen),255);
	// ������
	hpen = CreatePen(PS_SOLID, 10, cPen);
	// ������ˢ
	hbrush = CreateSolidBrush(cBrush);
	// ΪDCѡ��ʺͱ�ˢ
	hpenOld = SelectObject(hdc, hpen);
	hbrushOld = SelectObject(hdc, hbrush);
	// ��������
	LineTo(hdc,500,500);
	// ʹ�ó�ʼ�ı�
	SelectObject(hdc,hpenOld);
	// ���ƾ���
	Rectangle( hdc, 200, 200, 500, 500 );
	// �ͷ���Դ
	DeleteObject(hpen);
	SelectObject(hdc, hbrushOld);
	DeleteObject(hbrush);
}