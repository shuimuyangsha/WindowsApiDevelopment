/* ************************************
*����ͨWindows API��
* ʾ������
* EnumDevice.c
* 16.1 �豸����
**************************************/
/* ͷ�ļ� */
#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include <devguid.h>
#include <regstr.h>
/* �������� */
BOOL EnumPresentDevice( const GUID * InterfaceClassGuid );
BOOL EnumAllDevice();
/*************************************
* BOOL EnumClassDevice( const GUID * InterfaceClassGuid )
* ����	���������оٵ�ǰ���ڵ��豸
* ����	InterfaceClassGuid�������о��豸�ӿ����GUID
**************************************/
BOOL EnumClassDevice( const GUID * InterfaceClassGuid )

{
	HDEVINFO DeviceInfoSet;
	HDEVINFO NewDeviceInfoSet;

	SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
	PSP_DEVICE_INTERFACE_DETAIL_DATA lpDeviceInterfaceDetailData;

	DWORD dwBufferSize = 0;
	DWORD i;
	// �������豸��Ϣ�б�
	DeviceInfoSet = SetupDiCreateDeviceInfoList(NULL, NULL);

	if(DeviceInfoSet == INVALID_HANDLE_VALUE) 
	{
		printf("CreateDeviceInfoList failed: %d\n", GetLastError());
		return 0;
	}

	// ���ݽӿ����ͻ���µ��豸��Ϣ�б�

	NewDeviceInfoSet = SetupDiGetClassDevsEx(
		InterfaceClassGuid,
		NULL,
		NULL,
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE,
		DeviceInfoSet,// ֮ǰ�������豸��Ϣ�б�
		NULL,
		NULL
		);
	if(NewDeviceInfoSet == INVALID_HANDLE_VALUE)
	{
		printf( "SetupDiGetClassDevsEx failed: %d\n", GetLastError() );
		return 0;
	}
	// ���� SP_DEVICE_INTERFACE_DATA ��С
	DeviceInterfaceData.cbSize 
		= sizeof(SP_DEVICE_INTERFACE_DATA);

	for (i=0; ;i++)
	{
		// �оٽӿ���Ϣ
		BOOL bResult = SetupDiEnumDeviceInterfaces(
			NewDeviceInfoSet,
			NULL,
			InterfaceClassGuid,
			i,
			&DeviceInterfaceData
			);
		if(!bResult)
		{
			if ( GetLastError()!=NO_ERROR &&
				GetLastError()!=ERROR_NO_MORE_ITEMS )
			{
				printf("ERROR: (%d)",GetLastError());
				return FALSE;
			}
			break;
		}
		else
		{
			// ΪPSP_DEVICE_INTERFACE_DETAIL_DATA�ṹ�����ڴ棬���
			lpDeviceInterfaceDetailData = HeapAlloc(
				GetProcessHeap(), 0,
				sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA));
			lpDeviceInterfaceDetailData->cbSize 
				= sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			dwBufferSize = lpDeviceInterfaceDetailData->cbSize;
			// ��ýӿ���ϸ��Ϣ
			while(!SetupDiGetDeviceInterfaceDetail(
				NewDeviceInfoSet,
				&DeviceInterfaceData,
				lpDeviceInterfaceDetailData,
				dwBufferSize,
				&dwBufferSize,
				NULL))
			{
				// ����ڴ�ռ䲻�㣬�ٴη��䣬ֱ�����Գɹ�����
				if(ERROR_INSUFFICIENT_BUFFER==GetLastError())
				{
					lpDeviceInterfaceDetailData = HeapReAlloc(
						GetProcessHeap(), 0, 
						lpDeviceInterfaceDetailData, dwBufferSize);
					lpDeviceInterfaceDetailData->cbSize 
						= sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
				}               
			}
			// ��ʾ��Ϣ
			printf("DevicePath: %s\n",lpDeviceInterfaceDetailData->DevicePath);
			// lpDeviceInterfaceDetailData->DevicePath����ΪCreateFile�Ĳ���������IO����
			
			// �ͷ��ڴ�
			HeapFree(GetProcessHeap(),0,lpDeviceInterfaceDetailData);
		}
	}
	SetupDiDestroyDeviceInfoList(DeviceInfoSet);
	return TRUE;
}
/*************************************
* BOOL EnumAllDevice( )
* ����	�оٵ�ǰ���ڵ��豸
* ����ֵ	�Ƿ�ɹ�
**************************************/
BOOL EnumAllDevice()
{
	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD i;

	printf("Displaying the Installed Devices\n\n");

	// �õ������豸 HDEVINFO 
	hDevInfo = SetupDiGetClassDevs(NULL,
		0, // ������
		0, // �޻ص�����
		DIGCF_PRESENT | DIGCF_ALLCLASSES );
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	// ѭ���о�
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,
		&DeviceInfoData);i++)
	{
		DWORD DataT;
		LPTSTR buffer = NULL;
		DWORD buffersize = 0;

		// ��ȡ��ϸ��Ϣ
		while (!SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DeviceInfoData,
			SPDRP_DEVICEDESC,
			&DataT,
			(PBYTE)buffer,
			buffersize,
			&buffersize))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				// �ڴ治��
				if (buffer) HeapFree(GetProcessHeap(), 0, buffer);
				buffer = (LPTSTR)HeapAlloc(GetProcessHeap(), 0,  buffersize);
			}
			else
				break;
		}
// ���
		printf("GUID:{%.8X-%.4X-%.4X--%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X} "
			"Device: %s\n",
			DeviceInfoData.ClassGuid.Data1,
			DeviceInfoData.ClassGuid.Data2,
			DeviceInfoData.ClassGuid.Data3,
			DeviceInfoData.ClassGuid.Data4[0],
			DeviceInfoData.ClassGuid.Data4[1],
			DeviceInfoData.ClassGuid.Data4[2],
			DeviceInfoData.ClassGuid.Data4[3],
			DeviceInfoData.ClassGuid.Data4[4],
			DeviceInfoData.ClassGuid.Data4[5],
			DeviceInfoData.ClassGuid.Data4[6],
			DeviceInfoData.ClassGuid.Data4[7],buffer);

		if (buffer) HeapFree(GetProcessHeap(), 0, buffer);
	}

	if ( GetLastError()!=NO_ERROR &&
		GetLastError()!=ERROR_NO_MORE_ITEMS )
	{
		return FALSE;
	}
	//  �ͷ�
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return TRUE;
}

int main( int argc, char *argv[ ], char *envp[ ] )
{
	// �о������豸
	printf("Enumerating All Device\n\n");
	EnumAllDevice();
	// �оٴ��̷־��������豸
	printf("\n\nEnumerating Present Volume \n\n");
	EnumClassDevice(&GUID_DEVINTERFACE_VOLUME);
	return 0;
}