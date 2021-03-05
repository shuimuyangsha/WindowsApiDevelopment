/* ************************************
*����ͨWindows API��
* ʾ������
* iph.cpp
* 14.2 Ip Helper
**************************************/
/* ͷ�ļ� */
#include <stdio.h>
#include <windows.h>
#include "iphlpapi.h"
/* ȫ�ֱ��� */
ULONG ulOutBufLen;
DWORD dwRetVal;
HANDLE hHeap = GetProcessHeap();
/* �궨��*/
#define MyAlloc(size) HeapAlloc(hHeap, 0, size)
#define MyFree(ptr) HeapFree(hHeap, 0, ptr)

/*************************************
* main
* ���� ��ʾIP Helper������ʹ�÷���
**************************************/
void main(int ac, char* av[])
{
	// GetNetworkParams
	printf("------------------------\n");
	printf("GetNetworkParams\n\n");
	// ����ı���
	FIXED_INFO *pFixedInfo;
	IP_ADDR_STRING *pIPAddr;
	// �����ڴ�
	pFixedInfo = (FIXED_INFO *) MyAlloc( sizeof( FIXED_INFO ) );
	ulOutBufLen = sizeof( FIXED_INFO );
	
	if( GetNetworkParams( pFixedInfo, &ulOutBufLen ) == ERROR_BUFFER_OVERFLOW )
	{
		// �ռ䲻�㣬���·���
		MyFree( pFixedInfo );
		pFixedInfo = (FIXED_INFO * ) MyAlloc ( ulOutBufLen );
	}
	// ��ȡ��Ϣ
	if ( dwRetVal = GetNetworkParams( pFixedInfo, &ulOutBufLen ) != NO_ERROR )
	{
		printf("Call to GetNetworkParams failed.\n");
	}
	else
	{
		// ��ӡ���
		printf("\tHost Name: %s\n", pFixedInfo -> HostName);// ������
		printf("\tDomain Name: %s\n", pFixedInfo -> DomainName);// DNS������
		printf("\tDNS Servers:\n");//DNS ������
		printf("\t\t%s\n", pFixedInfo -> DnsServerList.IpAddress.String);// IP���ַ�����ʽ��

		pIPAddr = pFixedInfo -> DnsServerList.Next;// ����DNS
		while ( pIPAddr )
		{
			printf("\t\t%s\n", pIPAddr -> IpAddress.String);
			pIPAddr = pIPAddr -> Next;
		}

		if (pFixedInfo -> EnableRouting)// �Ƿ�·��
			printf("\tEnable Routing: Yes\n");
		else
			printf("\tEnable Routing: No\n");

		if (pFixedInfo -> EnableProxy)// �Ƿ��д���
			printf("\tEnable Proxy: Yes\n");
		else
			printf("\tEnable Proxy: No\n");

		if (pFixedInfo -> EnableDns)// �Ƿ���DNS
			printf("\tEnable DNS: Yes\n");
		else
			printf("\tEnable DNS: No\n");
	}
	MyFree(pFixedInfo);// �ͷ��ڴ�

	// GetNetworkParams
	printf("------------------------\n");
	printf("GetInterfaceInfo\n\n");
	// �������塢�����ڴ�ռ�
	PIP_INTERFACE_INFO pInfo;
	ulOutBufLen = sizeof(IP_INTERFACE_INFO);
	pInfo = (PIP_INTERFACE_INFO)MyAlloc( ulOutBufLen );

	while(1)
	{
		dwRetVal = GetInterfaceInfo(pInfo, &ulOutBufLen);
		// ���ڲ��㣬���·���
		if(dwRetVal == ERROR_INSUFFICIENT_BUFFER)
		{
			MyFree( pInfo );
			pInfo = (IP_INTERFACE_INFO *)MyAlloc( ulOutBufLen );
		}
		else if (dwRetVal == NO_ERROR)
		{
			// ��ʾ���ÿ����������Ϣ
			int i;
			printf("Num Adapters: %ld\n", pInfo->NumAdapters);
			for (i=0; i<pInfo->NumAdapters; i++)
			{
				printf("(%d)Adapter Name: %ws\n", i, pInfo->Adapter[i].Name);
				printf("(%d)Adapter Index: %ld\n", i, pInfo->Adapter[i].Index);
			}
			break;
		}
		else
			printf("GetInterfaceInfo: %u\n", GetLastError());
	}
	MyFree( pInfo );// �ͷ��ڴ�

	// GetIpAddrTable
	printf("----------------------\n");
	printf("GetIpAddrTable\n\n");
	
	// �������塢�ڴ����
	MIB_IPADDRTABLE *pIPAddrTable;
	DWORD dwSize;
	in_addr IPAddr;
	char *strIPAddr;

	pIPAddrTable = (MIB_IPADDRTABLE*) MyAlloc( sizeof( MIB_IPADDRTABLE) );
	dwSize = 0;
	IPAddr.S_un.S_addr = ntohl(pIPAddrTable->table[1].dwAddr);
	strIPAddr = inet_ntoa(IPAddr);

	if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
	{
		// �ռ䲻�㣬���·���
		MyFree( pIPAddrTable );
		pIPAddrTable = (MIB_IPADDRTABLE *) MyAlloc ( dwSize );
	}
	// �����Ϣ����ʾ
	if ( (dwRetVal = GetIpAddrTable( pIPAddrTable, &dwSize, 0 )) != NO_ERROR )
	{
		printf("Call to GetIpAddrTable failed.\n");
	}
	// IP�����������DWORD����ʽ�����
	printf("Address: %u.%u.%u.%u\n",
		pIPAddrTable->table[0].dwAddr & 0x000000FF,
		(pIPAddrTable->table[0].dwAddr & 0x0000FF00)>>8,
		(pIPAddrTable->table[0].dwAddr & 0x00FF0000)>>16,
		(pIPAddrTable->table[0].dwAddr & 0xFF000000)>>24);
	printf("Mask: %lu\n", pIPAddrTable->table[0].dwMask);
	printf("Index: %lu\n", pIPAddrTable->table[0].dwIndex);
	printf("BCast: %lu\n", pIPAddrTable->table[0].dwBCastAddr);
	printf("Reasm: %lu\n", pIPAddrTable->table[0].dwReasmSize);

	MyFree( pIPAddrTable );// �ͷ��ڴ�

	// GetIPStatistics
	printf("-------------------------\n");
	printf("GetIPStatistics\n\n");
	// ��������������ڴ�
	MIB_IPSTATS *pStats;
	pStats = (MIB_IPSTATS*) MyAlloc(sizeof(MIB_IPSTATS));
	
	// ��ȡ��Ϣ
	if ((dwRetVal = GetIpStatistics(pStats)) != NO_ERROR)
	{
		printf("\tError getting stats.\n");
	}
	// ��ʾ
	printf("\tNumber of IP addresses: %ld\n", pStats->dwNumAddr);
	printf("\tNumber of Interfaces: %ld\n", pStats->dwNumIf);
	printf("\tReceives: %ld\n", pStats->dwInReceives);
	printf("\tOut Requests: %ld\n", pStats->dwOutRequests);
	printf("\tRoutes: %ld\n", pStats->dwNumRoutes);
	printf("\tTimeout Time: %ld\n", pStats->dwReasmTimeout);
	printf("\tIn Delivers: %ld\n", pStats->dwInDelivers);
	printf("\tIn Discards: %ld\n", pStats->dwInDiscards);
	printf("\tTotal In: %ld\n", pStats->dwInDelivers + pStats->dwInDiscards);
	printf("\tIn Header Errors: %ld\n", pStats->dwInHdrErrors);
	MyFree(pStats);// �ͷ�
	
	// GetTCPStatistics
	printf("-------------------------\n");
	printf("GetTCPStatistics\n\n");
	// �������������ռ�
	MIB_TCPSTATS *pTCPStats;
	pTCPStats = (MIB_TCPSTATS*) MyAlloc (sizeof(MIB_TCPSTATS));
	
	// ��ȡ��Ϣ������ʾ
	if ((dwRetVal = GetTcpStatistics(pTCPStats)) != NO_ERROR)
		printf("Error getting TCP Stats.\n");
	printf("\tActive Opens: %ld\n", pTCPStats->dwActiveOpens);
	printf("\tPassive Opens: %ld\n", pTCPStats->dwPassiveOpens);
	printf("\tSegments Recv: %ld\n", pTCPStats->dwInSegs);
	printf("\tSegments Xmit: %ld\n", pTCPStats->dwOutSegs);
	printf("\tTotal # Conxs: %ld\n", pTCPStats->dwNumConns);
	MyFree(pTCPStats);// �ͷ��ڴ�

	// GetAdapatersInfo
	printf("------------------------\n");
	printf("GetAdapatersInfo\n\n");
	// ��������������ڴ�
	IP_ADAPTER_INFO *pAdapterInfo;
	IP_ADAPTER_INFO *pAdapter;
	pAdapterInfo = (IP_ADAPTER_INFO *) MyAlloc( sizeof(IP_ADAPTER_INFO) );
	ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	
	// Test�������ڴ��С
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)
	{
		MyFree (pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) MyAlloc (ulOutBufLen);
	}
	// �����Ϣ
	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) != NO_ERROR)
	{
		printf("Call to GetAdaptersInfo failed.\n");
	}
	pAdapter = pAdapterInfo;
	// ��ʾ������������Ϣ
	while (pAdapter)
	{
		printf("\n\t***********\n");
		printf("\tAdapter Name: \t%s\n", pAdapter->AdapterName);// �����豸��
		printf("\tAdapter Desc: \t%s\n", pAdapter->Description);// ����
		printf("\tAdapter Addr: \t%ld\n", pAdapter->Address);// MAC
		printf("\tIP Address: \t%s\n", pAdapter->IpAddressList.IpAddress.String);// IP
		printf("\tIP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);// ����

		printf("\tGateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);// ����
		printf("\t***\n");
		if (pAdapter->DhcpEnabled)// DHCP
		{
			printf("\tDHCP Enabled: Yes\n");
			printf("\t\tDHCP Server: \t%s\n", pAdapter->DhcpServer.IpAddress.String);
			printf("\tLease Obtained: %ld\n", pAdapter->LeaseObtained);
		}
		else
			printf("\tDHCP Enabled: No\n");
		if (pAdapter->HaveWins)// Wins
		{
			printf("\tHave Wins: Yes\n");
			printf("\t\tPrimary Wins Server: \t%s\n", pAdapter->PrimaryWinsServer.IpAddress.String);
			printf("\t\tSecondary Wins Server: \t%s\n", pAdapter->SecondaryWinsServer.IpAddress.String);
		}
		else
			printf("\tHave Wins: No\n");
		pAdapter = pAdapter->Next;
	}
	MyFree(pAdapterInfo);// �ͷ�
}