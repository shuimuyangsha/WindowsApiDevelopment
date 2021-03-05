/* ************************************
 *����ͨWindows API�� 
 * ʾ������
 * files.c
 * 4.3.1	ɾ�������ơ����������ƶ��ļ�
 **************************************/

/* ͷ�ļ���*/
#include <windows.h>
#include <stdio.h>

/* ************************************
 * int main( int argc, PCHAR argv[] )
 * ����	Ӧ�ó����������������������
 *		ɾ�������ơ��������ļ�
 *
 * ����	ɾ���ļ���
 *			-d �ļ�·��
 *		���ļ�·��1���ļ����Ƶ��ļ�·��2��
 *			-c �ļ�·��1 �ļ�·��2
 *		���ļ�·��1���ļ��ƶ���������Ϊ�ļ�·��2���ļ�
 *			-m �ļ�·��1 �ļ�·��2
 **************************************/
int main(int argc, PCHAR argv[])
{
	//-d������ɾ���ļ���
	if(0==lstrcmp("-d",argv[1]) && argc==3)
	{
		if(!DeleteFile(argv[2]))
		{
			printf("ɾ���ļ�����%x\n",GetLastError());
		}
		else
		{
			printf("ɾ���ɹ���\n");
		}
	}
	//-c�����������ļ���
	//����ļ����ڣ�ѯ���û��Ƿ񸲸�
	else if(0==lstrcmp("-c",argv[1]) && argc==4)
	{
		//���ƣ��������Ѿ����ڵ��ļ�
		if(!CopyFile(argv[2],argv[3],TRUE))
		{
			//LastError == 0x50���ļ����ڡ�
			if(GetLastError() == 0x50)
			{
				printf("�ļ�%s�Ѿ����ڣ��Ƿ񸲸ǣ�y/n��",argv[3]);
				if('y'==getchar())
				{
					//���ƣ������Ѿ����ڵ��ļ���
					if(!CopyFile(argv[2],argv[3],FALSE))
					{
						printf("�����ļ�����%d\n",GetLastError());
					}
					else
					{
						printf("���Ƴɹ���\n");
					}
				}
				else
				{
					return 0;
				}
			}
		}
		else
		{
			printf("���Ƴɹ���\n");
		}
	}
	//-m�������ƶ����������ļ���
	else if(0==lstrcmp("-m",argv[1]) && argc==4)
	{
		if(!MoveFile(argv[2],argv[3]))
		{
			printf("�ƶ��ļ�����%d\n",GetLastError());
		}
		else
		{
			printf("�ƶ��ļ��ɹ���\n");
		}
	}
	else
	{
		printf("��������\n");
	}
}

