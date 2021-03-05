/* ************************************
 *����ͨWindows API�� 
 * ʾ������
 * diskspace.c
 * 4.2.4	��ȡ���̿ռ���Ϣ
 **************************************/

/* ͷ�ļ���*/
#include <windows.h>
#include <stdio.h>

/* ************************************
 * BOOL GetDiskSpaceInfo(LPCSTR pszDrive
 * ����	�������������������ȡ����������
 *			���пռ䡢�������ȴ�����Ϣ
 * ����	��������·�������硰D:\����
 **************************************/
BOOL GetDiskSpaceInfo(LPCSTR pszDrive)
{
	DWORD64 qwFreeBytesToCaller, qwTotalBytes, qwFreeBytes;
	DWORD dwSectPerClust, dwBytesPerSect, dwFreeClusters,  dwTotalClusters;
	BOOL bResult;
	
	//ʹ��GetDiskFreeSpaceEx��ȡ������Ϣ����ӡ���
	bResult = GetDiskFreeSpaceEx (pszDrive,
		(PULARGE_INTEGER)&qwFreeBytesToCaller,
		(PULARGE_INTEGER)&qwTotalBytes,
		(PULARGE_INTEGER)&qwFreeBytes);

	if(bResult) 
	{
		printf("ʹ��GetDiskFreeSpaceEx��ȡ���̿ռ���Ϣ\n");
		printf("�ɻ�õĿ��пռ䣨�ֽڣ�: \t%I64d\n", qwFreeBytesToCaller);
		printf("���пռ䣨�ֽڣ�: \t\t%I64d\n", qwFreeBytes);
		printf("�������������ֽڣ�: \t\t%I64d\n", qwTotalBytes);
	}

	//ʹ��GetDiskFreeSpace��ȡ������Ϣ����ӡ���
	bResult = GetDiskFreeSpace (pszDrive, 
		&dwSectPerClust, 
		&dwBytesPerSect,
		&dwFreeClusters, 
		&dwTotalClusters);

	if(bResult) 
	{
		printf("\nʹ��GetDiskFreeSpace��ȡ���̿ռ���Ϣ\n");
		printf("���еĴ����� : \t\t\t%d\n",dwFreeClusters);
		printf("�ܴ����� : \t\t\t%d\n",dwTotalClusters);
		printf("ÿ�ص��������� : \t\t%d\n",dwSectPerClust);
		printf("ÿ�������������ֽڣ�: \t\t%d\n",dwBytesPerSect);
		printf("���пռ䣨�ֽڣ�: \t\t%I64d\n", 
			(DWORD64)dwFreeClusters*
			(DWORD64)dwSectPerClust*(DWORD64)dwBytesPerSect);
		printf("�������������ֽڣ�: \t\t%I64d",
			(DWORD64)dwTotalClusters*
			(DWORD64)dwSectPerClust*(DWORD64)dwBytesPerSect);
	}
	return bResult;
}

/* ************************************
 * int main( int argc, PCHAR argv[] )
 * ����	Ӧ�ó����������������������
 *			����GetDiskSpaceInfo������ȡ
 *			���̿ռ���Ϣ
 * ����	��������·�������硰D:\����
 **************************************/
int main(int argc, PCHAR argv[])
{
	GetDiskSpaceInfo (argv[1]);
}