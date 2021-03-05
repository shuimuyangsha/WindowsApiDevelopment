/* ************************************
*����ͨWindows API�� 
* ʾ������
* ConsoleIO.c
* 9.2  �ַ��������
**************************************/
/* Ԥ�������� */
#define _WIN32_WINNT 0x0501 

#include <windows.h>
#include <stdio.h>
#include <conio.h>
/* �ⲿ���� */
extern CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
extern HANDLE hStdout, hStdin; 
/* �������� */
VOID MyErrorExit(LPSTR szErrorText);
void NewLine(void) ;

/* ************************************
* void ScrollScreenBuffer(HANDLE h, INT x)
* ����	������Ļ
* ����	HANDLE h, ����̨�����		INT x�� ��
**************************************/
void ScrollScreenBuffer(HANDLE h, INT x)
{
	SMALL_RECT srctScrollRect, srctClipRect;
	CHAR_INFO chiFill;
	COORD coordDest;

	srctScrollRect.Left = 0;
	srctScrollRect.Top = 1;
	srctScrollRect.Right = csbiInfo.dwSize.X - x; 
	srctScrollRect.Bottom = csbiInfo.dwSize.Y - x; 

	// Ŀ��
	coordDest.X = 0; 
	coordDest.Y = 0; 

	// left unchanged. 
	srctClipRect = srctScrollRect; 

	// ���������ַ��������� 
	chiFill.Attributes = FOREGROUND_RED|FOREGROUND_INTENSITY; 
	chiFill.Char.AsciiChar = (char)' '; 

	// ����һ��
	ScrollConsoleScreenBuffer( 
		h,               // ��Ļ��������� 
		&srctScrollRect, // scrolling rectangle 
		&srctClipRect,   // clipping rectangle 
		coordDest,       // ���Ͻ�Ŀ��cell 
		&chiFill);       // �����ַ�����ɫ
}

/* ************************************
* VOID GetConsoleInfo(HANDLE hOutput)
* ����	��ȡ����̨��Ϣ
* ����	HANDLE hOutput������̨���
**************************************/
VOID GetConsoleInfo(HANDLE hOutput)
{
	CONSOLE_FONT_INFO cfi;
	DWORD dwProcessList[32];
	DWORD dwAttachedProcess;
	TCHAR szOutputBuffer[1024];
	HWND hConsoleWindows;
	DWORD dwWritten;
	DWORD i;
	CHAR szConsoleTitle[MAX_PATH];
	// �����Ľ��̣������Ƕ���32�������
	dwAttachedProcess = GetConsoleProcessList(dwProcessList,32);

	if(dwAttachedProcess==0)
	{
		MyErrorExit("GetConsoleProcessList");
	}
	// ����
	if( !GetConsoleTitle(szConsoleTitle, MAX_PATH) )
	{
		MyErrorExit("GetConsoleTitle");
	}
	// ���ھ��
	hConsoleWindows = GetConsoleWindow();
	// ����
	GetCurrentConsoleFont(hOutput,FALSE,&cfi);

	wsprintf(szOutputBuffer,"Now %u attached Processes: ",dwAttachedProcess);
	for(i=0;i<dwAttachedProcess;i++)
	{
		wsprintf(szOutputBuffer+lstrlen(szOutputBuffer),"%u, ",dwProcessList[i]);
	}
	// �����ַ���
	wsprintf(szOutputBuffer+lstrlen(szOutputBuffer),
		"\nConsoleTitle is %s,\nWindow Handle is 0x%.8X\n"
		"Font is %u, Font Size X = %u, Y = %u\n",
		szConsoleTitle,hConsoleWindows,
		cfi.nFont,cfi.dwFontSize.X,cfi.dwFontSize.Y);
	// ��ʾ��ȡ����Ϣ
	if(!WriteConsole(hOutput,szOutputBuffer,lstrlen(szOutputBuffer),&dwWritten,NULL))
	{
		MyErrorExit("WriteConsole");
	}
}
/* ************************************
* VOID ChangeBackgroudColor(HANDLE hConsole)
* ����	�ı䱳����ɫ
* ����	HANDLE hOutput������̨���
**************************************/
VOID ChangeBackgroudColor(HANDLE hConsole)
{
	COORD coord;
	WORD wColor;
	DWORD cWritten;
	BOOL fSuccess;
	coord.X = 0; // ��һ��Cell
	coord.Y = 0; // ��һ��
	// ������ǰ��ɫ
	wColor = BACKGROUND_BLUE | BACKGROUND_RED |
		FOREGROUND_RED |
		FOREGROUND_GREEN |
		FOREGROUND_BLUE;
	// ���ÿ���̨����
	fSuccess = FillConsoleOutputAttribute(
		hConsole,		// Handle
		wColor,			// ��ɫ
		80*50,			// ����Cell����
		coord,			// �ı����Եĵ�һ��Cell
		&cWritten);	// ʵ�ʸı�����

	if (! fSuccess)
		MyErrorExit("FillConsoleOutputAttribute");
}

