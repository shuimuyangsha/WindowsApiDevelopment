/* ************************************
*����ͨWindows API�� 
* ʾ������
* Enum.c
* 6.7	����״̬��Ϣ
* ʹ��PSAPI �� Tool helper��鿴����״̬��Ϣ
**************************************/
/* ͷ�ļ���*/
#include <Windows.h>
#include <Psapi.h>
#include <Tlhelp32.h>
#include <stdio.h>
/* Ԥ����������*/
#pragma comment (lib, "psapi.lib")
/* ����������*/
VOID WINAPI EnumProcess1();
VOID WINAPI EnumProcess2();
VOID ListProcessModules1( DWORD dwPID );
VOID ListProcessModules2( DWORD dwPID);
VOID PrintMemoryInfo( DWORD dwPID );
VOID ShowProcessMemoryInfo( DWORD dwPID );
VOID ListHeapInfo( DWORD dwPID );
VOID ListProcessThreads( DWORD dwPID );
VOID PrintError( LPTSTR msg );

/*************************************
* VOID WINAPI EnumProcess1()
* ����	����EnumProcesses�������̣�
*		������ListProcessModules1������
*		ListProcessThreads�����о�ģ����߳�
*
* �޲������޷���ֵ
**************************************/
VOID WINAPI EnumProcess1()
{
	// ���費����1024������
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;
	// ����EnumProcesses
	if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
		return;
	// ������
	cProcesses = cbNeeded / sizeof(DWORD);
	for ( i = 0; i < cProcesses; i++ )
	{
		// ��ʾ������Ϣ
		printf( "\n\n**************************************************" );
		printf("\nPROCESS : %u\n\n",aProcesses[i]);
		printf( "\n****************************************************" );
		// �о�ģ����Ϣ���߳���Ϣ
		ListProcessModules1( aProcesses[i] );
		ListProcessThreads( aProcesses[i] );
	}
}
/*************************************
* VOID WINAPI EnumProcess2()
* ����	����Process32First��Process32Next�������̣�
*		������ListProcessModules2�����о�ģ�飬
*		����ShowProcessMemoryInfo������ʾ�ڴ�ʹ�����
*
* �޲������޷���ֵ
**************************************/
VOID WINAPI EnumProcess2()
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;
	// Snapshot
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		PrintError( "CreateToolhelp32Snapshot (of processes)" );
		return ;
	}
	// ��������������ṹ�Ĵ�С
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// ��ʼ�оٽ���
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		PrintError( "Process32First" );  // ������Ϣ
		CloseHandle( hProcessSnap );
		return ;
	}
	do
	{
		// ��ӡ������
		printf( "\n\n=====================================================" );
		printf( "\nPROCESS NAME:  %s", pe32.szExeFile );
		printf( "\n-----------------------------------------------------" );

		// ��ȡ���ȼ�
		dwPriorityClass = 0;
		hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
		if( hProcess == NULL )
			PrintError( "OpenProcess" );
		else
		{
			dwPriorityClass = GetPriorityClass( hProcess );
			if( !dwPriorityClass )
				PrintError( "GetPriorityClass" );
			CloseHandle( hProcess );
		}
		// ��ӡ���������Ϣ
		printf( "\n  process ID        = 0x%08X", pe32.th32ProcessID );
		printf( "\n  thread count      = %d",   pe32.cntThreads );
		printf( "\n  parent process ID = 0x%08X", pe32.th32ParentProcessID );
		printf( "\n  Priority Base     = %d", pe32.pcPriClassBase );
		if( dwPriorityClass )
			printf( "\n  Priority Class    = %d", dwPriorityClass );

		// ��ȡģ����Ϣ����ʾ�ڴ�ʹ�����
		ListProcessModules2( pe32.th32ProcessID );
		PrintMemoryInfo(pe32.th32ProcessID);
		ListHeapInfo(pe32.th32ProcessID);

	} while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );	//�رվ��
	return ;
}

