/* ************************************
 *����ͨWindows API�� 
 * ʾ������
 * attr.c
 * 4.3.7  ��ȡ�������ļ����Ժ�ʱ��
 **************************************/

/* ͷ�ļ���*/
#include <windows.h>
#include <stdio.h>

/* ����������*/
DWORD ShowFileTime(PFILETIME lptime);
DWORD ShowFileSize(DWORD dwFileSizeHigh,DWORD dwFileSizeLow);
DWORD ShowFileAttrInfo(DWORD dwAttribute);
DWORD SetFileHiddenAndReadonly(LPSTR szFileName);

/* ************************************
 * DWORD ShowFileAttributes(LPSTR szPath)
 * ����	��ȡ����ʾ�ļ����ԣ�
 *		����ShowFileTime��ShowFileSize��
 *		ShowFileAttrInfo����
 *
 * ����	LPTSTR szPath����ȡ����ʾ���ļ�������
 *
 * ����ֵ	0����ִ����ɣ�1���뷢������
 **************************************/
DWORD ShowFileAttributes(LPSTR szPath)
{
	//�ļ����Խṹ
	WIN32_FILE_ATTRIBUTE_DATA wfad;
	printf("�ļ���%s\n",szPath);
	//��ȡ�ļ�����
	if(!GetFileAttributesEx(szPath,
		GetFileExInfoStandard,
		&wfad))
	{
		printf("��ȡ�ļ����Դ���%d\n",GetLastError());
		return 1;
	}
	//��ʾ���ʱ��
	printf("����ʱ�䣺\t");
	ShowFileTime(&(wfad.ftCreationTime));
	printf("������ʱ�䣺\t");
	ShowFileTime(&(wfad.ftLastAccessTime));
	printf("����޸�ʱ�䣺\t");
	ShowFileTime(&(wfad.ftLastWriteTime));
	//��ʾ�ļ���С
	ShowFileSize(wfad.nFileSizeHigh,wfad.nFileSizeLow);
	//��ʾ�ļ�����
	ShowFileAttrInfo(wfad.dwFileAttributes);

	return 0;
}

/* ************************************
 * DWORD ShowFileAttrInfo(DWORD dwAttribute)
 * ����	��ӡ���ļ�����
 *
 * ����	DWORD dwAttribute���ļ�����
 *
 * ����ֵ 0
 **************************************/
DWORD ShowFileAttrInfo(DWORD dwAttribute)
{
	//�����ж����ԣ�����ʾ��
	printf("�ļ����ԣ�\t");
	if(dwAttribute&FILE_ATTRIBUTE_ARCHIVE)
		printf("<ARCHIVE> ");
	if(dwAttribute&FILE_ATTRIBUTE_COMPRESSED)
		printf("<ѹ��> ");
	if(dwAttribute&FILE_ATTRIBUTE_DIRECTORY)
		printf("<Ŀ¼> ");
	if(dwAttribute&FILE_ATTRIBUTE_ENCRYPTED)
		printf("<����> ");
	if(dwAttribute&FILE_ATTRIBUTE_HIDDEN)
		printf("<����> ");
	if(dwAttribute&FILE_ATTRIBUTE_NORMAL)
		printf("<NORMAL> ");
	if(dwAttribute&FILE_ATTRIBUTE_OFFLINE)
		printf("<OFFLINE> ");
	if(dwAttribute&FILE_ATTRIBUTE_READONLY)
		printf("<ֻ��> ");
	if(dwAttribute&FILE_ATTRIBUTE_SPARSE_FILE)
		printf("<SPARSE> ");
	if(dwAttribute&FILE_ATTRIBUTE_SYSTEM)
		printf("<ϵͳ�ļ�> ");
	if(dwAttribute&FILE_ATTRIBUTE_TEMPORARY)
		printf("<��ʱ�ļ�> ");

	printf("\n");

	return 0;
}
/* ************************************
 * DWORD ShowFileSize(DWORD dwFileSizeHigh, DWORD dwFileSizeLow)
 * ����	��ӡ�ļ���С��Ϣ
 *
 * ����	DWORD dwFileSizeHigh���ļ���С��32λ
 *		DWORD dwFileSizeLow���ļ���С��32λ
 *
 * ����ֵ 0
 **************************************/
DWORD ShowFileSize(DWORD dwFileSizeHigh, DWORD dwFileSizeLow)
{
	ULONGLONG liFileSize;
	liFileSize = dwFileSizeHigh;

	//�����ƶ�32λ
	liFileSize <<= sizeof(DWORD)*8;
	liFileSize += dwFileSizeLow;
	printf("�ļ���С��\t%I64u �ֽ�\n",liFileSize);
	return 0;
}

/* ************************************
 *DWORD ShowFileTime(PFILETIME lptime)
 * ����	�ֻ��ļ�ʱ�䣬����ӡ
 *
 * ����	PFILETIME lptime��ָ���ļ�ʱ���ָ��
 *
 * ����ֵ 0
 **************************************/
DWORD ShowFileTime(PFILETIME lptime)
{
	//�ļ�ʱ��ṹ
	FILETIME ftLocal;
	//ϵͳʱ��ṹ
	SYSTEMTIME st;
	//����Ϊϵͳ����ʱ����ʱ��
	FileTimeToLocalFileTime(
		lptime,
		&ftLocal
		);
	//���ļ�ʱ��ת��ΪSYSTEMTIME��ʽ��������ʾ��
	FileTimeToSystemTime(
		&ftLocal,
		&st
		);
	//��ʾʱ����Ϣ�ַ���
	printf("%4d��%.2d��%#02d�գ�%.2d:%.2d:%.2d\n",
		st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

	return 0;
}

/* ************************************
 * DWORD SetFileHiddenAndReadonly(LPSTR szFileName)
 * ����	��ָ�����ļ�����Ϊ���غ�ֻ��
 *
 * ����	LPSTR szFileName���ļ�·��
 *
 * ����ֵ 0
 **************************************/
DWORD SetFileHiddenAndReadonly(LPSTR szFileName)
{
	//��ȡԭ�����ļ�����
	DWORD dwFileAttributes = GetFileAttributes(szFileName);
	//��ֻ�����������Ը��ӵ�ԭ�����ļ�������
	dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
	dwFileAttributes |= FILE_ATTRIBUTE_HIDDEN;
	//�����ļ����ԣ����ж��Ƿ�ɹ���
	if(SetFileAttributes(szFileName, dwFileAttributes))
	{
		printf("�ļ�%s�����غ��������óɹ�\n",szFileName);
	}
	else
	{
		printf("��������; %d\n",GetLastError());
	}
	return 0;
}

/* ************************************
 * int main(int argc, PCHAR argv[])
 * ����	���úͻ�ȡ�ļ����Ե�
 *
 * ����	��ʾ��һ������ָ���ļ������ԡ�ʱ�䡢��С
 *		���ڶ�����������������Ϊ���ء�ֻ����
 *
 * ����ֵ	0����ִ����ɣ�1���뷢������
 **************************************/
int main(int argc, PCHAR argv[])
{	
	if(argc != 3)
	{
		printf("���������\n");
		printf("��ʾ��һ������ָ���ļ������ԡ�ʱ�䡢��С��\n");
		printf("���ڶ�����������������Ϊ���ء�ֻ����");
		return 1;
	}
	ShowFileAttributes(argv[1]);
	SetFileHiddenAndReadonly(argv[2]);
	return 0;
}