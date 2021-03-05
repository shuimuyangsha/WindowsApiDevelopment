/* ************************************
*����ͨWindows API��
* ʾ������
* text.c
* 13.4 �ı�������
**************************************/
/* ͷ�ļ� */
#include <Windows.h>
/*************************************
* HFONT ChooseNewFont()  
* ���� ѡ������
*
* ����ֵ ����������
**************************************/
HFONT ChooseNewFont() 
{ 
    CHOOSEFONT cf; 
    LOGFONT lf; 
    HFONT hfont; 
 
    // CHOOSEFONT �ṹ 
    cf.lStructSize = sizeof(CHOOSEFONT); 
    cf.hwndOwner = (HWND)NULL; 
    cf.hDC = (HDC)NULL; 
    cf.lpLogFont = &lf; 
    cf.iPointSize = 0; 
    cf.Flags = CF_SCREENFONTS; 
    cf.rgbColors = RGB(0,0,0); 
    cf.lCustData = 0L; 
    cf.lpfnHook = (LPCFHOOKPROC)NULL; 
    cf.lpTemplateName = (LPSTR)NULL; 
    cf.hInstance = (HINSTANCE) NULL; 
    cf.lpszStyle = (LPSTR)NULL; 
    cf.nFontType = SCREEN_FONTTYPE; 
    cf.nSizeMin = 0; 
    cf.nSizeMax = 0; 
 
    // ѡ������Ի��� 
    ChooseFont(&cf); 
	// �õ�HFONT ����
    hfont = CreateFontIndirect(cf.lpLogFont); 
    return (hfont); 
} 
/*************************************
* WinMain  
* ���� ѡ�����壬����������ʾ�ڽ�����
*
**************************************/
int WINAPI WinMain(
			HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR lpCmdLine,
			int nCmdShow
			)
{
	HDC hdc = GetDC(NULL);

	int XIncrement = 10;
	int YStart = 50;
	TEXTMETRIC tm; 
	HFONT hfntDefault, hfntChoose; 
	SIZE sz; 
	UINT uAlignPrev;
	LPSTR lpszString1 = "�ַ���һ"; 
	LPSTR lpszString2 = "�ַ�����"; 
	LPSTR lpszString3 = "�ַ�����";  
	DWORD dwStrLen1 = lstrlen(lpszString1);
	DWORD dwStrLen2 = lstrlen(lpszString2);
	DWORD dwStrLen3 = lstrlen(lpszString3);

	// ѡ������
	hfntChoose = ChooseNewFont(); 
	// ������ɫ
	SetBkColor(hdc,RGB(255,255,255));
	SetTextColor(hdc,RGB(255,0,0));
	SetBkMode(hdc,TRANSPARENT);

	// ����ַ���һ
	TextOut(hdc, XIncrement, YStart, lpszString1, dwStrLen1); 

	// Ϊ�ַ������������λ��
	GetTextExtentPoint32(hdc, lpszString1, dwStrLen1, &sz); 
	XIncrement += sz.cx; 
	GetTextMetrics(hdc, &tm); 
	XIncrement -= tm.tmOverhang; 
	// �ı�����
	hfntDefault = SelectObject(hdc, hfntChoose); 	
	// ����ַ�����
	TextOut(hdc, XIncrement, YStart, lpszString2, dwStrLen2); 
	// �����ַ����������λ��
	GetTextExtentPoint32(hdc, lpszString1, dwStrLen1, &sz); 
	XIncrement = 10; 
	YStart += sz.cy;
	GetTextMetrics(hdc, &tm); 
	XIncrement -= tm.tmOverhang;
	// ����ΪĬ������
	SelectObject(hdc, hfntDefault);
	// ����ַ�����
	uAlignPrev = SetTextAlign(hdc, TA_UPDATECP);    
	MoveToEx(hdc, XIncrement, YStart, (LPPOINT)NULL);
	TextOut(hdc, 0, 0, lpszString3, dwStrLen3); 
	SetTextAlign(hdc, uAlignPrev); 
	// Clear
	DeleteObject(hfntChoose); 
	SetBkMode(hdc, OPAQUE); 
	DeleteDC( hdc );
	return 0;
}