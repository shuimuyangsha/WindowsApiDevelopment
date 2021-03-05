/* ************************************
*����ͨWindows API�� 
* ʾ������
* ConsoleIO.c
* 9.2  �ַ��������
**************************************/
/* ͷ�ļ� */
#include <windows.h> 
/* �������� */
void ScrollScreenBuffer(HANDLE, INT); 
VOID HandleInput(LPSTR szInput);
/* ȫ�ֱ��� */
HANDLE hStdout, hStdin; 
CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 

/* ************************************
* int main(void)
* ����	��ʾ����̨�����������
* ����	��		����ֵ	��
**************************************/
void main(void) 
{
	// ��ʾ

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
		hStdout,					// ������
		lpszPrompt,				// ����ַ��� 
		lstrlen(lpszPrompt),	// �ַ������� 
		&cWritten,				// ����ʵ������ĳ��� 
		NULL) )						// �� overlapped
	{
		MessageBox(NULL, "WriteFile", "Console Error", MB_OK); 
		return; 
	}
	while (1) 
	{
		// ѭ���ȴ��û�����
		ZeroMemory(chBuffer,256);
		if (! ReadConsole( 
			hStdin,    // �������̨���
			chBuffer,  // �������ݵĻ����� 
			255,       // ��������С
			&cRead,    // ʵ�ʶ����Ĵ�С 
			NULL) )    // �� overlapped 
			break; 
		// �����û�������
		HandleInput(chBuffer);
	} 
}

/* ************************************
* VOID HandleInput(LPSTR szInput)
* ����	�����û�������
* ����	�û�������ַ���
**************************************/
VOID HandleInput(LPSTR szInput)
{
	switch(szInput[0])
	{
	case 'q':	// �˳�����
		ExitProcess(0);
	case 'i':		// ��ʾ����̨��Ϣ 
		GetConsoleInfo(hStdout);
		break;
	case 'c':		// ���ý��汳����ɫ
		ChangeBackgroudColor(hStdout);
		break;
	case 's':		// �����ļ���ɫ
		ChangeTextColor(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case 't':		// �����ļ�����
		ChangeUpCaseTextBackgroudColor(hStdin, hStdout);
		break;
	case 'b':	// ʹ��Block IO
		UseBlockIO();
		break;
	case 'u':	// ���ÿ���̨����
		ChangeConsoleTitle(&szInput[2]);
		break;
	case 'm':	// ʹ���¼�
		UseEvent();
		break;
	case 'r':		// �ı�ģʽ
		ChangeMode(hStdin,hStdout);
		break;
	case 'p':	// ����
		cls(hStdout);
		break;
	default:		// ������ֱ�ӷ���
		return;
	}
	return;
}