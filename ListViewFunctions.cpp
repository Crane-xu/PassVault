#include "PasswordManager.h"


WNDPROC g_originalListViewProc = NULL;

void InitListView(HWND hWnd) {
	hListView = CreateWindow(
		WC_LISTVIEW, L"",
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SHOWSELALWAYS,
		10, 10, 860, 380, hWnd, (HMENU)IDC_LISTVIEW, hInst, NULL
	);
	SetupListView(hListView);

	// 设置扩展样式
	ListView_SetExtendedListViewStyle(hListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 添加列
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;

	lvc.iSubItem = 0;
	lvc.pszText = const_cast<wchar_t*>(L"标题");
	lvc.cx = 180;
	ListView_InsertColumn(hListView, 0, &lvc);

	lvc.iSubItem = 1;
	lvc.pszText = const_cast<wchar_t*>(L"账号");
	lvc.cx = 150;
	ListView_InsertColumn(hListView, 1, &lvc);

	lvc.iSubItem = 2;
	lvc.pszText = const_cast<wchar_t*>(L"创建日期");
	lvc.cx = 150;
	ListView_InsertColumn(hListView, 2, &lvc);
}

// 自定义ListView窗口过程
LRESULT CALLBACK ListViewSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE)
	{
		// 转移焦点到父窗口或其他控件
		SetFocus(GetParent(hWnd));
		return 0; // 已处理，不再传递
	}

	// 调用原始窗口过程处理其他消息
	return CallWindowProc(g_originalListViewProc, hWnd, uMsg, wParam, lParam);
}

// 在创建ListView后设置子类化
void SetupListView(HWND hListView)
{
	g_originalListViewProc = (WNDPROC)SetWindowLongPtr(
		hListView,
		GWLP_WNDPROC,
		(LONG_PTR)ListViewSubclassProc
	);
}