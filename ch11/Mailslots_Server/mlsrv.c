/* ************************************
*����ͨWindows API�� 
* ʾ������
* mlsrv.c
* 11.1 ͨ��mailslot���̼�ͨ��
**************************************/
/* ͷ�ļ� */
#include <windows.h>
#include <stdio.h>
/* ȫ�ֱ��� */
HANDLE hSlot;
LPTSTR lpszSlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");
LPTSTR Message = TEXT("Message for mailslot in primary domain."); 

/* ************************************
* void main()
* ����	���̼�mailslotͨ�ſͻ���
**************************************/
void main()
{ 
	DWORD cbMessage, cMessage, cbRead; 
	BOOL fResult; 
	LPTSTR lpszBuffer; 
	TCHAR achID[80]; 
	DWORD cAllMessages; 
	HANDLE hEvent;
	OVERLAPPED ov;

	cbMessage = cMessage = cbRead = 0; 

	hSlot = CreateMailslot(
		lpszSlotName,		// mailslot ��
		0,							// ��������Ϣ��С 
		MAILSLOT_WAIT_FOREVER,         // �޳�ʱ 
		(LPSECURITY_ATTRIBUTES) NULL); 

	if (hSlot == INVALID_HANDLE_VALUE) 
	{ 
		printf("CreateMailslot failed with %d\n", GetLastError());
		return ; 
	} 
	else printf("Mailslot created successfully.\n"); 

	while(1)
	{
		// ��ȡmailslot��Ϣ
		fResult = GetMailslotInfo(hSlot, // mailslot ��� 
			(LPDWORD) NULL,               // �������Ϣ����
			&cbMessage,                   // ��һ����Ϣ�Ĵ�С
			&cMessage,                    // ��Ϣ������
			(LPDWORD) NULL);              // ��ʱ��

		if (!fResult) 
		{ 
			printf("GetMailslotInfo failed with %d.\n", GetLastError()); 
			return ; 
		} 

		if (cbMessage == MAILSLOT_NO_MESSAGE) 
		{ 
			// û����Ϣ����һ��ʱ����ȥ��
			Sleep(20000);
			continue;
		} 

		cAllMessages = cMessage; 

		while (cMessage != 0)  // ��ȡȫ����Ϣ���п��ܲ�ֻһ��
		{ 
			// ��ʾ��Ϣ
			wsprintf((LPTSTR) achID, 
				"\nMessage #%d of %d\n", 
				cAllMessages - cMessage + 1, 
				cAllMessages); 

			// ����ռ�
			lpszBuffer = (LPTSTR) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,
				lstrlen((LPTSTR) achID)*sizeof(TCHAR) + cbMessage); 
			if( NULL == lpszBuffer )
			{
				return ;
			}
			// ��ȡ��Ϣ
			fResult = ReadFile(hSlot,	// mailslot���
				lpszBuffer,			// ����
				cbMessage,			// ��Ϣ�ĳ���
				&cbRead,			// ʵ�ʶ�ȡ�ĳ���
				NULL); 

			if (!fResult) 
			{ 
				printf("ReadFile failed with %d.\n", GetLastError()); 
				GlobalFree((HGLOBAL) lpszBuffer); 
				return ; 
			} 

			// ������Ϣ����ʾ
			lstrcat(lpszBuffer, (LPTSTR) achID); 
			printf("Contents of the mailslot: %s\n", lpszBuffer); 

			HeapFree(GetProcessHeap(),0,lpszBuffer); 
			// ����ʣ�����Ϣ��
			fResult = GetMailslotInfo(hSlot,  
				(LPDWORD) NULL,     
				&cbMessage,              
				&cMessage,                
				(LPDWORD) NULL);           

			if (!fResult) 
			{ 
				printf("GetMailslotInfo failed (%d)\n", GetLastError());
				return ; 
			} 
		} 
	}
	return ; 
}