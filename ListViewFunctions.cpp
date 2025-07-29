#include "PasswordManager.h"


WNDPROC g_originalListViewProc = NULL;

void InitListView(HWND hWnd) {
	hListView = CreateWindow(
		WC_LISTVIEW, L"",
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SHOWSELALWAYS,
		10, 10, 860, 380, hWnd, (HMENU)IDC_LISTVIEW, hInst, NULL
	);
	SetupListView(hListView);

	// ������չ��ʽ
	ListView_SetExtendedListViewStyle(hListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// �����
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;

	lvc.iSubItem = 0;
	lvc.pszText = const_cast<wchar_t*>(L"����");
	lvc.cx = 180;
	ListView_InsertColumn(hListView, 0, &lvc);

	lvc.iSubItem = 1;
	lvc.pszText = const_cast<wchar_t*>(L"�˺�");
	lvc.cx = 150;
	ListView_InsertColumn(hListView, 1, &lvc);

	lvc.iSubItem = 2;
	lvc.pszText = const_cast<wchar_t*>(L"��������");
	lvc.cx = 150;
	ListView_InsertColumn(hListView, 2, &lvc);
}

// �Զ���ListView���ڹ���
LRESULT CALLBACK ListViewSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE)
	{
		// ת�ƽ��㵽�����ڻ������ؼ�
		SetFocus(GetParent(hWnd));
		return 0; // �Ѵ������ٴ���
	}

	// ����ԭʼ���ڹ��̴���������Ϣ
	return CallWindowProc(g_originalListViewProc, hWnd, uMsg, wParam, lParam);
}

// �ڴ���ListView���������໯
void SetupListView(HWND hListView)
{
	g_originalListViewProc = (WNDPROC)SetWindowLongPtr(
		hListView,
		GWLP_WNDPROC,
		(LONG_PTR)ListViewSubclassProc
	);
}