/* ************************************
* VOID ChangeUpCaseTextBackgroudColor(HANDLE hStdin, HANDLE hStdout)
* ����	�ı����ֱ�����ɫ
* ����	HANDLE hStdin, HANDLE hStdout������̨����������
**************************************/
VOID ChangeUpCaseTextBackgroudColor(HANDLE hStdin, HANDLE hStdout)
{
	DWORD dwLen, i, dwRead, dwWritten;
	WORD wColors[3];
	BOOL fSuccess;
	COORD coord;
	HANDLE hSreenHandle;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	PCHAR lpCharacter = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,2048);
	coord.X = 0;	// ��ʼCell
	coord.Y = 0;
	// ��ȡOutputCharacter
	if(!ReadConsoleOutputCharacter(hStdout,lpCharacter,2047,coord,&dwRead))
	{
		MyErrorExit("ReadConsoleOutputCharacter");
	}
	// ��ȡScreenBufferInfo
	GetConsoleScreenBufferInfo(hStdout,&csbi);

	dwLen = lstrlen(lpCharacter);
	// ��ɫ����
	wColors[0] = BACKGROUND_RED;
	wColors[1] = BACKGROUND_RED | BACKGROUND_GREEN;
	wColors[2] = BACKGROUND_GREEN;

	for (i=0 ;i<dwLen; i++)
	{
		// ���Ҵ�д��ĸ
		if(lpCharacter[i]>='A'&& lpCharacter[i]<='Z')
		{
			// ����Ǵ�д��ĸ����Ӵ�д��ĸ��ʼλ�õ�����Cell��
			// ���ֱ����ı�����ֲ�ͬ����ɫ
			coord.Y = i/csbi.dwSize.X;
			coord.X = i%csbi.dwSize.X;
			fSuccess = WriteConsoleOutputAttribute(
				hStdout, // handle
				wColors, 3, 
				coord, // ��ʼλ��
				&dwWritten);
			if(!fSuccess)
			{
				MyErrorExit("WriteConsoleOutputAttribute");
			}
		}
	}
}

/* ************************************
* VOID ChangeTextColor(HANDLE hStdout,WORD wColor)
* ����	�ı�������ɫ
* ����	HANDLE hStdout,����� WORD wColor���µ���ɫ
**************************************/
VOID ChangeTextColor(HANDLE hStdout, WORD wColor)
{
	// ������������
	if (! SetConsoleTextAttribute(hStdout, wColor))
	{
		MessageBox(NULL, "SetConsoleTextAttribute", "Console Error",
			MB_OK);
		return;
	}
}

/* ************************************
* VOID ChangeConsoleTitle(LPSTR szNewTitle)
* ����	�ı����̨���ڵı���
* ����	LPSTR szNewTitle���±���
**************************************/
VOID ChangeConsoleTitle(LPSTR szNewTitle)
{

	if( !SetConsoleTitle(szNewTitle) )
		MyErrorExit("SetConsoleTitle failed\n");
}

/* ************************************
* void cls( HANDLE hConsole )
* ����	����
* ����	HANDLE hConsole�����
**************************************/
void cls( HANDLE hConsole )
{
	COORD coordScreen = { 0, 0 }; // ���ָ��λ��
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	// ��ǰBuffer�е�����Cell��Ŀ
	if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
		return;
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// ��������Ļ���
	if( !FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
		dwConSize, coordScreen, &cCharsWritten ))
		return;

	// ��ȡ��������
	if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
		return;

	// ����buffer����
	if( !FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
		dwConSize, coordScreen, &cCharsWritten ))
		return;

	// �����ָ���ƶ���ʼλ��
	SetConsoleCursorPosition( hConsole, coordScreen );
}

/* ************************************
* VOID ChangeMode( HANDLE hStdin, HANDLE hStdout)
* ����	�ı����̨��ģʽ���ر�ENABLE_LINE_INPUT��ENABLE_ECHO_INPUT
* ����	HANDLE hStdin, HANDLE hStdout�����
**************************************/
VOID ChangeMode( HANDLE hStdin, HANDLE hStdout)
{
		LPSTR lpszPrompt = "Mode changeed Type any key, or q to quit: ";
		CHAR chBuffer[256];

	DWORD fdwMode, fdwOldMode;
	DWORD cRead,cWritten;
	// ��ȡ��ǰģʽ
	if (! GetConsoleMode(hStdin, &fdwOldMode)) 
	{
		MessageBox(NULL, "GetConsoleMode", "Console Error", MB_OK); 
		return;
	}
	// �޸�ģʽ����������
	fdwMode = fdwOldMode & 
		~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT); 
	if (! SetConsoleMode(hStdin, fdwMode)) 
	{
		MessageBox(NULL, "SetConsoleMode", "Console Error", MB_OK); 
		return;
	}
	// ����
	NewLine();
	// ѭ���ȴ����벢����
	while (1) 
	{ 
		if (! WriteFile( hStdout, 
			lpszPrompt,  lstrlen(lpszPrompt), 
			&cWritten, NULL) )
		{
			MyErrorExit("WriteFile");
			return;
		}

		if (! ReadFile(hStdin, chBuffer, 1, &cRead, NULL)) 
			break; 
		if (chBuffer[0] == '\r')
			NewLine();
		else if (! WriteFile(hStdout, chBuffer, cRead, 
			&cWritten, NULL)) break;
		else
			NewLine();
		if (chBuffer[0] == 'q') break; // ����q���˳� 
	} 
	// �ָ�ģʽ
	SetConsoleMode(hStdin, fdwOldMode);
}