/*************************************
* VOID ListProcessModules1( DWORD dwPID )
* ����	����EnumProcessModules����
*		�оٺ���ʾ���̼��ص�ģ��
*
* ����	DWORD dwPID	����PID
**************************************/
VOID ListProcessModules1( DWORD dwPID )
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;	

	printf( "\nListProcessModules1 Process ID %u\n", dwPID );

	// �򿪽��̣���þ��
	hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, dwPID );
	if (NULL == hProcess)
		return;
	// ����EnumProcessModules
	if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
		{
			TCHAR szModName[MAX_PATH];
			// ��ȡ��ȡ��·��
			if ( GetModuleFileNameEx( hProcess, hMods[i], szModName,
				sizeof(szModName)/sizeof(TCHAR)))
			{
				// ��ӡ
				printf( TEXT("\t%s (0x%08X)\n"), szModName, hMods[i] );
			}
		}
	}
	CloseHandle( hProcess );	// �رս��̾��
}

/*************************************
* VOID ListProcessModules2( DWORD dwPID )
* ����	����Module32First��Module32Next����
*		�оٺ���ʾ���̼��ص�ģ��
*
* ����	DWORD dwPID	����PID
**************************************/
VOID ListProcessModules2( DWORD dwPID)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;
	printf( "\nListProcessModules2 Process ID %u\n", dwPID );
	// Snapshot
	hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID );
	if( hModuleSnap == INVALID_HANDLE_VALUE )
	{
		PrintError( "CreateToolhelp32Snapshot (of modules)" );
		return ;
	}
	// ��������������ṹ�Ĵ�С
	me32.dwSize = sizeof( MODULEENTRY32 );
	// ��ʼ��ȡģ����Ϣ
	if( !Module32First( hModuleSnap, &me32 ) )
	{
		PrintError( "Module32First" );  // Show cause of failure
		CloseHandle( hModuleSnap );     // Must clean up the snapshot object!
		return ;
	}
	do
	{
		printf( "\n\n     MODULE NAME:     %s",             me32.szModule );
		printf( "\n     executable     = %s",             me32.szExePath );
		printf( "\n     process ID     = 0x%08X",         me32.th32ProcessID );
		printf( "\n     ref count (g)  =     0x%04X",     me32.GlblcntUsage );
		printf( "\n     ref count (p)  =     0x%04X",     me32.ProccntUsage );
		printf( "\n     base address   = 0x%08X", (DWORD) me32.modBaseAddr );
		printf( "\n     base size      = %d",             me32.modBaseSize );

	} while( Module32Next( hModuleSnap, &me32 ) );
	CloseHandle( hModuleSnap );	// �رվ��
	return ;
}

/*************************************
* VOID PrintMemoryInfo( DWORD dwPID )
* ����	��ʾ���̵��ڴ�ʹ�����
*
* ����	DWORD dwPID	����PID
**************************************/
VOID PrintMemoryInfo( DWORD dwPID )
{
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	printf( "\nProcess ID: %u\n", dwPID );

	hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, dwPID );
	if (NULL == hProcess)
		return;

	if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
	{
		printf( "\tPageFaultCount: 0x%08X\n", pmc.PageFaultCount );
		printf( "\tPeakWorkingSetSize: 0x%08X\n", 
			pmc.PeakWorkingSetSize );
		printf( "\tWorkingSetSize: 0x%08X\n", pmc.WorkingSetSize );
		printf( "\tQuotaPeakPagedPoolUsage: 0x%08X\n", 
			pmc.QuotaPeakPagedPoolUsage );
		printf( "\tQuotaPagedPoolUsage: 0x%08X\n", 
			pmc.QuotaPagedPoolUsage );
		printf( "\tQuotaPeakNonPagedPoolUsage: 0x%08X\n", 
			pmc.QuotaPeakNonPagedPoolUsage );
		printf( "\tQuotaNonPagedPoolUsage: 0x%08X\n", 
			pmc.QuotaNonPagedPoolUsage );
		printf( "\tPagefileUsage: 0x%08X\n", pmc.PagefileUsage ); 
		printf( "\tPeakPagefileUsage: 0x%08X\n", 
			pmc.PeakPagefileUsage );
	}
	CloseHandle( hProcess );
}

