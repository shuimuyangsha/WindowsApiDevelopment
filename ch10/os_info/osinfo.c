/* ************************************
*����ͨWindows API�� 
* ʾ������
* osinfo.c
* 10.1  ϵͳ��Ϣ
**************************************/
/* Windows 2k */
#define _WIN32_WINNT  0x0500
/* ͷ�ļ� */
#include <windows.h>
#include <Lmcons.h>
#include <stdio.h>
/* �������� */
void ShowVersionInfo();
void ShowSystemInfo();
void GetFolders();
void GetNames();
void MouseSpeed();
/* ************************************
* int main()
* ����	���ε��ø�ʾ������
**************************************/
int main()
{
	ShowVersionInfo();
	ShowSystemInfo();
	GetFolders();
	GetNames();
	MouseSpeed();
}

/* ************************************
* void ShowVersionInfo()
* ����	��ȡ����ʾϵͳ�汾��Ϣ
**************************************/
void ShowVersionInfo()
{
	OSVERSIONINFOEX ovex;
	CHAR szVersionInfo[1024];
	*szVersionInfo = '\x00';
	// ���ò�����С�����ò��ж��Ƿ�ɹ�
	ovex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if(!GetVersionEx(&ovex))
	{
		printf("error %d\n",GetLastError());
		return;
	}
	// �жϰ汾
	if(ovex.dwMajorVersion==5)
	{
		if(ovex.dwMinorVersion==0)
			lstrcat(szVersionInfo,"Windows 2000 ");
		else if(ovex.dwMinorVersion==1)
			lstrcat(szVersionInfo,"Windows XP ");
		else if(ovex.dwMinorVersion==2)
			lstrcat(szVersionInfo,"Windows Server 2003 ");
	}
	else if(ovex.dwMajorVersion == 6)
		lstrcat(szVersionInfo,"Windows Vista ");
	else
		lstrcat(szVersionInfo,"Windows NT 4.0 �������� ");
	// ��װ��SP���ַ���
	lstrcat(szVersionInfo,ovex.szCSDVersion);
	// �ж�wProductType����Ʒ����
	switch(ovex.wProductType)
	{
	case VER_NT_DOMAIN_CONTROLLER:
		lstrcat(szVersionInfo,"\n�������");
		break;
	case VER_NT_SERVER:
		lstrcat(szVersionInfo,"\n������");
		break;
	case VER_NT_WORKSTATION :
		lstrcat(szVersionInfo,"\n��������վ");
		break;
	}
	// �ж�wSuiteMask
	if(ovex.wSuiteMask & VER_SUITE_PERSONAL)
	{
		lstrcat(szVersionInfo,"\nWindows XP Home Edition");
	}
	if(ovex.wSuiteMask & VER_SUITE_SINGLEUSERTS)
	{
		lstrcat(szVersionInfo,"\n��װ���ն˷��񣬵�ֻ֧��һ���Ự");
	}
	// wSuiteMask��Ա������������ֵ�����
	//VER_SUITE_BLADE
	//VER_SUITE_COMPUTE_SERVER
	//VER_SUITE_DATACENTER
	//VER_SUITE_ENTERPRISE
	//VER_SUITE_EMBEDDEDNT
	//VER_SUITE_PERSONAL
	//VER_SUITE_SINGLEUSERTS
	//VER_SUITE_SMALLBUSINESS
	//VER_SUITE_SMALLBUSINESS_RESTRICTED
	//VER_SUITE_STORAGE_SERVER
	//VER_SUITE_TERMINAL
	printf("%s\n",szVersionInfo);
}
/* ************************************
* void ShowSystemInfo()
* ����	��ȡ����ʾӲ�������Ϣ
**************************************/
void ShowSystemInfo()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	printf("�ڴ��ҳ��С:0x%.8X�������ڴ���ʼ:0x%.8X�������ڴ����:0x%.8X��\n"
		"����������:%d������������:",
		si.dwPageSize,
		si.lpMinimumApplicationAddress,
		si.lpMaximumApplicationAddress,
		si.dwNumberOfProcessors);

	switch (si.dwProcessorType)
	{
	case PROCESSOR_INTEL_386:
		printf("386");
		break;
	case PROCESSOR_INTEL_486:
		printf("486");
		break;
	case PROCESSOR_INTEL_PENTIUM:
		printf("pentium");
		printf(", Cpu Model 0x%.2X, Stepping 0x%.2X",
			(BYTE)(si.wProcessorRevision>>8),
			(BYTE)si.wProcessorRevision);
		break;
	}
	printf("\n�������ܹ�:");
	switch (si.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
		printf("intel");
		printf(" CPU vendor is %d",si.wProcessorLevel);
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		printf("64 bits intel");
		break;
	case PROCESSOR_ARCHITECTURE_AMD64:
		printf("64 bits AMD");
		break;
	case PROCESSOR_ARCHITECTURE_UNKNOWN:
		printf("UNKNOWN");
		break;
	}
	printf("\n");
}
/* ************************************
*void GetFolders()
* ����	��ȡϵͳĿ¼����Ϣ
**************************************/
void GetFolders()
{
	TCHAR szSystemDirectory[MAX_PATH];
	TCHAR szWindowsDirectory[MAX_PATH];

	GetSystemDirectory(szSystemDirectory,MAX_PATH);
	GetWindowsDirectory(szWindowsDirectory,MAX_PATH);
	printf("ϵͳĿ¼��\t%s\nWindowsĿ¼��\t%s\n",
		szSystemDirectory,
		szWindowsDirectory);
}
/* ************************************
* void GetNames()
* ����	��ȡ����������û�������Ϣ
**************************************/
void GetNames()
{
	DWORD dwComputerNameLen = MAX_COMPUTERNAME_LENGTH+1;
	DWORD dwUserNameLen = UNLEN+1;
	TCHAR szComputerName[MAX_COMPUTERNAME_LENGTH+1];
	TCHAR szUserName[UNLEN + 1];
	if(!SetComputerName("My_Computer"))
	{
		printf("Set Error %d",GetLastError());
	}
	GetComputerName(szComputerName,&dwComputerNameLen);

	printf("���������%s\n",szComputerName);

	//ComputerNameNetBIOS 
	//ComputerNameDnsHostname 
	//ComputerNameDnsFullyQualified 
	//ComputerNamePhysicalNetBIOS 
	//ComputerNamePhysicalDnsHostname 
	//ComputerNamePhysicalDnsDomain 
	//ComputerNamePhysicalDnsFullyQualified 
	dwComputerNameLen = MAX_COMPUTERNAME_LENGTH+1;
	GetComputerNameEx(ComputerNameDnsHostname,szComputerName,&dwComputerNameLen);
	printf("ComputerNameDnsHostname: %s\n",szComputerName);

	dwComputerNameLen = MAX_COMPUTERNAME_LENGTH+1;
	GetComputerNameEx(ComputerNamePhysicalNetBIOS,szComputerName,&dwComputerNameLen);
	printf("ComputerNamePhysicalNetBIOS: %s\n",szComputerName);	
	GetUserName(szUserName,&dwUserNameLen);
	printf("�û�����%s\n",szUserName);
}
/* ************************************
* void MouseSpeed()
* ����	��ȡϵͳĿ¼����Ϣ
**************************************/
void MouseSpeed()
{

	BOOL fResult;
	int aMouseInfo[3];       // ����������Ϣ������

	// ���� SystemParametersInfo
	fResult = SystemParametersInfo(
		SPI_GETMOUSE,   // ��ȡ�����Ϣ
		0,              // δʹ��
		&aMouseInfo,    // ���ڱ��������Ϣ
		0);             // δʹ��

	// ������ٶȼӱ� 
	if( fResult )
	{
		aMouseInfo[2] = 2 * aMouseInfo[2]; 

		SystemParametersInfo(
			SPI_SETMOUSE,		// ���������Ϣ
			0,                 // δʹ��
			aMouseInfo,			// �����Ϣ
			SPIF_SENDCHANGE);  // ���� win.ini 
	}
}