/* ************************************
*����ͨWindows API�� 
* ʾ������
* MainWindow.c
* 9.3-9.6  ͼ���û�����
**************************************/
/* ͷ�ļ� */
#include <windows.h>
#include <commctrl.h>
#include <Winuser.h>
#include <shellapi.h>
#include "resource.h"
#include "Window.h"
/* ȫ�ֱ��� */
HINSTANCE hinst;
HWND hwndMain;
HWND hwndTreeView;
HWND hwndListView;
HWND hwndEdit;
HMENU hMenuMain;
HMENU hPopup;
// �ڽ�������ʾ���ı�
LPSTR lpszLatin =  "Lorem ipsum dolor sit amet, consectetur "
		"adipisicing elit, sed do eiusmod tempor " 
		"incididunt ut labore et dolore magna " 
		"aliqua. Ut enim ad minim veniam, quis " 
		"nostrud exercitation ullamco laboris nisi " 
		"ut aliquip ex ea commodo consequat. Duis " 
		"aute irure dolor in reprehenderit in " 
		"voluptate velit esse cillum dolore eu " 
		"fugiat nulla pariatur. Excepteur sint " 
		"occaecat cupidatat non proident, sunt " 
		"in culpa qui officia deserunt mollit " 
		"anim id est laborum."; 

/* ************************************
* HWND CreateTreeView(HWND hwndParent, LPSTR szWindowName)
* ����	����һ��Tree View�ؼ���������InitTreeViewItems�������ɸ��ڵ�
* ����	hwndParent�������ھ��
*			szWindowName�����ڱ���
* ����ֵ	�����Ĵ��ھ��
**************************************/
HWND CreateTreeView(HWND hwndParent, LPSTR szWindowName)
{
	HWND hwndTV;
	// ����Tree View
	hwndTV = CreateWindowEx(0,
		WC_TREEVIEW,		//	Tree View���ƴ�����
		szWindowName,	// ���ڵı���
		// ������ʽ ���ɼ����Ӵ��ڣ��ɸı��С�����崰�ڱ���
		WS_VISIBLE | WS_CHILD | WS_SIZEBOX |  WS_TILED |
		// ����Tree View��ʽ
		TVS_HASBUTTONS |TVS_LINESATROOT , 
		// Ĭ�ϴ�С��λ�ã�����ʹ��SetWindows��������
		CW_USEDEFAULT,	
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwndParent,		// �����ھ��
		(HMENU)NULL,	// û�в˵�
		hinst,		// Ӧ�ó���ʵ��
		NULL);		// û��ͼ��

	// ��ʼ�� Image List �ͽڵ㣬
	if (!InitTreeViewImageLists(hwndTV) ||
		!InitTreeViewItems(hwndTV))
	{
		DestroyWindow(hwndTV);
		return NULL;
	}
	return hwndTV;
}

/* ************************************
* BOOL InitTreeViewItems(HWND hwndTV)
* ����	ΪTree View�����������ɸ��ڵ�
* ����	hwndTV��Tree View�ؼ����ھ��
**************************************/
BOOL InitTreeViewItems(HWND hwndTV)
{
	// ���ڵ�1
	HTREEITEM hRoot = AddItemToTree(hwndTV, "type1", NULL,TRUE);
	// ���ڵ�1���ӽڵ�1
	HTREEITEM hP = AddItemToTree(hwndTV, "name1", hRoot,FALSE);
	// ���ڵ�1���ӽڵ�2
	hP = AddItemToTree(hwndTV, "name2", hRoot, TRUE);
	// �ӽڵ�2���ӽڵ�
	AddItemToTree(hwndTV, "sub1", hP,FALSE);
	// �ӽڵ�2���˽ڵ�
	AddItemToTree(hwndTV, "sub2", hP,FALSE);
	// ���ڵ�2
	hRoot = AddItemToTree(hwndTV, "type2", NULL,TRUE);
	// ���ڵ�2���ӽڵ�
	hP = AddItemToTree(hwndTV, "name3", hRoot, TRUE);
	// ���ڵ�2�����ӽڵ�
	hP = AddItemToTree(hwndTV, "sub3", hP,FALSE);
	// ���ڵ�3
	hRoot = AddItemToTree(hwndTV, "type3", NULL,TRUE);

	return TRUE;
}

