/* ************************************
*����ͨWindows API��
* ʾ������
* server.c
* 14.1 socketͨ��
**************************************/
/* ͷ�ļ� */
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
/* ���� */
#define DEFAULT_PORT "10000" // �˿�
#define MAX_REQUEST 1024 // �������ݵĻ����С
#define BUF_SIZE 4096 // �������ݵĻ����С

/*************************************
* CommunicationThread
* ����	���ڽ��պͷ������ݵ��߳�
*			Ϊÿһ�����ӵĿͻ��˴���һ�����շ������ݵ��̣߳�
*			����ʹ�ö���ͻ���ͬʱ���ӵ������
* ����	lpParameter��SOKCET
**************************************/
DWORD WINAPI CommunicationThread(
								 LPVOID lpParameter
								 )
{
	DWORD dwTid = GetCurrentThreadId();
	// ��ò���sokcet
	SOCKET socket = (SOCKET)lpParameter;
	// Ϊ�������ݷ���ռ�
	LPSTR szRequest = HeapAlloc(GetProcessHeap(),0, MAX_REQUEST);
	int iResult;
	int bytesSent;// ���ڱ���send�ķ���ֵ��ʵ�ʷ��͵����ݵĴ�С

	// ��������
	iResult = recv(socket, // socket
		szRequest, // ���ջ���
		MAX_REQUEST, // �����С
		0);// ��־
	if (iResult == 0)// ��������ʧ�ܣ������Ѿ��ر�
	{
		printf("Connection closing...\n");
		HeapFree(GetProcessHeap(), 0 ,szRequest);
		closesocket(socket);
		return 1;
	}
	else if (iResult == SOCKET_ERROR)// ��������ʧ�ܣ�socket����
	{
		printf("recv failed: %d\n", WSAGetLastError());
		HeapFree(GetProcessHeap(), 0 ,szRequest);
		closesocket(socket);
		return 1;
	}
	else if (iResult > 0) // �������ݳɹ�
	{
		// ��ʾ���յ�������
		printf("\tCommunicationThread(%d)\tBytes received: %d\n", dwTid, iResult);
		printf("\tCommunicationThread(%d)\trequest string is (%s)\n",dwTid, szRequest);

		// ������յ���������"download file"
		if (lstrcmpi(szRequest, "download file") == 0)
		{
			// ��ȡ�ļ�download.txt������
			HANDLE hFile;
			LPVOID lpReadBuf; // ���ͻ���
			DWORD dwBytesRead;
			DWORD dwFileSize;
			DWORD dwSendFile = 0;
			hFile = CreateFile("download.txt",
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (hFile == INVALID_HANDLE_VALUE)
			{
				printf("\tCommunicationThread\tCould not open file (error %d)\n", 
					GetLastError());
				send(socket, "error", 6, 0);
				closesocket(socket);
				return 1;
			}
			// ���䷢�����ݻ���
			lpReadBuf = HeapAlloc(GetProcessHeap(), 0 , BUF_SIZE);
			// ��ȡ�ļ���С
			dwFileSize = GetFileSize(hFile, NULL);
			// ѭ������
			while(1)
			{
				// ���ļ�������
				if(!ReadFile(hFile, lpReadBuf, BUF_SIZE, &dwBytesRead, NULL))
				{
					printf("\tCommunicationThread\tCould not read from file (error %d)\n", 
						GetLastError());
					closesocket(socket);
					CloseHandle(hFile);
					return 1;
				}
				// ���Ͷ�ȡ���ļ�����
				bytesSent = send(socket, lpReadBuf, dwBytesRead, 0);
				if( bytesSent == SOCKET_ERROR)
				{
					printf("\tCommunicationThread\tsend error %d\n", 
						WSAGetLastError());
					closesocket(socket);
					CloseHandle(hFile);
					return 1;
				}
				// ��ʾ�������ݵĴ�С
				printf("\tCommunicationThread(%d)\tsend %d bytes\n", dwTid,  bytesSent);
				// �ۼӣ��Ѿ����͵Ĵ�С
				dwSendFile += dwBytesRead;
				// ��������ļ����ݶ��Ѿ�����
				if(dwSendFile == dwFileSize)
				{
					printf("\tCommunicationThread\tFile download ok\n");
					break;// �˳�ѭ��
				}
			}
			// �ͷ��ڴ桢�ر����ӣ��ر��ļ�
			HeapFree(GetProcessHeap(), 0 , lpReadBuf);
			CloseHandle(hFile);
			closesocket(socket);
		}
		// ������յ���������"get information"
		else if (lstrcmpi(szRequest, "get information") == 0)
		{
			// ��������
			bytesSent = send(socket, // socket
				"this is information", // ����
				lstrlen("this is information")+1, // ���ݳ���
				0);// ��־
			// �ж��Ƿ�ɹ�
			if( bytesSent == SOCKET_ERROR)
			{
				printf("\tCommunicationThread\tsend error %d\n", 
					WSAGetLastError());
				closesocket(socket);
				return 1;
			}
			printf("\tCommunicationThread(%d)\tsend %d bytes\n",dwTid, bytesSent);
		}
		else// �յ�δ֪����
		{
			printf ("unreferenced request\n");
		}
	}
	// �ͷŽ������ݻ��棬�ر�socket
	HeapFree(GetProcessHeap(), 0 ,szRequest);
	closesocket(socket);
	return 0;
}

/*************************************
* int __cdecl main(void)
* ����	socket�����
**************************************/
int __cdecl main(void)
{
	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET;// ����socket
	SOCKET ClientSocket = INVALID_SOCKET;// ����socket
	struct addrinfo *result = NULL,
		hints;
	int iResult;// ���淵�ؽ��

	// ��ʼ��Winsock����֤Ws2_32.dll�Ѿ�����
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}
	// ��ַ
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// ��ȡ������ַ����֤����Э����õ�
	iResult = getaddrinfo(NULL, // ����
		DEFAULT_PORT, // �˿�
		&hints, // ʹ�õ�����Э�飬�������͵�
		&result);// ���
	if ( iResult != 0 )
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// ����socket�����ڼ���
	ListenSocket = socket(
		result->ai_family, // ����Э�飬AF_INET��IPv4
		result->ai_socktype, // ���ͣ�SOCK_STREAM
		result->ai_protocol);// ͨ��Э�飬TCP
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("socket failed: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	// �󶨵��˿�
	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	printf("bind\n");

	freeaddrinfo(result);// reuslt����ʹ��

	// ��ʼ����
	iResult = listen(ListenSocket, SOMAXCONN);
	printf("start listen......\n");
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	while (1)
	{
		// ���տͻ��˵����ӣ�accept������ȴ���ֱ�����ӽ���
		printf("ready to accept\n");
		ClientSocket = accept(ListenSocket, NULL, NULL);
		// accept�������أ�˵���Ѿ��пͻ�������
		// ��������socket
		printf("accept a connetion\n");
		if (ClientSocket == INVALID_SOCKET)
		{
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			break;// �ȴ����Ӵ����˳�ѭ��
		}
		// Ϊÿһ�����Ӵ���һ�����ݷ��͵Ľ����̣߳�
		// ʹ������ֿ����������������ͻ��˵�����
		if(!CreateThread(
			NULL,
			0,
			CommunicationThread, // �̺߳���
			(LPVOID)ClientSocket, // ��socket��Ϊ����
			0,
			NULL))
		{
			printf("Create Thread error (%d)", GetLastError());
			break;
		}
	}
	// ѭ���˳����ͷ�DLL��
	WSACleanup();
	return 0;
}