/*************************************
* VOID ListHeapInfo( DWORD dwPID )
* ����	��ʾ���̵Ķѷ������
*
* ����	DWORD dwPID	����PID
**************************************/
VOID ListHeapInfo( DWORD dwPID )
{
	HEAPLIST32 hl;
	HEAPENTRY32 he;
	HANDLE hSnapshot = INVALID_HANDLE_VALUE;

	printf( "\\ListHeapInfo Process ID %u\n", dwPID );

	//  Snapshot
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST , dwPID ); 
	if( hSnapshot == INVALID_HANDLE_VALUE ) 
	{ 
		PrintError( "CreateToolhelp32Snapshot (of heaplist)" ); 
		return ; 
	}
	// ��������������ṹ�Ĵ�С
	hl.dwSize = sizeof( HEAPLIST32 ); 

	// ��ʼ��ȡ��Ϣ
	if( !Heap32ListFirst( hSnapshot, &hl ) ) 
	{ 
		PrintError( "Heap32ListFirst" );  
		CloseHandle( hSnapshot );     
		return ; 
	} 
	do 
	{ 
		printf( "\n\tHeap ID     =%u", hl.th32HeapID ); 
		printf( "\tHeap Flags     = %u", hl.dwFlags ); 
		he.dwSize = sizeof(HEAPENTRY32);
		if( !Heap32First(&he,dwPID,hl.th32HeapID) ) 
		{ 
			PrintError( "Heap32First" );	// ����
			CloseHandle( hSnapshot );     
			return ; 
		}
		do 
		{ 
			// ��ʾ��Ϣ
			printf( "\n\t\t Heap Address\t= %u",he.dwAddress ); 
			printf( "\t Heap Size\t= %u",he.dwBlockSize); 
			printf( "\t Heap Flags\t= %u",he.dwFlags);
			printf( "\t Heap Handle\t= %u",he.hHandle);
		} while( Heap32Next(&he )); 

	} while( Heap32ListNext( hSnapshot, &hl ) ); 
	CloseHandle( hSnapshot );	// �رվ��
	return ; 
}

/*************************************
* VOID ListProcessThreads( DWORD dwPID )
* ����	����Thread32First��Thread32Next
*		��ʾһ�����̵��߳�
*
* ����	DWORD dwPID	����PID
**************************************/
VOID ListProcessThreads( DWORD dwPID )
{
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE; 
	THREADENTRY32 te32; 

	printf( "\\ListProcessThreads Process ID %u\n", dwPID );

	// Snapshot
	hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ); 
	if( hThreadSnap == INVALID_HANDLE_VALUE ) 
		return ; 

	// ��������������ṹ�Ĵ�С
	te32.dwSize = sizeof(THREADENTRY32 ); 

	// ��ʼ��ȡ��Ϣ
	if( !Thread32First( hThreadSnap, &te32 ) ) 
	{
		PrintError( "Thread32First" );  // Show cause of failure
		CloseHandle( hThreadSnap );     // Must clean up the snapshot object!
		return ;
	}
	do 
	{ 
		if( te32.th32OwnerProcessID == dwPID )
		{
			// ��ʾ�����Ϣ
			printf( "\n   THREAD ID = 0x%08X", te32.th32ThreadID ); 
			printf( "\t   base priority = %d", te32.tpBasePri ); 
			printf( "\t   delta priority = %d", te32.tpDeltaPri ); 
		}
	} while( Thread32Next(hThreadSnap, &te32 ) ); 

	CloseHandle( hThreadSnap );
	return ;

}

// ��ӡ������Ϣ
VOID PrintError( LPTSTR msg )
{
	DWORD eNum;
	TCHAR sysMsg[256];
	TCHAR* p;

	eNum = GetLastError( );
	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		sysMsg, 256, NULL );
	p = sysMsg;
	while( ( *p > 31 ) || ( *p == 9 ) )
		++p;
	do { *p-- = 0; } while( ( p >= sysMsg ) &&
		( ( *p == '.' ) || ( *p < 33 ) ) );
	printf( "\n  WARNING: %s failed with error %d (%s)", msg, eNum, sysMsg );
}

void main()
{
	printf("EnumProcess1 \n");
	EnumProcess1();
	printf("\n\n\nEnumProcess2 \n");
	EnumProcess2();
}