/* ************************************
* AddItemToTree
* ����	ΪTree View�ؼ����ӽڵ�
* ����	hwndTV��Tree View�ؼ����ھ��
*			lpszItem���ڵ������ı���
*			hParent�����ڵ��������ΪNULL�������Ӹ��ڵ�
*			bFolder���Ƿ�����ӽڵ㣨Ӱ��ڵ�ͼ��ICON��
* ����ֵ	�����Ľڵ���
**************************************/
HTREEITEM AddItemToTree(
						HWND hwndTV, 
						LPSTR lpszItem, 
						HTREEITEM hParent, 
						BOOL bFolder)
{
	TVITEM tvi;
	TVITEM tParent;
	TVINSERTSTRUCT tvins;
	HTREEITEM hme;
	// ���TVITEM�ṹ
	// ͨ��TVITEM��mask�ֶΣ�˵���Ǵ��ֶ�����Ч��
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

	// �ֽڵ��ı����ı�����
	tvi.pszText = lpszItem;
	tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]);

	// ����bFolder����Ϊ�ڵ�ָ����ͬ��ͼ��
	if(bFolder)
	{
		tvi.iImage = 1;
		tvi.iSelectedImage = 2;
		tvi.cChildren = I_CHILDRENCALLBACK;
	}
	else
	{
		tvi.iImage = 3;
		tvi.iSelectedImage = 3;
		tvi.cChildren = 0;
	}

	// ���TVINSERTSTRUCT�ṹ
	tvins.item = tvi;
	// �²�����ӽڵ��λ�ã�TVI_SORT��ʾ���ı�����
	// ��������TVI_FIRST TVI_LAST�ȡ�	
	tvins.hInsertAfter = TVI_SORT;

	// ���hParentΪNULL�������Ľڵ�Ϊ���ڵ㣬
	// ����hParentΪ�丸�ڵ�
	if(hParent == NULL)
	{
		tvins.hParent = TVI_ROOT;
	}
	else
	{
		tvins.hParent = hParent;
	}
	// ����TreeView_InsertItem�꣬�����½ڵ�
	hme = TreeView_InsertItem(hwndTV,&tvins);
	return hme;
}

/* ************************************
* BOOL OnRclickTree(NMHDR* pNMHDR)
* ����	�ڴ��ڱ��Ҽ���������ã�������ʾ�Ҽ��˵�
*			�ж��Ƿ���Tree View�ڵ��ϵ��������������λ��
* ����	pNMHDR��WM_NOTITY ��Ϣ�� lParam
* ����ֵ	�Ƿ񵥻��˽ڵ�
**************************************/
BOOL OnRclickTree(NMHDR* pNMHDR)
{
	POINT point;
	TVHITTESTINFO thti;
	HTREEITEM htItem;

	// ��ȡ����λ��
	GetCursorPos(&point);

	// �������Client�����λ�ã�
	// TreeView_HitTestҪ������λ���������Client
	ScreenToClient(hwndTreeView,&point);
	// ���TVHITTESTINFO�ṹ
	thti.pt = point;
	thti.flags = TVHT_TORIGHT;

	// TreeView_HitTest�����Ƿ������˽ڵ㣬��ȡ������ڵ�ľ��
	htItem =TreeView_HitTest(hwndTreeView,&thti);

	// Ҳ���Ը��ݽڵ����ԵĲ�ͬ��������ͬ���Ҽ��˵�
	// TreeView_GetItem�Ⱥ����������ڻ�ȡTVITEM�ṹ�еĸ����ֶ�
	// �û���������lParam�ֶ���������ڵ���صĸ�����Ϣ
	if(htItem!=NULL)
	{
		// DisplayContextMenu����Ҫ������λ�����������Ļ��
		ClientToScreen(hwndTreeView,&point);
		// ѡ���û��һ��Ľڵ㣬����Tree View�б�ѡ�еĽڵ㲻��ı�
		TreeView_SelectItem(hwndTreeView,htItem);
		// ����DisplayContextMenu������ʾ�Ҽ��˵�
		DisplayContextMenu(hwndMain, point);
		return TRUE;
	}
	return FALSE;
}


