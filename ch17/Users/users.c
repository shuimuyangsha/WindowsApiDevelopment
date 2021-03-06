/* ************************************
*《精通Windows API》
* 示例代码
* users.c
* 17.3 用户、用户组
**************************************/
/* UNICODE */
#ifndef UNICODE
#define UNICODE
#endif
/* 头文件 */
#include <stdio.h>
#include <assert.h>
#include <windows.h> 
#include <lm.h>

/*************************************
* AddUser
* 功能	增加用户
* 参数	szServerName，主机名，如果为本机增加用户，设置为NULL
*			szUserName，用户名
*			szPassword，密码
**************************************/
int AddUser(LPWSTR szServerName, 
			LPWSTR szUserName,
			LPWSTR szPassword)
{
	USER_INFO_1 ui;
	DWORD dwLevel = 1;	// 使用 USER_INFO_1 作为参数
	DWORD dwError = 0;
	NET_API_STATUS nStatus;
	// 填充 USER_INFO_1
	ui.usri1_name = szUserName;	// 用户名
	ui.usri1_password = szPassword;	// 密码
	ui.usri1_priv = USER_PRIV_USER;	// privilege  
	ui.usri1_home_dir = NULL;
	ui.usri1_comment = NULL;
	ui.usri1_flags = UF_SCRIPT;
	ui.usri1_script_path = NULL;
	// 调用 NetUserAdd 增加用户
	nStatus = NetUserAdd(szServerName,
		dwLevel,
		(LPBYTE)&ui,
		&dwError);

	// 判断结果
	if (nStatus == NERR_Success)
	{
		wprintf(stderr, L"User %s has been successfully added on %s\n",
		szUserName, szServerName);
	}
	else
	{
		fprintf(stderr, "A system error has occurred: %d\n", nStatus);
	}
	return 0;
}

/*************************************
* AddUserToGroup
* 功能	为用户组增加用户
* 参数	szServerName，主机名，如果为本机，设置为NULL
*			szUserName，用户名
*			szGroup，用户组名
**************************************/
int AddUserToGroup(LPWSTR szServerName, 
				   LPWSTR szUserName,  
				   LPWSTR szGroup)
{
	NET_API_STATUS nStatus;
	// 调用 NetGroupAddUser
	nStatus =  NetGroupAddUser(
		szServerName,
		szGroup,
		szUserName
		);

	// 判断结果
	if (nStatus == NERR_Success)
		fwprintf(stderr, L"User %s has been successfully added on %s\n",
		szUserName, szServerName);

	else
		fprintf(stderr, "NetGroupAddUser A system error has occurred: %d\n", nStatus);
	return 0;
}

/*************************************
* DelUser
* 功能	删除用户
* 参数	szServerName，主机名，如果为本机，设置为NULL
*			szUserName，用户名
**************************************/
int DelUser(LPWSTR szServerName, LPWSTR szUserName)
{
	DWORD dwError = 0;
	NET_API_STATUS nStatus;

	// 调用 NetUserDel 删除用户
	nStatus = NetUserDel(szServerName, szUserName);
	// 判断并显示结果
	if (nStatus == NERR_Success)
		fwprintf(stderr, L"User %s has been successfully deleted on %s\n",
		szUserName, szServerName);
	else
		fprintf(stderr, "A system error has occurred: %d\n", nStatus);

	return 0;

}

