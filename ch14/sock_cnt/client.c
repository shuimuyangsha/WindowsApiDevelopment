/* ************************************
*����ͨWindows API��
* ʾ������
* client.c
* 14.1 socketͨ��
**************************************/
/* ͷ�ļ� */
#include <stdio.h>
#include "winsock2.h"
/* ���� */
#define RECV_BUFFER_SIZE 8192

/*************************************
* main
* ���� socketͨ�ſͻ���
**************************************/
void main(int argc, char* argv[])
{
	// ��������
	SOCKADDR_IN clientService;// ��ַ
	SOCKET ConnectSocket;// socket
	WSADATA wsaData;// ��
	LPVOID recvbuf;// ���ջ���
	int bytesSent;
	int bytesRecv = 0;
	char sendbuf[32] = "get information";// Ĭ�Ϸ��͵�����

	// ��ʼ��socket�⣬	����ws2_32.dll�Ѿ�����
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)
		printf("Error at WSAStartup()\n");

	// ����socket
	ConnectSocket = socket(AF_INET, // IPv4
		SOCK_STREAM, // ˳��ġ��ɿ��ġ��������ӵġ�˫���������ͨ��
		IPPROTO_TCP// ʹ��TCPЭ��
		);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	// ���÷���˵�ͨ��Э�顢IP��ַ���˿�
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	clientService.sin_port = htons( 10000 );

	// ���ӵ������
	if ( connect(
		ConnectSocket, // socket
		(SOCKADDR*) &clientService, // ��ַ
		sizeof(clientService) // ��ַ�Ĵ�С
		) == SOCKET_ERROR)
	{
		printf( "Failed to connect(%d)\n",WSAGetLastError() );
		WSACleanup();
		return;
	}
	// ׼����������
	// ������������-d����ô���͵������ǡ�download file��������"get information"
	if(argc ==2 && (!lstrcmp(argv[1], "-d")))
	{
		lstrcpyn(sendbuf, "download file", 32);
	}
	// �����˷�������
	bytesSent = send( ConnectSocket, // socket
		sendbuf,// ���͵����� 
		lstrlen(sendbuf)+1,// ���ݳ���
		0 );// �ޱ�־

	if(bytesSent == SOCKET_ERROR)
	{
		printf( "send error (%d)\n", WSAGetLastError());
		closesocket(ConnectSocket);
		return;
	}
	printf( "Bytes Sent: %ld\n", bytesSent );

	// ׼����������
	recvbuf = HeapAlloc(GetProcessHeap(), 0, RECV_BUFFER_SIZE);
	// ѭ������
	while( bytesRecv != SOCKET_ERROR )
	{
		//Sleep(50);
		bytesRecv = recv( ConnectSocket, // socket
			recvbuf, // �������ݻ���
			RECV_BUFFER_SIZE,// �����С
			0 );// �ޱ�־
		if ( bytesRecv == 0 )
		{
			printf( "Connection Closed.\n");
			break;
		}
		// TODO���������յ����ݣ�����ֻ�򵥵Ľ��յ������ݴ�С��ʾ
		printf( "Bytes Recv: %ld\n", bytesRecv );
	}
	HeapFree(GetProcessHeap(), 0, recvbuf);
	WSACleanup();
	return;
}