/* ************************************
* VOID APIENTRY DisplayContextMenu(HWND hwnd, POINT pt)
* ����	����Ļ����ʾ�����˵�
* ����	hwnd���˵���������
*			pt���˵�λ�ã��������Ļ��
**************************************/
VOID APIENTRY DisplayContextMenu(HWND hwnd, POINT pt)
{
	HMENU hmenu; // �����˵�
	HMENU hmenuTrackPopup; // �����˵�

	// ���ز˵���Դ����ò˵����
	if ((hmenu = LoadMenu(hinst, MAKEINTRESOURCE(IDR_MENU_POPUP))) == NULL)
		return;

	// ��ò˵���Դ�е��Ӳ˵�������ʾ���ǲ˵���Դ�е��Ӳ˵�
	hmenuTrackPopup = GetSubMenu(hmenu, 0);
	// ����
	TrackPopupMenuEx(hmenuTrackPopup,
		TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		pt.x, pt.y, hwnd, NULL);
	// ��ɣ��ͷ������Դ
	DestroyMenu(hmenu);
}

// δ��ʼ�� Tree View�� ImageList����˽ڵ㽫û��ͼ��
// ��Ȼ�ڲ���ڵ�ʱָ���˽ڵ��Image����������ImageListΪ�գ�
// ���ͼ�겻����ʾ
BOOL InitTreeViewImageLists(HWND hwndTV)
{
	// TODO ���� ImageLists 
	// TODO ���� ImageLists ��
	// TODO ʹ�� TreeView_SetImageList ����Tree View�� Image List
	return TRUE;
}

/* ************************************
* HWND CreateListView (HWND hwndParent, LPSTR szWindowName)
* ����	���� List View
* ����	hwndParent��������
*			szWindowName�����ڱ���
* ����ֵ	���ھ��
**************************************/
HWND CreateListView (HWND hwndParent, LPSTR szWindowName)
{
	HWND hWndListView;
	// ����List View
	hWndListView = CreateWindow (
		WC_LISTVIEW, // List View������
		// ���ڱ��⣬����δָ�� WS_TILED ������һ���ޱ���Ĵ���
		szWindowName,
		// ������ʽ�����ӣ��Ӵ��ڣ��Լ�List View��ʽ
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
		// λ�úʹ�С��������ɺ�ʹ��SetWindows����
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwndParent,		// ������
		NULL, hinst, NULL);	// �޲˵�����ͼ��
	if (hWndListView == NULL)
	{
		return NULL;
	}
	// ��ʼ��Image List����ʼ���С�����һЩ��
	if(InitListViewImageLists(hWndListView) &&
		InitListViewColumns(hWndListView) &&
		AddListViewItems(hWndListView))
	{
		return hWndListView;
	}
	DestroyWindow(hWndListView);
	return FALSE;
}

/* ************************************
* BOOL InitListViewImageLists(HWND hWndListView)
* ����	���� List View �� ImageList
* ����	hWndListView��List View �ؼ�����
**************************************/
BOOL InitListViewImageLists(HWND hWndListView)
{
	HIMAGELIST himl; 
	HBITMAP hbmp; 
	// ���� GetImageList ���ImageList
	GetImageList(&himl);
	// ���� List View�ĵ� ImageList
	ListView_SetImageList(hWndListView, himl, LVSIL_SMALL);
	return TRUE;
}

