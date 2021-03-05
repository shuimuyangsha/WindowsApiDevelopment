/* ************************************
*����ͨWindows API�� 
* ʾ������
* CopySend.c
* 11.4 ͨ��WM_COPYDATA���̼�ͨ��
**************************************/
/* ͷ�ļ� */
#include <windows.h> 
#include <commctrl.h>
#include "resource.h"
/* ȫ�ֱ��� */
HINSTANCE hinst;
HWND hwnd, hwndEdit;
LPSTR lpszCopy =  "you can input any text\n then click \'Send\' Item at Menu"; 
/* �������� */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int); 
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM); 

/* ************************************
* WinMain
**************************************/
int WINAPI WinMain(
				   HINSTANCE hinstance, 
				   HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine,
				   int nCmdShow) 
{ 
	MSG msg; 
	WNDCLASSEX wcx; 

	hinst = hinstance;	// ����Ӧ�ó���ʵ��

	// ���WNDCLASSEX
	wcx.cbSize = sizeof(wcx);      
	wcx.style = CS_HREDRAW | CS_VREDRAW;                   
	wcx.lpfnWndProc = MainWndProc;		// ��Ϣ������
	wcx.cbClsExtra = 0;        
	wcx.cbWndExtra = 0;             
	wcx.hInstance = hinstance;   
	wcx.hIcon = LoadIcon(NULL, 	IDI_APPLICATION);          
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);                 
	wcx.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH);               
	wcx.lpszMenuName =  MAKEINTRESOURCE(IDR_MENU_COMMAND);   // �˵�
	wcx.lpszClassName = "copydata";		// ��������
	wcx.hIconSm = NULL; 
	// ע�ᴰ����
	if(RegisterClassEx(&wcx) == 0)
	{
		return 0;
	}
	// ��������
	hwnd = CreateWindow( 
		"copydata",        
		"Sample",           
		WS_TILEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 	400,    
		(HWND) NULL,        
		(HMENU) NULL,      
		hinstance,         
		(LPVOID) NULL);    
	if (!hwnd) 
		return 0; 

	// ��ʾ��ˢ�´���
	ShowWindow(hwnd, nCmdShow); 
	UpdateWindow(hwnd); 
	// ��Ϣѭ��

	BOOL fGotMessage;
	while ((fGotMessage = GetMessage(&msg, (HWND) NULL, 0, 0)) != 0 && fGotMessage != -1) 
	{ 
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	} 
	return msg.wParam; 
	UNREFERENCED_PARAMETER(lpCmdLine); 
} 

/* ************************************
* ��Ϣ������
**************************************/
LONG APIENTRY MainWndProc(HWND hwnd,          
						  UINT message,           
						  WPARAM wParam,         
						  LPARAM lParam)          
{ 
	switch (message) 
	{ 
	case WM_CREATE:
		{
			// �������ڴ���ʱ������һ��EDIT�ؼ������ڱ༭��Ҫ���Ƶ�����
			RECT rectMain;
			GetClientRect(hwnd,&rectMain);
			hwndEdit = CreateWindow("EDIT",    
				NULL,       
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER |
				ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 
				0., 0, rectMain.right, rectMain.bottom, 
				hwnd,  	NULL,
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE), 
				NULL);     
			if(hwndEdit == NULL)
			{
				MessageBox(hwnd, "Create Window Error","ERROR",MB_OK);
				ExitProcess(0);
			}
			// ����EDIT�ؼ�������
			SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM) lpszCopy); 
			break;
		}
	case WM_COMMAND:		// �˵�����
		{
			// ���ͨ���˵������send����ť
			if (LOWORD(wParam) == ID_COMMAND_SEND) 
			{
				// ��ȡEDIT�ؼ��е��ı�
				DWORD dwTextLenth = GetWindowTextLength(hwndEdit);
				LPSTR szText = (LPSTR)HeapAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,dwTextLenth);
				GetWindowText(hwndEdit,szText,dwTextLenth);
				// ���� COPYDATASTRUCT
				COPYDATASTRUCT cds;
				HWND hwnd2 = FindWindow("copydata2","RECV COPY DATA");
				if(hwnd2 == NULL)
				{
					MessageBox(hwnd,"can not find window 2","ERROR",MB_OK);
					break;
				}
				cds.dwData = 0x12345678;			// �Զ��������
				cds.cbData = dwTextLenth;		// lpData�ĳ���
				cds.lpData = szText;				// ��Ҫ���Ƶ�����
				// ������Ϣ��COPYDATASTRUCT�ṹ��ָ����ΪlParam
				SendMessage(    
					hwnd2,									// Ŀ�Ĵ���
					(UINT) WM_COPYDATA,		// ��Ϣ ID
					(WPARAM) hwnd,		//  wParam��Դ���ھ��
					(LPARAM) &cds			//  lParam��COPYDATASTRUCT
					); 
				// �ͷ��ڴ�					
				HeapFree(GetProcessHeap(),HEAP_NO_SERIALIZE,szText);
			} 
			break; 
		}
	case WM_DESTROY:	// �˳�
		PostQuitMessage(0); 
		return 0; 

	default:		// ������Ϣ�Ĵ���
		return DefWindowProc(hwnd, message, wParam, lParam); 
	}
return 0; 
}