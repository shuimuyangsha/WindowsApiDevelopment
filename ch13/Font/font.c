/* ************************************
*《精通Windows API》
* 示例代码
* font.c
* 13.4.6 遍历字体
**************************************/
/* 头文件 */
#include <Windows.h>
#include <stdio.h>
/* 函数声明 */
BOOL CALLBACK EnumFamCallBack(LPLOGFONT , LPNEWTEXTMETRIC , DWORD , LPVOID ) ;
DWORD ListFont();
// main
int main()
{
	// 桌面DC
	ListFont(NULL);
}
/*************************************
* DWORD ListFont(HWND hwnd)  
* 功能	列举指定窗口的DC的所具有的字体
**************************************/
DWORD ListFont(HWND hwnd)
{
	// 获得DC
	HDC hdc = GetDC(hwnd);
    // 用于计数
	int aFontCount[] = { 0, 0, 0 }; 
	// 调用EnumFontFamilies，开始列举，
	
    EnumFontFamilies(hdc, (
		LPCTSTR) NULL, // 列举所有类型
        (FONTENUMPROC) EnumFamCallBack,// 回调函数为EnumFamCallBack
		(LPARAM) aFontCount); //传递给EnumFamCallBack的参数

	// 显示统计信息
	printf("Number of raster fonts: %d\n",aFontCount[0]);
	printf("Number of vector fonts: %d\n",aFontCount[1]);
	printf("Number of TrueType fonts: %d\n",aFontCount[2]);
	// 返回
	return 0;
}
 
/*************************************
* EnumFamCallBack
* 功能	字体列举回调函数次
*			每列举一个字体会被调用一次
* 参数	lplf，字体的LOGFONT结构
*			lpntm，字符的尺度属性
*			FontType，字体类型
*			lParam，通过EnumFontFamilies输入给本函数的参数，这里用于计数
**************************************/
BOOL CALLBACK EnumFamCallBack(
							  LPLOGFONT lplf, 
							  LPNEWTEXTMETRIC lpntm, 
							  DWORD FontType, 
							  LPVOID aFontCount) 
{ 
	// 获得参数
    PINT aiFontCount = (PINT) aFontCount; 
	// 判断字体类型，输出类型信息，并根据类型进行计数
    if (FontType & RASTER_FONTTYPE) 
	{
		printf(" RASTER TYPE \t");
        aiFontCount[0]++; 
	}
    else if (FontType & TRUETYPE_FONTTYPE)
	{
		printf(" TRUETYPE \t");
        aiFontCount[2]++;
	}
    else 
	{
		printf(" VECTOR TYPE \t");
        aiFontCount[1]++; 
	}
	// 显示字体信息
	printf("%s\tItalic = %d\n",lplf->lfFaceName,lplf->lfItalic);
    // 返回
	if (aiFontCount[0] || aiFontCount[1] || aiFontCount[2]) 
        return TRUE; 
    else 
        return FALSE; 
} 