/* ************************************
* void NewLine(void) 
* ����	�µ�һ�У�������Ļ
**************************************/
void NewLine(void) 
{ 
	if (! GetConsoleScreenBufferInfo(hStdout, &csbiInfo)) 
	{
		MessageBox(NULL, "GetConsoleScreenBufferInfo", 
			"Console Error", MB_OK); 
		return;
	}

	csbiInfo.dwCursorPosition.X = 0; 

	if ((csbiInfo.dwSize.Y-1) == csbiInfo.dwCursorPosition.Y) 
	{ 
		ScrollScreenBuffer(hStdout, 1); 
	} 

	else csbiInfo.dwCursorPosition.Y += 1; 

	if (! SetConsoleCursorPosition(hStdout, 
		csbiInfo.dwCursorPosition)) 
	{
		MessageBox(NULL, "SetConsoleCursorPosition", "Console Error", 
			MB_OK); 
		return;
	}
} 

/* ************************************
* VOID UseBlockIO()
* ����	ʹ��Block IO
**************************************/
VOID UseBlockIO()
{
	HANDLE hStdout, hNewScreenBuffer;
	SMALL_RECT srctReadRect;
	SMALL_RECT srctWriteRect;
	CHAR_INFO chiBuffer[160]; // [2][80];
	COORD coordBufSize;
	COORD coordBufCoord;
	BOOL fSuccess;

	// ��þ��
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	// �½�buffer
	hNewScreenBuffer = CreateConsoleScreenBuffer(
		GENERIC_READ | 	GENERIC_WRITE,// �ɶ���д
		0, // ������
		NULL, // Ĭ�ϰ�ȫ����
		CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE
		NULL); 
	if (hStdout == INVALID_HANDLE_VALUE ||
		hNewScreenBuffer == INVALID_HANDLE_VALUE)
	{
		printf("CreateConsoleScreenBuffer (%d)\n", GetLastError());
		return;
	}

	// ������Ļ��Buffer
	if (! SetConsoleActiveScreenBuffer(hNewScreenBuffer) )
	{
		printf("SetConsoleActiveScreenBuffer (%d)\n", GetLastError());
		return;
	}

	// ����ԴCell����
	srctReadRect.Top = 0; // top left: row 0, col 0
	srctReadRect.Left = 0;
	srctReadRect.Bottom = 1; // bot. right: row 1, col 79
	srctReadRect.Right = 79;

	// ��ʱbuffer��С 2 rows x 80 columns.
	coordBufSize.Y = 2;
	coordBufSize.X = 80;
	// ��ʱbuffer����ʼλ��
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	// ����Ļbuffer�������ݵ���ʱbuffer
	fSuccess = ReadConsoleOutput(
		hStdout, // Դ
		chiBuffer, // Ŀ��
		coordBufSize, // Ŀ��buffer��С
		coordBufCoord, // Ŀ����ʼλ��
		&srctReadRect); // Դ��ʼλ��
	if (! fSuccess)
	{
		printf("ReadConsoleOutput (%d)\n", GetLastError());
		return;
	}

	// ����Ŀ��Cells����
	srctWriteRect.Top = 10; // top lt: row 10, col 0
	srctWriteRect.Left = 0;
	srctWriteRect.Bottom = 11; // bot. rt: row 11, col 79
	srctWriteRect.Right = 79;

	// ����ʱbuffer���µ���Ļbuffer����
	fSuccess = WriteConsoleOutput(
		hNewScreenBuffer, 
		chiBuffer,
		coordBufSize, 
		coordBufCoord, 
		&srctWriteRect); 
	if (! fSuccess)
	{
		printf("WriteConsoleOutput (%d)\n", GetLastError());
		return;
	}
	// �ȵ�һ��ʱ��
	Sleep(10000);

	// �ָ�
	if (! SetConsoleActiveScreenBuffer(hStdout))
	{
		printf("SetConsoleActiveScreenBuffer (%d)\n", GetLastError());
		return;
	}
}


VOID MyErrorExit(LPSTR szErrorText)
{
	LPVOID szShow = HeapAlloc(GetProcessHeap(),
		HEAP_NO_SERIALIZE,lstrlen(szErrorText)+32);
	wsprintf(szShow,"ERROR; %s\nLastErrorCode (%u)",szErrorText,GetLastError());
	MessageBox(NULL,szShow,"ERROR",MB_OK);
	HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, szShow);
	ExitProcess(0);
}
