/* ************************************
*����ͨWindows API�� 
* ʾ������
* Enumeratevolume.c
* 4.2.1	��������������ȡ����������
**************************************/

#define _WIN32_WINNT 0x0501
/* ͷ�ļ���*/
#include <windows.h>
#include <stdio.h>
/* Ԥ���塡*/
#define BUFSIZE            MAX_PATH
/* ����������*/
BOOL GetDirverInfo(LPSTR szDrive);

/* ************************************
* ����	Ӧ�ó���������������������������
*		GetDirverInfo ��ȡ����������
**************************************/
int main(void)
{
	TCHAR buf[BUFSIZE];      // �����Ϣ
	HANDLE hVol;             // ��������
	BOOL bFlag;

	hVol = FindFirstVolume (buf, BUFSIZE );

	if (hVol == INVALID_HANDLE_VALUE)
	{
		printf (TEXT("No volumes found!\n"));
		return (-1);
	}

	GetDirverInfo (buf);

	while( FindNextVolume(
		hVol,
		buf,
		BUFSIZE
		))
	{
		GetDirverInfo (buf);
	}

	bFlag = FindVolumeClose(
		hVol
		);

	return (bFlag);
}

/* ************************************
 * BOOL GetDirverInfo(LPSTR szDrive)
 * ����	��ȡ������������
 * ����	LPSTR szDrive
 * 	ָ��Ҫ��ȡ���Ե��������ĸ�·�� �� C:\
 * ����ֵ BOOL �Ƿ�ɹ�
 **************************************/
BOOL GetDirverInfo(LPSTR szDrive)
{
	UINT uDriveType;
	DWORD dwVolumeSerialNumber;
	DWORD dwMaximumComponentLength;
	DWORD dwFileSystemFlags;
	CHAR szFileSystemNameBuffer[BUFSIZE];
	CHAR szDirveName[MAX_PATH];
	printf("\n%s\n",szDrive);
	uDriveType = GetDriveType(szDrive);
	switch(uDriveType)
	{
	case DRIVE_UNKNOWN:
		printf("The drive type cannot be determined. ");
		break;
	case DRIVE_NO_ROOT_DIR:
		printf("The root path is invalid, for example, no volume is mounted at the path. ");
		break;
	case DRIVE_REMOVABLE:
		printf("The drive is a type that has removable media, for example, a floppy drive or removable hard disk. ");
		break;
	case DRIVE_FIXED:
		printf("The drive is a type that cannot be removed, for example, a fixed hard drive. ");
		break;
	case DRIVE_REMOTE:
		printf("The drive is a remote (network) drive. ");
		break;
	case DRIVE_CDROM:
		printf("The drive is a CD-ROM drive. ");
		break;
	case DRIVE_RAMDISK:
		printf("The drive is a RAM disk. ");
		break;
	default:
		break;
	}
	if (!GetVolumeInformation(
		szDrive,
		szDirveName,
		MAX_PATH,
		&dwVolumeSerialNumber,
		&dwMaximumComponentLength,
		&dwFileSystemFlags,
		szFileSystemNameBuffer,
		BUFSIZE
		))
	{
		return FALSE;
	}
	if(0!=lstrlen(szDirveName))
	{
		printf ("\nDrive Name is %s\n",szDirveName);
	}

	printf ("\nVolume Serial Number is %u",dwVolumeSerialNumber);
	printf ("\nMaximum Component Length is %u",dwMaximumComponentLength);
	printf ("\nSystem Type is %s\n",szFileSystemNameBuffer);

	if(dwFileSystemFlags & FILE_SUPPORTS_REPARSE_POINTS)
	{
		printf ("The file system does not support volume mount points.\n");
	}
	if(dwFileSystemFlags & FILE_VOLUME_QUOTAS)
	{
		printf ("The file system supports disk quotas.\n");
	}
	if(dwFileSystemFlags & FILE_CASE_SENSITIVE_SEARCH)
	{
		printf ("The file system supports case-sensitive file names.\n");
	}
	//you can use these value to get more informaion
	//
	//FILE_CASE_PRESERVED_NAMES
	//FILE_CASE_SENSITIVE_SEARCH
	//FILE_FILE_COMPRESSION
	//FILE_NAMED_STREAMS
	//FILE_PERSISTENT_ACLS
	//FILE_READ_ONLY_VOLUME
	//FILE_SUPPORTS_ENCRYPTION
	//FILE_SUPPORTS_OBJECT_IDS
	//FILE_SUPPORTS_REPARSE_POINTS
	//FILE_SUPPORTS_SPARSE_FILES
	//FILE_UNICODE_ON_DISK
	//FILE_VOLUME_IS_COMPRESSED
	//FILE_VOLUME_QUOTAS
	printf("...\n");
	return TRUE;
}