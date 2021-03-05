/* ************************************
*����ͨWindows API�� 
* ʾ������
* reg.c
* 10.3  ע���
**************************************/
/* ͷ�ļ� */
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
/* Ԥ���� */
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
/* ************************************
* void QueryKey(HKEY hKey) 
* ����	�о�ָ��ע�������Ӽ�
**************************************/
void QueryKey(HKEY hKey) 
{ 
	TCHAR    achKey[MAX_KEY_LENGTH];   
	DWORD    cbName;                 
	TCHAR    achClass[MAX_PATH] = TEXT("");  
	DWORD    cchClassName = MAX_PATH; 
	DWORD    cSubKeys=0;        
	DWORD    cbMaxSubKey;          
	DWORD    cchMaxClass;           
	DWORD    cValues;              
	DWORD    cchMaxValue;        
	DWORD    cbMaxValueData;      
	DWORD    cbSecurityDescriptor; 
	FILETIME ftLastWriteTime;    

	DWORD i, retCode; 

	TCHAR  achValue[MAX_VALUE_NAME]; 
	DWORD cchValue = MAX_VALUE_NAME; 

	// ��ȡ����������
	retCode = RegQueryInfoKey(
		hKey,                    // ���ľ��
		achClass,                //  ���� 
		&cchClassName,           // �������� 
		NULL,                    // ����
		&cSubKeys,               // �Ӽ�������
		&cbMaxSubKey,            // �Ӽ�����
		&cchMaxClass,            // �೤��
		&cValues,                // �Ӽ���ֵ����
		&cchMaxValue,            // �Ӽ�������
		&cbMaxValueData,         // ��ֵ����
		&cbSecurityDescriptor,   // ��ȫ������
		&ftLastWriteTime);       // ���дʱ�� 

	// �о��Ӽ�    
	if (cSubKeys)
	{
		printf( "\nNumber of subkeys: %d\n", cSubKeys);

		for (i=0; i<cSubKeys; i++) 
		{ 
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,
				achKey, 
				&cbName, 
				NULL, 
				NULL, 
				NULL, 
				&ftLastWriteTime); 
			if (retCode == ERROR_SUCCESS) 
			{
				printf(TEXT("(%d) %s\n"), i+1, achKey);
			}
		}
	} 

	// �оټ�ֵ
	if (cValues) 
	{
		printf( "\nNumber of values: %d\n", cValues);

		for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
		{ 
			cchValue = MAX_VALUE_NAME; 
			achValue[0] = '\0'; 
			retCode = RegEnumValue(hKey, i, 
				achValue, 
				&cchValue, 
				NULL, 
				NULL,
				NULL,
				NULL);

			if (retCode == ERROR_SUCCESS ) 
			{ 
				printf(TEXT("(%d) %s\n"), i+1, achValue); 
			} 
		}
	}
}
/* ************************************
* void AddKey(HKEY hKey)
* ����	����һ���Ӽ��������ü�ֵ
**************************************/
void AddKey(HKEY hKey)
{
	HKEY hSubKey;
	DWORD dwKeyValue = 100;
	// ������
	RegCreateKey(hKey,"MySoftware",&hSubKey);
	// ���ü�ֵ
	if( ERROR_SUCCESS != RegSetValueEx(
		hSubKey,
		"TEST",
		0,
		REG_DWORD,
		(LPBYTE)&dwKeyValue,
		sizeof(DWORD)))
	{
		printf("error\n");
	}
}
/* ************************************
* void main(void)
* ����	�򿪼�����ü����
**************************************/
void main(void)
{
	HKEY hTestKey;

	if( RegOpenKeyEx( HKEY_CURRENT_USER,
		TEXT("SOFTWARE"),
		0,
		KEY_READ | KEY_WRITE,
		&hTestKey) == ERROR_SUCCESS
		)
	{
		// ���Ӽ�
		AddKey(hTestKey);
		// �о��Ӽ�
		QueryKey(hTestKey);
	}
}