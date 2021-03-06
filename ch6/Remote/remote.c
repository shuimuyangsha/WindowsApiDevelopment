/* ************************************
*《精通Windows API》 
* 示例代码
* remote.c
* 6.3.3  创建远程线程、将代码注入到其他进程中执行
**************************************/
/* 头文件　*/
#include <windows.h>
#include <Tlhelp32.h>

/*************************************
* BOOL EnablePrivilege (PCSTR name)
* 功能	提升本权限
*
* 参数	PCSTR name	所需的权限
* 返回是否成功
**************************************/
DWORD EnablePrivilege (PCSTR name)
{
	HANDLE hToken;
	BOOL rv;
	//设置结构
	TOKEN_PRIVILEGES priv = { 1, {0, 0, SE_PRIVILEGE_ENABLED} };
	// 查找权限值
	LookupPrivilegeValue (
		0,
		name,
		&priv.Privileges[0].Luid
		);
	// 打开本进程Token
	OpenProcessToken(
		GetCurrentProcess (),
		TOKEN_ADJUST_PRIVILEGES,
		&hToken
		);
	// 提权
	AdjustTokenPrivileges (
		hToken,
		FALSE,
		&priv,
		sizeof priv,
		0,
		0
		);
	// 返回值，错误信息，如果操作成功，则应为ERROR_SUCCESS，为O
	rv = GetLastError();
	// 关闭Token
	CloseHandle (hToken);
	return rv;
}

/*************************************
* BOOL LoadRometeDll(DWORD dwProcessId, LPTSTR lpszLibName)
* 功能	通过创建远程线程给其他进程加载Dll
*
* 参数	DWORD dwProcessId	目标进程PID
*		LPTSTR lpszLibName	Dll的路径
* 返回是否成功
**************************************/
BOOL LoadRometeDll(DWORD dwProcessId, LPTSTR lpszLibName)
{
	BOOL   bResult          = FALSE; 
	HANDLE hProcess         = NULL;
	HANDLE hThread          = NULL;
	PSTR   pszLibFileRemote = NULL;
	DWORD  cch;
	PTHREAD_START_ROUTINE pfnThreadRtn;

	__try 
	{
		// 获得想要注入代码的进程的句柄.
		hProcess = OpenProcess(
			PROCESS_ALL_ACCESS, 
			FALSE, 
			dwProcessId
			);

		if (hProcess == NULL)
			__leave;

		// 计算DLL路径名需要的字节数.
		cch = 1 + lstrlen(lpszLibName);

		// 在远程线程中为路径名分配空间.
		pszLibFileRemote = (PSTR)VirtualAllocEx(
			hProcess, 
			NULL, 
			cch, 
			MEM_COMMIT, 
			PAGE_READWRITE
			);

		if (pszLibFileRemote == NULL) 
			__leave;

		// 将DLL的路径名复制到远程进程的内存空间.
		if (!WriteProcessMemory(
			hProcess, 
			(PVOID)pszLibFileRemote, 
			(PVOID)lpszLibName, 
			cch, 
			NULL)) 
			__leave;

		// 获得LoadLibraryA在Kernel32.dll中的真正地址. 
		pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(
			GetModuleHandle(TEXT("Kernel32")), TEXT("LoadLibraryA"));

		if (pfnThreadRtn == NULL) 
			__leave;

		// 创建远程线程，并通过远程线程调用用户的DLL文件. 
		hThread = CreateRemoteThread(
			hProcess, 
			NULL, 
			0, 
			pfnThreadRtn, 
			(PVOID)pszLibFileRemote, 
			0, 
			NULL
			);
		if (hThread == NULL) 
			__leave;

		// 等待远程线程终止.
		WaitForSingleObject(hThread, INFINITE);
		bResult = TRUE; 
	}
	__finally 
	{ 
		// 关闭句柄. 
		if (pszLibFileRemote != NULL) 
			VirtualFreeEx(hProcess, (PVOID)pszLibFileRemote, 0, MEM_RELEASE);
		if (hThread  != NULL) 
			CloseHandle(hThread);
		if (hProcess != NULL) 
			CloseHandle(hProcess);
	}
	return bResult;
}
/*************************************
* BOOL GetProcessIdByName(LPSTR szProcessName, LPDWORD lpPID)
* 功能	通过进程名获取进程PID
*
* 参数	LPSTR szProcessName	进程名
*		LPDWORD lpPID		指向保存PID的变量
* 返回是否成功
**************************************/
BOOL GetProcessIdByName(LPSTR szProcessName, LPDWORD lpPID)
{
	// 变量及初始化
	STARTUPINFO st;
	PROCESS_INFORMATION pi;
	PROCESSENTRY32 ps;
	HANDLE hSnapshot;
	ZeroMemory(&st, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	st.cb = sizeof(STARTUPINFO);
	ZeroMemory(&ps,sizeof(PROCESSENTRY32));
	ps.dwSize = sizeof(PROCESSENTRY32);
	// 遍历进程
	hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0);
	if(hSnapshot == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if(!Process32First(hSnapshot,&ps))
	{
		return FALSE;
	}
	do
	{
		// 比较进程名
		if(lstrcmpi(ps.szExeFile,"explorer.exe")==0)
		{
			// 找到了
			*lpPID = ps.th32ProcessID;
			CloseHandle(hSnapshot);
			return TRUE;
		}
	}
	while(Process32Next(hSnapshot,&ps));
	// 没有找到
	CloseHandle(hSnapshot);
	return FALSE;
}
/*************************************
* int WinMain(
*			HINSTANCE hInstance,
*			HINSTANCE hPrevInstance,
*			LPSTR lpCmdLine,
*			int nCmdShow
*			)
**************************************/
int WinMain(
			HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR lpCmdLine,
			int nCmdShow
			)
{
	DWORD dwPID;
	// 提权，获取SE_DEBUG_NAME权限，
	// 可以在其他进程的内存空间中写入、创建线程
	if(0!=EnablePrivilege (SE_DEBUG_NAME))
		return 0;
	// 获取目录进程的PID
	if(!GetProcessIdByName("explorer.exe",&dwPID))
		return 0;
	// 通过创建远程线程加载DLL
	// 将msg.dll放置在系统目录下
	if(!LoadRometeDll(dwPID,"msg.dll"))
		return 0;
	return 1;
}