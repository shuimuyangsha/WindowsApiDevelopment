/* ************************************
*����ͨWindows API�� 
* ʾ������
* start.c
* 1.1.1	��һ��ʾ�����򣬵�����Ϣ�Ի���
**************************************/

/* Ԥ����*/
/* ͷ�ļ���*/
#include <windows.h>
//����ʱʹ��User32.lib
#pragma comment (lib, "User32.lib")

/* ************************************
* WinMain
* ����	WindowsӦ�ó���ʾ��
**************************************/
int WinMain(
			HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR lpCmdLine,
			int nCmdShow
			)
{
	// ����API���� MessageBox
	MessageBox(NULL,
		TEXT("��ʼѧϰWindows���"),
		TEXT("��Ϣ�Ի���"),
		MB_OK);
	return 0;
}