/* ************************************
* BOOL InitListViewColumns(HWND hWndListView)
* ����	���� List View �� ����
* ����	hWndListView��List View �ؼ�����
* ����ֵ	�Ƿ�ɹ�
**************************************/
BOOL InitListViewColumns(HWND hWndListView)
{
	char szText[256]; 
	LVCOLUMN lvc;
	DWORD i;
	// ��������
	LPSTR ColNames[] = {"name", "type", "size", "time"};
	// LVCOLUMN����Ч�ĳ�Ա
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	// ���LVCOLUMN�ṹ
	lvc.pszText = szText;
	lvc.cx = 100;						// ����
	lvc.iImage = 1;
	lvc.fmt = LVCFMT_LEFT;	// �������
	// ǰ����
	for(i=0; i<3; i++)
	{
		//	���������ı�
		lvc.pszText = ColNames[i];
		lvc.iSubItem = i;
		// ����ListView_InsertColumn�������
		if (ListView_InsertColumn(hWndListView, i,	&lvc) == -1)
		{
			return FALSE;
		}
	}
	// ���һ������
	lvc.cx = 200; // ����
	lvc.fmt = LVCFMT_RIGHT; // �Ҷ���
	lvc.iSubItem = 3;
	lvc.pszText = ColNames[3];
	// �������
	if (ListView_InsertColumn(hWndListView, 3,	&lvc) == -1)
	{
		return FALSE;
	}
	return TRUE;
}

/* ************************************
* BOOL AddListViewItems(HWND hwndListView)
* ����	ΪList View������
* ����	hwndListView��������
* ����ֵ	���ھ��
**************************************/
BOOL AddListViewItems(HWND hwndListView)
{
	LVITEM lvI;
	DWORD index;

	ZeroMemory (&lvI,sizeof(lvI));
	// ��Ч����
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	// ���LVITEM
	lvI.state = 0;
	lvI.stateMask = 0;
	// ѭ������
	for (index = 0; index < 3; index++)
	{
		lvI.iItem = index;
		lvI.iImage = 0;
		lvI.iSubItem = 0;
		// ����ı��ͳ���
		lvI.pszText = "name1"; 
		lvI.cchTextMax = lstrlen(lvI.pszText)+1;
		// ������
		if(ListView_InsertItem(hwndListView, &lvI) == -1)
			return FALSE;
		// ����������ı���0 based
		ListView_SetItemText(hwndListView, index, 2, "size a");
	}
	return TRUE;
}
/* ************************************
* BOOL GetImageList(HIMAGELIST * pHiml)
* ����	TODO ����ImageList��
* ����	pHiml��Image List
**************************************/
BOOL GetImageList(HIMAGELIST * pHiml)
{
	return TRUE;
}

/* ************************************
* HWND CreateEdit(HWND hwndP, LPSTR szTitle)
* ����	�����ı��༭�ؼ�
* ����	hwndP�������ھ��
*			szTitle�����ڱ���
* ����ֵ	���ھ����ʧ�ܷ��� NULL
**************************************/
HWND CreateEdit(HWND hwndP, LPSTR szTitle)
{
	WNDCLASSEX wcx;
	HWND hwnd;

	hwnd = CreateWindow(
		"EDIT", // �ı�����ؼ�
		NULL, // û�б���
		// ������ʽ
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER |
		// �ı�����ʽ
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 
		// λ�úʹ�С
		CW_USEDEFAULT ,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		hwndP, // ������
		(HMENU) NULL, // �޲˵�
		hinst, // ʵ�����
		(LPVOID) NULL); // ��ͼ��

	if (!hwnd)
	{
		return NULL;
	}
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	return hwnd;
}

/* ************************************
* HWND ShowTextOnEdit(HWND hwndP, LPSTR szTitle)
* ����	���ı�������Ϣ�������ı�
* ����	szTextToShow����Ҫ��ʾ���ı�
**************************************/
DWORD ShowTextOnEdit(LPSTR szTextToShow)
{
	// ���ı�����WM_SETTEXT��Ϣ�����ô��ڵ��ı�
	SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM) szTextToShow); 
	return 0;
}