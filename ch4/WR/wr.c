/* ************************************
*����ͨWindows API�� 
* ʾ������
* wr.c
* 4.3.2	�������򿪡���д�ļ�����ȡ�ļ���С
**************************************/

/* ͷ�ļ���*/
#include <windows.h>
#include <stdio.h>

/* ************************************
* DWORD ReadFileContent(LPSTR szFilePath)
* ����	��ȡ�ļ���С
*		��ȡ�ļ����ݣ�����16���Ƶ���ʽ��ӡ����
* ����	LPSTR szFilePath
*		�ļ�·��
**************************************/
DWORD ReadFileContent(LPSTR szFilePath)
{
	//�ļ���С
	HANDLE hFileRead;
	//�����ļ���С
	LARGE_INTEGER liFileSize;
	//�ɹ���ȡ���ļ����ݴ�С
	DWORD dwReadedSize;
	//�ۼӼ����Ѿ���ȡ���ݵĴ�С
	LONGLONG liTotalRead = 0;
	//�ļ����ݻ���
	BYTE lpFileDataBuffer[32];

	//���Ѿ����ڵ��ļ�����ȡ���ݡ�	
	hFileRead = CreateFile(szFilePath,// Ҫ�򿪵��ļ���
		GENERIC_READ,			   // �Զ���ʽ��
		FILE_SHARE_READ,           // �ɹ����
		NULL,                      // Ĭ�ϰ�ȫ����
		OPEN_EXISTING,             // ֻ���Ѿ����ڵ��ļ�
		FILE_ATTRIBUTE_NORMAL,     // �����ļ�����
		NULL);                     // ��ģ��

	//���ļ��Ƿ�ɹ���
	if(hFileRead==INVALID_HANDLE_VALUE)
	{
		printf("���ļ�ʧ�ܣ�%d",GetLastError());
	}

	if(!GetFileSizeEx(hFileRead,&liFileSize))
	{
		printf("��ȡ�ļ���Сʧ�ܣ�%d",GetLastError());
	}
	else
	{
		printf("�ļ���СΪ��%d\n",liFileSize.QuadPart);
	}

	//ѭ����ȡ����ӡ�ļ�����
	while(TRUE)
	{
		DWORD i;

		if(!ReadFile(hFileRead,	//���ļ��ľ��
			lpFileDataBuffer,	//�洢��ȡ���ļ�����
			32,					//���Ĵ�С���ֽڣ�
			&dwReadedSize,		//ʵ�ʶ�ȡ�Ĵ�С
			NULL))				//��ʹ��Overlapped
		{
			printf("���ļ�����%d\n",GetLastError());
			break;
		}
		printf("��ȡ��%d�ֽڣ��ļ������ǣ�",dwReadedSize);
		
		for(i=0; i<dwReadedSize; i++)
		{
			printf("0x%x ",lpFileDataBuffer[i]);
		}
		printf("\n");
		liTotalRead += dwReadedSize;
		if(liTotalRead == liFileSize.QuadPart)
		{
			printf("���ļ�����\n");
			break;
		}
	}
	CloseHandle(hFileRead);
	return 0;
}

/* ************************************
*  SaveDataToFile
* ����	�����ݴ洢���ļ�ĩβ
* ����	LPSTR szFilePath	�ļ�·��
*		LPVOID lpData		��洢������
*		DWORD dwDataSize	���ݴ�С���ֽڣ�
**************************************/
DWORD SaveDataToFile(
					 LPSTR szFilePath,
					 LPVOID lpData,
					 DWORD dwDataSize)
{
	//�ļ����
	HANDLE hFileWrite;
	//�ɹ�д������ݴ�С
	DWORD dwWritedDateSize;

	//���Ѿ����ڵ��ļ�����ȡ���ݡ�	
	hFileWrite = CreateFile(szFilePath,	// Ҫ�򿪵��ļ���
		GENERIC_WRITE,			// ��д��ʽ��
		0,						// �ɹ����
		NULL,					// Ĭ�ϰ�ȫ����
		OPEN_ALWAYS,			// ���Ѿ����ڵ��ļ���û���򴴽�
		FILE_ATTRIBUTE_NORMAL,	// �����ļ�����
		NULL);					// ��ģ��
	
	//�ж��Ƿ�򿪳ɹ�
	if(hFileWrite==INVALID_HANDLE_VALUE)
	{
		printf("���ļ�ʧ�ܣ�%d\n",GetLastError());
	}

	//�����ļ�ָ�뵽�ļ�β
	SetFilePointer(hFileWrite,0,0,FILE_END);

	//������д���ļ�
	if(!WriteFile(hFileWrite,lpData,dwDataSize,&dwWritedDateSize,NULL))
	{
		printf("д�ļ�ʧ�ܣ�%d\n",GetLastError());
	}
	else
	{
		printf("д�ļ��ɹ���д��%d�ֽڡ�\n",dwWritedDateSize);
	}
	CloseHandle(hFileWrite);
	return 0;
}

/* ************************************
* int main(void)
* ����	��ʾʹ��SaveDataToFile��ReadFileContent����
**************************************/
int main(void)
{
	LPSTR szFileData = "����һ������";
	SaveDataToFile("C:\\show.txt",szFileData,lstrlen(szFileData));
	ReadFileContent("C:\\show.txt");
	return 0;
}