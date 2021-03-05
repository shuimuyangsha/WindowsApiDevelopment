#include <windows.h> 

void NewLine(void); 
void ScrollScreenBuffer(HANDLE, INT); 
VOID HandleInput(LPSTR szInput);

HANDLE hStdout, hStdin; 
CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 

void main(void) 
{ 
	LPSTR lpszPrompt = "Type a line and press Enter, 255 max \n q to quit "
		"\n i to Show console info\n c to Change backgroud color \n s to Change text color"
		"\n t to Find upper case character\n b to Call UseBlockIO \n u to Change conslos titile"
		"\n m to Show console event \n r to Change mode \n p to Clear up Console\n";

	CHAR chBuffer[256]; 
	DWORD cRead, cWritten, fdwMode, fdwOldMode; 
	WORD wOldColorAttrs; 

	//  STDIN �� STDOUT ���
	hStdin = GetStdHandle(STD_INPUT_HANDLE); 
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	if (hStdin == INVALID_HANDLE_VALUE || 
		hStdout == INVALID_HANDLE_VALUE) 
	{
		// ʹ��MessageBox����
		MessageBox(NULL, "GetStdHandle", "Console Error", MB_OK);
		return;
	}

	// ��ǰ������ɫ
	if (! GetConsoleScreenBufferInfo(hStdout, &csbiInfo)) 
	{
		MessageBox(NULL, "GetConsoleScreenBufferInfo", 
			"Console Error", MB_OK); 
		return;
	}
	wOldColorAttrs = csbiInfo.wAttributes; 

	// �����ַ���ɫ���죩�ͱ���

	ChangeTextColor(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);

	// ����ַ�
		if (! WriteConsole( 
			hStdout,              // ������
			lpszPrompt,          // ����ַ��� 
			lstrlen(lpszPrompt), // �ַ������� 
			&cWritten,            // ����ʵ������ĳ��� 
			NULL) )               // �� overlapped
		{
			MessageBox(NULL, "WriteFile", "Console Error", MB_OK); 
			return; 
		}
	while (1) 
	{
		ZeroMemory(chBuffer,256);
		if (! ReadConsole( 
			hStdin,    // input handle 
			chBuffer,  // buffer to read into 
			255,       // size of buffer 
			&cRead,    // actual bytes read 
			NULL) )    // not overlapped 
			break; 
		HandleInput(chBuffer);
		//if (chBuffer[0] == 'q') break; 
	} 



	//// Restore the original text colors. 

	//SetConsoleTextAttribute(hStdout, wOldColorAttrs);
}

VOID HandleInput(LPSTR szInput)
{
	switch(szInput[0])
	{
	case 'q':
		ExitProcess(0);
	case 'i':
		GetConsoleInfo(hStdout);
		break;
	case 'c':
		ChangeBackgroudColor(hStdout);
		break;
	case 's':
		ChangeTextColor(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case 't':
		ChangeTextBackgroudColor(hStdin, hStdout);
		break;
	case 'b':
		UseBlockIO();
		break;
	case 'u':
		ChangeConsoleTitle(&szInput[2]);
		break;
	case 'm':
		UseEvent();
		break;
	case 'r':
		ChangeMode(hStdin,hStdout);
		break;
	case 'p':
		cls(hStdout);
		break;
	default:
		return;
	}
	return;
}



// The NewLine function handles carriage returns when the processed 
// input mode is disabled. It gets the current cursor position 
// and resets it to the first cell of the next row. 

void NewLine(void) 
{ 
	if (! GetConsoleScreenBufferInfo(hStdout, &csbiInfo)) 
	{
		MessageBox(NULL, "GetConsoleScreenBufferInfo", 
			"Console Error", MB_OK); 
		return;
	}

	csbiInfo.dwCursorPosition.X = 0; 

	// If it is the last line in the screen buffer, scroll 
	// the buffer up. 

	if ((csbiInfo.dwSize.Y-1) == csbiInfo.dwCursorPosition.Y) 
	{ 
		ScrollScreenBuffer(hStdout, 1); 
	} 

	// Otherwise, advance the cursor to the next line. 

	else csbiInfo.dwCursorPosition.Y += 1; 

	if (! SetConsoleCursorPosition(hStdout, 
		csbiInfo.dwCursorPosition)) 
	{
		MessageBox(NULL, "SetConsoleCursorPosition", "Console Error", 
			MB_OK); 
		return;
	}
} 
