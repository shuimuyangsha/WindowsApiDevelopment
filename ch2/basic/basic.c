/* ************************************
*����ͨWindows API�� 
* ʾ������
* basic.c
* 2.2  Windows API�Ĺ��ܷ���
**************************************/

/* ͷ�ļ���*/
#include <windows.h>

/* ************************************
* ����	��ȡϵͳĿ¼��Ϣ�����洢���ļ���
**************************************/
int main(int argc, TCHAR argv[])
{
    //�ļ����
    HANDLE hFile;

    DWORD dwWritten;
    //�ַ����飬���ڴ洢ϵͳĿ¼
    TCHAR szSystemDir[MAX_PATH];
    //��ȡϵͳĿ¼
    GetSystemDirectory(szSystemDir,MAX_PATH);

    //�����ļ�systemroot.txt
    hFile = CreateFile("systemroot.txt",
        GENERIC_WRITE,
        0,NULL,CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    //�ж��ļ��Ƿ񴴽��ɹ�
    if(hFile != INVALID_HANDLE_VALUE)
    {
        //��ϵͳĿ¼ϵͳ��Ϣд���ļ�
        if(!WriteFile(hFile,szSystemDir,lstrlen(szSystemDir),&dwWritten,NULL))
        {
            return GetLastError();
        }
    }
    //�ر��ļ������ء�
    CloseHandle(hFile);
    return 0;
}