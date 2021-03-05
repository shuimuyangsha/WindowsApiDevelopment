/* ************************************
*����ͨWindows API�� 
* ʾ������
* mlclt.c
* 11.1 ͨ��mailslot���̼�ͨ��
**************************************/
/* ͷ�ļ� */
#include <windows.h>
#include <stdio.h>
/* ȫ�ֱ��� */
HANDLE hSlot;
LPTSTR lpszSlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");		// mailslot��
LPTSTR lpszMessage = TEXT("Test Message for mailslot "); // ͨ�ŵ�����

/* ************************************
* void main()
* ����	���̼�mailslotͨ�ſͻ���
**************************************/
void main()
{ 
	BOOL fResult; 
	HANDLE hFile; 
	DWORD cbWritten; 

	DWORD cbMessage;
	// ��mailslot
	hFile = CreateFile(lpszSlotName, 
		GENERIC_WRITE,		// ��д
		FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES) NULL, 
		OPEN_EXISTING,		// ��һ���Ѿ����ڵ�mailslot��Ӧ���ɷ�����Ѿ�����
		FILE_ATTRIBUTE_NORMAL, 
		(HANDLE) NULL); 

	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		printf("CreateFile failed with %d.\n", GetLastError()); 
		return ; 
	}
	// ��mailslotд��
	fResult = WriteFile(hFile, 
		lpszMessage, 
		(DWORD) (lstrlen(lpszMessage)+1)*sizeof(TCHAR),  
		&cbWritten, 
		(LPOVERLAPPED) NULL); 

	if (!fResult) 
	{ 
		printf("WriteFile failed with %d.\n", GetLastError()); 
		return ; 
	} 
	// ����
	printf("Slot written to successfully.\n"); 
	CloseHandle(hFile); 
	return ;
}