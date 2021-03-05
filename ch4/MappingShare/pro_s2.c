/* ************************************
*����ͨWindows API��
* ʾ������
* pro_s2.c
* 4.4.2  ͨ��Mapping File�ڽ��̼乲���ڴ�
**************************************/
#include <windows.h>
#include <stdio.h>
#include <conio.h>
/* Ԥ��������*/
#pragma comment (lib, "User32.lib")
#define BUF_SIZE 256
/* ȫ�ֱ�����*/
TCHAR szName[]=TEXT("SharedFileMappingObject");

/* ************************************
* int main(void)
* ����	��ʾ�ļ�mapping�����ڴ棬�ӹ��������ж���Ϣ
*
* ����	��
*
* ����ֵ	0����ִ����ɣ�����������
**************************************/
void main()
{
	HANDLE hMapFile;
	LPTSTR pBuf;
	//���ļ�mapping
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		szName); 
	if (hMapFile == NULL) 
	{ 
		printf("OpenFileMapping error: %d.\n",  GetLastError());
		return;
	} 
	//ӳ��
	pBuf = MapViewOfFile(hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,                    
		0,                    
		BUF_SIZE);  
	if (pBuf == NULL) 
	{ 
		printf("MapViewOfFile error %d\n", GetLastError()); 
		return;
	}
	//��Ϣ�õ��Ĺ�������
	MessageBox(NULL, pBuf, TEXT("Process2"), MB_OK);
	//ȡ��mapping���رվ��������
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);
}