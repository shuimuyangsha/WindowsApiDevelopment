/* ************************************
*����ͨWindows API��
* ʾ������
* ioctl.c
* 16.2 IO���ơ��ں�ͨ��
**************************************/
/* ͷ�ļ� */
#include <Windows.h>
#include <Winioctl.h>
#include <stdio.h>
/* �������� */
DWORD EnjectCdrom(LPSTR szCdRomName);
DWORD PrintNTFSInfo(LPSTR szVolumeName);

/*************************************
* main
* ����	-cdrom <�̷�> ��������
*			-ntfs <�̷�> ��ʾnfts��������Ϣ
**************************************/
int main(int argc, char* argv[])
{
	CHAR szName[64];
	if(argc == 3)
	{
		// �����豸��
		wsprintf(szName, "\\\\.\\%s.", argv[2]);
		// ��������
		if(lstrcmp(argv[1],"-cdrom") == 0)
		{
			EnjectCdrom( szName );
			return 0;
		}
		// ��ȡNTFS������ϸ��Ϣ
		if(lstrcmp(argv[1],"-ntfs") == 0)
		{
			PrintNTFSInfo( szName );
			return 0;
		}
	}
	// ʹ�÷���
	printf("usage:  \n\t %s -cdrom <volume>\n\t %s -ntfs <volume>\nlike this: \n\t -cdrom G:", 
		argv[0], argv[0]);

	return 0;
}

/*************************************
* DWORD EnjectCdrom(LPSTR szCdRomName)
* ����	����ָ���Ĺ���
* ����	szCdRomName���豸��
**************************************/
DWORD EnjectCdrom(LPSTR szCdRomName)
{
	HANDLE hDevice;
	DWORD dwBytesReturned;

	hDevice = CreateFile(szCdRomName, // �豸��
		GENERIC_ALL, // ��ȡȨ��
		FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, // ����ʽ
		NULL, // Ĭ�ϰ�ȫ����
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 
		NULL); 
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Could not open file (error %d)\n", GetLastError());
		return 0;
	}
	// ����IOCTL
	if(!DeviceIoControl(
		(HANDLE) hDevice, // �豸���
		IOCTL_STORAGE_EJECT_MEDIA, // ������
		NULL, // ���뻺��
		0, // ���뻺���С
		NULL, // �������
		0, // ��������С
		&dwBytesReturned, // ʵ����Ҫ�������뻺���С
		NULL // ��OVERLAPPED
		))
	{
		printf("DeviceIoControl error (%d)",GetLastError());
		return 0;
	}
	return 1;

}

/*************************************
* DWORD PrintNTFSInfo(LPSTR szVolumeName)
* ����	��ȡ��ʾָ����NTFS��������Ϣ
* ����	szVolumeName���豸��
**************************************/
DWORD PrintNTFSInfo(LPSTR szVolumeName)
{
	// FSCTL_GET_NTFS_VOLUME_DATA IO���Ƶķ���ֵ������
	// NTFS_VOLUME_DATA_BUFFER�ṹ��
	NTFS_VOLUME_DATA_BUFFER nvdb;
	DWORD dwBufferSize;
	HANDLE hDevice;
	// ��ղ���
	ZeroMemory(&nvdb,sizeof(nvdb));

	hDevice = CreateFile(szVolumeName, 
		GENERIC_ALL, 
		FILE_SHARE_READ|FILE_SHARE_WRITE, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL); 
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Could not open file (error %d)\n", GetLastError());
		return 0;
	}

	if(DeviceIoControl(
		hDevice, // �豸���
		FSCTL_GET_NTFS_VOLUME_DATA, // ������
		NULL, // ���뻺��
		0, // ���뻺���С
		&nvdb, // �������
		sizeof( NTFS_VOLUME_DATA_BUFFER ), // ��������С
		&dwBufferSize, // ���ص�ʵ�����ݴ�С
		NULL // ��OVERLAPPED
		))
	{
		// ��ӡ��ȡ����Ϣ
		printf("SerialNumber %lu\n",nvdb.VolumeSerialNumber);
		printf("Starting logical cluster number of the master file table: %lu\n",nvdb.MftStartLcn);
		printf("Length of the master file table: %lu\n",nvdb.MftValidDataLength);
		printf("... ...\n");
	}
	else
	{
		printf("DeviceIoControl error: (%d)\n",GetLastError());
		return 0;
	}
	return 1;
}