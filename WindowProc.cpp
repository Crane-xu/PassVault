#include "PasswordManager.h"

HWND hMainWindow, hListView, hStatusBar;
HWND hAddBtn, hDeleteBtn, hCopyBtn, hGenerateBtn, hUnameBtn;
HMENU hMenu;
HINSTANCE hInst;
std::vector<PasswordEntry> passwordEntries;
std::wstring currentFilePath;
HFONT hFont, hBoldFont;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE: {
		// 创建菜单
		hMenu = CreateMenu();
		HMENU hFileMenu = CreateMenu();
		HMENU hHelpMenu = CreateMenu();

		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_NEW, L"新建");
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_OPEN, L"打开...");
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVE, L"保存");
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_IMPORT, L"导入...");
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVEAS, L"导出...");
		AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_EXIT, L"退出");

		AppendMenu(hHelpMenu, MF_STRING, IDM_HELP_ABOUT, L"关于");
		AppendMenu(hHelpMenu, MF_STRING, IDM_HELP_SETTINGS, L"设置");

		AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"文件");
		AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, L"帮助");

		SetMenu(hWnd, hMenu);

		// 创建状态栏
		hStatusBar = CreateWindowEx(
			0, STATUSCLASSNAME, NULL,
			WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
			hWnd, (HMENU)IDM_STATUSBAR, hInst, NULL
		);

		// 创建ListView
		InitListView(hWnd);

		// 创建按钮
		hCopyBtn = CreateWindow(
			L"BUTTON", L"复制密码",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			10, 400, 80, 30, hWnd, (HMENU)IDC_COPY_BUTTON, hInst, NULL
		);

		hUnameBtn = CreateWindow(
			L"BUTTON", L"复制账号",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			100, 400, 80, 30, hWnd, (HMENU)IDC_UNAME_BUTTON, hInst, NULL
		);

		hAddBtn = CreateWindow(
			L"BUTTON", L"添加",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			190, 400, 80, 30, hWnd, (HMENU)IDC_ADD_BUTTON, hInst, NULL
		);

		hDeleteBtn = CreateWindow(
			L"BUTTON", L"删除",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			280, 400, 80, 30, hWnd, (HMENU)IDC_DELETE_BUTTON, hInst, NULL
		);

		hGenerateBtn = CreateWindow(
			L"BUTTON", L"生成密码",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			370, 400, 100, 30, hWnd, (HMENU)IDC_GENERATE_BUTTON, hInst, NULL
		);

		// 设置字体
		SendMessage(hListView, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hCopyBtn, WM_SETFONT, (WPARAM)hBoldFont, TRUE);
		SendMessage(hAddBtn, WM_SETFONT, (WPARAM)hBoldFont, TRUE);
		SendMessage(hDeleteBtn, WM_SETFONT, (WPARAM)hBoldFont, TRUE);
		SendMessage(hUnameBtn, WM_SETFONT, (WPARAM)hBoldFont, TRUE);
		SendMessage(hGenerateBtn, WM_SETFONT, (WPARAM)hBoldFont, TRUE);

		UpdateStatusBar(L"就绪");
		break;
	}

	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case IDM_FILE_NEW:
			passwordEntries.clear();
			ListView_DeleteAllItems(hListView);
			currentFilePath.clear();
			SetWindowText(hMainWindow, L"密码管理器");
			UpdateStatusBar(L"已创建新的密码库");
			break;

		case IDM_FILE_OPEN: {
			OPENFILENAME ofn;
			wchar_t szFile[MAX_PATH] = { 0 };

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"JSON文件 (*.json)\0*.json\0所有文件 (*.*)\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn)) {
				currentFilePath = szFile;
				LoadPasswordsFromFile(currentFilePath);
				std::wstring title = L"密码管理器 - " + currentFilePath;
				SetWindowText(hMainWindow, title.c_str());
			}
			break;
		}

		case IDM_FILE_SAVE:
			if (currentFilePath.empty()) {
				SendMessage(hWnd, WM_COMMAND, IDM_FILE_SAVEAS, 0);
			}
			else {
				SavePasswordsToFile(currentFilePath);
				UpdateStatusBar(L"已保存密码库");
			}
			break;

		case IDM_FILE_SAVEAS: {
			OPENFILENAME ofn;
			wchar_t szFile[MAX_PATH] = { 0 };

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"JSON文件 (*.json)\0*.json\0所有文件 (*.*)\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
			ofn.lpstrDefExt = L"json";

			if (GetSaveFileName(&ofn)) {
				ExportPasswordsToFile(szFile);
				UpdateStatusBar(L"已保存密码库");
			}
			break;
		}

		case IDM_FILE_IMPORT: {
			OPENFILENAME ofn;
			wchar_t szFile[MAX_PATH] = { 0 };

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"JSON文件 (*.json)\0*.json\0所有文件 (*.*)\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn)) {
				ImportPasswordsFromFile(szFile);
			}
			break;
		}

		case IDM_FILE_EXIT:
			DestroyWindow(hWnd);
			break;

		case IDC_ADD_BUTTON:
			ShowPasswordEntryDialog(hWnd);
			break;

		case IDC_UNAME_BUTTON: {
			int selectedIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
			if (selectedIndex != -1) {
				CopyUnameToClipboard(selectedIndex);
				UpdateStatusBar(L"账号已复制到剪贴板");
				SetFocus(hMainWindow);
			}
			else {
				MessageBox(hWnd, L"请先选择一个密码条目", L"提示", MB_ICONINFORMATION);
			}
			break;
		}

		case IDC_DELETE_BUTTON: {
			int selectedIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
			if (selectedIndex != -1) {
				if (MessageBox(hWnd, L"确定要删除这个密码条目吗？", L"确认删除",
					MB_YESNO | MB_ICONWARNING) == IDYES) {
					DeletePasswordEntry(selectedIndex);
					UpdateStatusBar(L"已删除密码条目");
				}
			}
			else {
				MessageBox(hWnd, L"请先选择一个密码条目", L"提示", MB_ICONINFORMATION);
			}
			break;
		}

		case IDC_COPY_BUTTON: {
			int selectedIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
			if (selectedIndex != -1) {
				CopyPasswordToClipboard(selectedIndex);
				UpdateStatusBar(L"密码已复制到剪贴板");
				SetFocus(hMainWindow);
			}
			else {
				MessageBox(hWnd, L"请先选择一个密码条目", L"提示", MB_ICONINFORMATION);
			}
			break;
		}

		case IDC_GENERATE_BUTTON: {
			std::wstring password = GenerateRandomPassword(16);
			if (OpenClipboard(hWnd)) {
				EmptyClipboard();
				HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, (password.length() + 1) * sizeof(wchar_t));
				wchar_t* pchData = (wchar_t*)GlobalLock(hClipboardData);
				wcscpy_s(pchData, password.length() + 1, password.c_str());
				GlobalUnlock(hClipboardData);
				SetClipboardData(CF_UNICODETEXT, hClipboardData);
				CloseClipboard();
				MessageBox(hWnd, L"随机密码已生成并复制到剪贴板", L"生成密码", MB_ICONINFORMATION);
			}
			break;
		}

		case IDM_HELP_ABOUT:
			ShowAboutDialog(hWnd);
			break;

		case IDM_HELP_SETTINGS:
			ShowSettingsDialog(hWnd);
			break;
		}
		break;
	}

	case WM_NOTIFY: {
		switch (((LPNMHDR)lParam)->code) {
		case LVN_ITEMCHANGED: {
			LPNMLISTVIEW pnmlv = (LPNMLISTVIEW)lParam;
			if (pnmlv->uNewState & LVIS_SELECTED) {
				int index = pnmlv->iItem;
				if (index >= 0 && index < passwordEntries.size()) {
					std::wstring status = L"已选择: " + passwordEntries[index].website;
					UpdateStatusBar(status);
				}
			}
			break;
		}
		}
		break;
	}

	case WM_SIZE: {
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);

		// 调整ListView大小
		MoveWindow(hListView, 10, 10, width - 20, 380, TRUE);

		// 调整按钮位置
		MoveWindow(hCopyBtn, 10, height - 100, 80, 30, TRUE);
		MoveWindow(hUnameBtn, 100, height - 100, 80, 30, TRUE);
		MoveWindow(hAddBtn, 190, height - 100, 80, 30, TRUE);
		MoveWindow(hDeleteBtn, 280, height - 100, 80, 30, TRUE);
		MoveWindow(hGenerateBtn, 370, height - 100, 100, 30, TRUE);

		// 调整状态栏
		SendMessage(hStatusBar, WM_SIZE, 0, 0);
		break;
	}

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			// 检查当前是否有模态对话框或焦点在某个控件上
			HWND hFocus = GetFocus();
			if (GetFocus() == hListView) {
				SetFocus(hMainWindow);
			}
			if (hFocus == hMainWindow || hFocus == NULL) {  // 仅当主窗口是焦点时才退出
				DestroyWindow(hWnd);
				return 0;
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}