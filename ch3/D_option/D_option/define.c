/* ************************************
*����ͨWindows API�� 
* ʾ������
* define.c
* 3.1.6  ʾ����ʹ��/Dѡ�������������
**************************************/

/* Ԥ����*/
/* ͷ�ļ���*/
#include <stdio.h>

/* �ж����� */
#ifdef DEBUG_PRINT
#define MyPrint printf("hello\n")
#else
#define MyPrint
#endif

// main����
int main()
{
	MyPrint;
	return 0;
}