/* ************************************
*����ͨWindows API�� 
* ʾ������
* windata.c
* 2.1.1	���õ�Windows��������
**************************************/

/* ͷ�ļ���*/
#include <windows.h>
#include <stdio.h>

/* ************************************
* ����	Windows ����������ʾ
**************************************/
int WINAPI WinMain(
            HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPSTR lpCmdLine,
            int nCmdShow
            )
{
    //�����ַ���
    LPSTR szString = "Windows data type, string.";
    //�����ַ�����
    CHAR lpString[120];//Ҫ����szString�ĳ���
    //����DWORD���͵�����
    DWORD dwMax = 0xFFFFFFFF;
    DWORD dwOne = 0x1;
    //����INT���͵�����
    INT iMax = 0xFFFFFFFF;
    INT iOne = 0x1;
    
    //��ʾ�ַ���
    MessageBox(NULL,szString,"LPSTR",MB_OK);
    //�����ڴ棬���ַ������Ƶ������У�����NULL��������
    CopyMemory(lpString, szString,lstrlen(szString)+1);
    //��ʾ���Ƶ��ַ���
    MessageBox(NULL,lpString,"CHAR[]",MB_OK);

    //�Ƚ�DWORD����ʾ���
    if(dwMax>dwOne)
    {
        MessageBox(NULL,"DWORD���͵����� OxFFFFFFFF > 0x1","DWORD",MB_OK);
    }
    //�Ƚ�INT����ʾ���
    if(iMax<iOne)
    {
        MessageBox(NULL,"INT���͵����� OxFFFFFFFF < 0x1","INT",MB_OK);
    }
    return 0;
}