/*************************************
* int ListUsers(LPWSTR pszServerName)
* 功能	列举用户
* 参数	szServerName，主机名，如果为本机，设置为NULL
**************************************/
int ListUsers(LPWSTR pszServerName)
{
	LPUSER_INFO_0 pBuf = NULL;
	LPUSER_INFO_0 pTmpBuf;
	DWORD dwLevel = 0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;

	// 循环，直到可以成功调用 NetUserEnum
	do 
	{
		// 调用NetUserEnum函数
		nStatus = NetUserEnum(pszServerName,
			dwLevel,// 这里设置为0，使用 LPUSER_INFO_0 返回结果
			FILTER_NORMAL_ACCOUNT, // 只列举“正常”类型的用户
			(LPBYTE*)&pBuf,// LPUSER_INFO_0 保存返回结果
			// MAX_PREFERRED_LENGTH，内存由API分配，需要在之后调用NetApiBufferFree释放
			dwPrefMaxLen,
			&dwEntriesRead,// 读了的 Entries
			&dwTotalEntries,// 一共的 Entries
			&dwResumeHandle);
		// 判断是否成功
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{
				// 循环读取用户信息
				for (i = 0; (i < dwEntriesRead); i++)
				{
					assert(pTmpBuf != NULL);

					if (pTmpBuf == NULL)
					{
						fprintf(stderr, "An access violation has occurred\n");
						break;
					}
					// 输出
					wprintf(L"\t-- %s\n", pTmpBuf->usri0_name);
					// 下一个
					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}
		else
			fprintf(stderr, "A system error has occurred: %d\n", nStatus);
		// 释放内存
		if (pBuf != NULL)
		{
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	}
	while (nStatus == ERROR_MORE_DATA); // end do

	// 释放内存
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);

	fprintf(stderr, "Total of %d users\n\n", dwTotalCount);

	return 0;
}

/*************************************
* int ListGroup(LPWSTR pszServerName)
* 功能	列举用户组
* 参数	szServerName，主机名，如果为本机，设置为NULL
**************************************/
int ListGroup(LPWSTR pszServerName)
{

	DWORD dwLevel = 0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;


	LPLOCALGROUP_INFO_0 pBuf = NULL;
	LPLOCALGROUP_INFO_0 pTmpBuf;

	do // begin do
	{
		// 调用NetLocalGroupEnum 参数设置与NetLocalGroup类似
		nStatus = NetLocalGroupEnum(
			pszServerName,
			0,
			(LPBYTE*)&pBuf,
			dwPrefMaxLen,
			&dwEntriesRead,
			&dwTotalEntries,
			&dwResumeHandle);
		// 判断结果
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{
				// 循环输出
				for (i = 0; (i < dwEntriesRead); i++)
				{
					assert(pTmpBuf != NULL);

					if (pTmpBuf == NULL)
					{
						fprintf(stderr, "An access violation has occurred\n");
						break;
					}

					wprintf(L"\t-- %s\n", pTmpBuf->lgrpi0_name);
					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}

		else
			fprintf(stderr, "A system error has occurred: %d\n", nStatus);
		// 释放内存
		if (pBuf != NULL)
		{
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	}

	while (nStatus == ERROR_MORE_DATA); // end do

	if (pBuf != NULL)
		NetApiBufferFree(pBuf);

	fprintf(stderr, "Total of %d groups\n\n", dwTotalCount);

	return 0;
}

/*************************************
* ShowUsersInfo
* 功能	显示指定用户的信息
* 参数	szServerName，主机名，如果为本机，设置为NULL
*			pszUserName，用户名
**************************************/
int ShowUsersInfo(LPWSTR pszServerName,LPWSTR pszUserName)
{

	DWORD dwLevel = 4;// 使用 LPUSER_INFO_4 返回结果
	LPUSER_INFO_4 pBuf = NULL;
	NET_API_STATUS nStatus;

	nStatus = NetUserGetInfo(pszServerName,
		pszUserName,
		dwLevel,	// pBuf参数类型
		(LPBYTE *)&pBuf);
	
	// 判断并输出结果
	if (nStatus == NERR_Success)
	{
		if (pBuf != NULL)
		{
			wprintf(L"\n\tAccount:      %s\n", pBuf->usri4_name);
			wprintf(L"\tComment:      %s\n", pBuf->usri4_comment);
			wprintf(L"\tUser comment: %s\n", pBuf->usri4_usr_comment);
			wprintf(L"\tFull name:    %s\n", pBuf->usri4_full_name);
			wprintf(L"\tpriv:    %d\n", pBuf->usri4_priv);
		}
	}

	else
		fprintf(stderr, "A system error has occurred: %d\n", nStatus);
	// 释放内存
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);
	return 0;

}

/*************************************
* wmain
* 功能	入口函数，根据参数判断需要调用的功能函数
* 参数	参见usage输出
**************************************/
int __cdecl wmain(int ac, wchar_t * av[])
{

	if (ac == 4 && lstrcmpW( av[1], L"-a") == 0)
	{
		AddUser(NULL, av[2], av[3]);
	}

	else if (ac == 4 && lstrcmpW( av[1], L"-g") == 0)
	{
		AddUserToGroup(NULL, av[2], av[3]);
	}
	else 	if (ac == 3 && lstrcmpW( av[1], L"-i") == 0)
	{
		ShowUsersInfo(NULL, av[2]);
	}
	else 	if (ac == 2 && lstrcmpW( av[1], L"-i") == 0)
	{
		ListUsers(NULL);
		ListGroup(NULL);
	}
	else	if (ac == 3 && lstrcmpW( av[1], L"-d") == 0)
	{
		DelUser(NULL, av[2]);
	}
	else
	{
		printf("usage: \n"
			"\t %ws -a <username> <password> to add a user\n"
			"\t %ws -g <username> <group> add a user to a group"
			"\t %ws -i <username> to show user info\n"
			"\t %ws -d <username> to del a user\n", 
			av[0], av[0], av[0], av[0]);
	}
	return 0;
}