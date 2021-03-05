/* ************************************
*����ͨWindows API�� 
* ʾ������
* opt.c
* 5.5.2  ��õ�ǰϵͳ�ڴ�ʹ�����
**************************************/

/* ͷ�ļ���*/
#include <windows.h>
#include <stdio.h>

/* �������� */
#define MEM_BLOCK_MAX_SIZE 32

/*************************************
* BOOL ShowMemContent(LPVOID lpMem,SIZE_T dwSize)
* ����	��ʾ�ڴ��е�����
*
* ����	LPVOID lpMem ��Ҫ��ʾ���ڴ�ָ��
*		SIZE_T dwSize ��С
*
* ����ֵ BOOL ������ݹ������������򷵻�FALSE�� 
**************************************/
BOOL ShowMemContent(LPVOID lpMem,SIZE_T dwSize)
{
	BYTE lpShow[MEM_BLOCK_MAX_SIZE];
	INT i=0;
	//��ֹջ���
	if(dwSize>MEM_BLOCK_MAX_SIZE)
	{
		printf("over-flow");
		return FALSE;
	}
	//���Ƶ�����
	CopyMemory((LPVOID)lpShow,lpMem,dwSize);
	//��ʾ
	for(; i<dwSize; i++)
	{		
		printf("%.2X ",lpShow[i]);
		if(!((i+1)%16))
		{
			printf("\n");
		}		
	}
	printf("\n");
	return TRUE;
}

/*************************************
* int main(void)
* ����	��ȡ�ڴ�ʹ�����
*
* ����	δʹ��
**************************************/
int main(void)
{	
	HANDLE hHeap = GetProcessHeap();
	LPVOID lpSrc;
	LPVOID lpDis;

	//�����ڴ�
	lpSrc = HeapAlloc(hHeap,0,MEM_BLOCK_MAX_SIZE);
	lpDis = HeapAlloc(hHeap,0,MEM_BLOCK_MAX_SIZE);
	//��ʾ�·�����ڴ�
	printf("HeapAlloc ���䵫�����㣺\n");
	ShowMemContent(lpDis, MEM_BLOCK_MAX_SIZE);	
	//���ڴ����㲢��ʾ
	ZeroMemory(lpDis,MEM_BLOCK_MAX_SIZE);
	printf("ZeroMemory ���㣺\n");
	ShowMemContent(lpDis, MEM_BLOCK_MAX_SIZE);
	//ʹ��0xBB���ȫ���ڴ�
	FillMemory(lpSrc,MEM_BLOCK_MAX_SIZE,0xBB);
	//���ڴ���ǰ�벿��ʹ��0xAA���
	FillMemory(lpSrc,MEM_BLOCK_MAX_SIZE/2,0xAA);
	CopyMemory(lpDis,lpSrc,MEM_BLOCK_MAX_SIZE);
	printf("FillMemory �й��ɵ�����ڴ棺\n");
	ShowMemContent(lpDis,MEM_BLOCK_MAX_SIZE);

	//�ͷ��ڴ�
	HeapFree(hHeap,0,lpSrc);
	HeapFree(hHeap,0,lpDis);
	return 0;

}