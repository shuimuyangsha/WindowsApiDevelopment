/* ************************************
*����ͨWindows API�� 
* ʾ������
* sub_dir.c
* 4.3.5	����Ŀ¼�µ��ļ�����Ŀ¼
**************************************/

/* ͷ�ļ���*/
#include <windows.h>
#include <stdio.h>

/* ************************************
* DWORD EnumerateFileInDrectory(LPSTR szPath)
* ����	����Ŀ¼�µ��ļ�����Ŀ¼������ʾ�ļ���
*		�ļ����ļ������ء����ܵ�����
* ����	LPTSTR szPath��Ϊ�������·��
* ����ֵ	0����ִ����ɣ�1���뷢������
**************************************/
DWORD EnumerateFileInDrectory(LPSTR szPath)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile;
	CHAR szFilePath[MAX_PATH];
	
	//���������Ŀ¼���ļ���·�����ַ�����ʹ��ͨ�����*��
	lstrcpy(szFilePath, szPath);	
	//ע�͵Ĵ���������ڲ��������ԡ�.txt��β�����ļ���
	//lstrcat(szFilePath, "\\*.txt");
	lstrcat(szFilePath, "\\*");
	//���ҵ�һ���ļ�/Ŀ¼����ò��Ҿ��
	hListFile = FindFirstFile(szFilePath,&FindFileData);
	//�жϾ��
	if(hListFile==INVALID_HANDLE_VALUE)
	{
		printf("����%d",GetLastError());
		return 1;
	}
	else
	{		
		do
		{
			/*	���������ʾ������Ŀ¼���ϼ�Ŀ¼�ġ�.���͡�..����
				����ʹ��ע�Ͳ��ֵĴ�����ˡ�
			if(lstrcmp(FindFileData.cFileName,TEXT("."))==0||
				lstrcmp(FindFileData.cFileName,TEXT(".."))==0)
			{
				continue;
			}
			*/
			//��ӡ�ļ�����Ŀ¼��
			printf("%s\t\t",FindFileData.cFileName);
			//�ж��ļ����ԣ������ļ����ļ���
			if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_ENCRYPTED)
			{
				printf("<����> ");
			}
			//�ж��ļ����ԣ������ļ����ļ���
			if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN)
			{
				printf("<����> ");
			}
			//�ж��ļ����ԣ�Ŀ¼
			if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				printf("<DIR> ");
			}
			//���߿ɸ����ļ����Ա��е�������������ж��ļ����ԡ�
			printf("\n");
		}
		while(FindNextFile(hListFile, &FindFileData));
	}
	return 0;
}

/* ************************************
* int main(int argc, PCHAR argv[])
* ����	����ListFileInDrectory
*		����Ŀ¼�µ��ļ�����Ŀ¼
* ����	argv[1]Ϊ�������·�������Ϊ�����ȡ
*		��ǰ·��
**************************************/
int main(int argc, PCHAR argv[])
{	
	if(argc == 2)
	{
		EnumerateFileInDrectory(argv[1]);
	}
	else 
	{
		CHAR szCurrentPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,szCurrentPath);
		EnumerateFileInDrectory(szCurrentPath);
	}
	return 0;
}