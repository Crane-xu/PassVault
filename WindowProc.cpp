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
		// �����˵�
		hMenu = CreateMenu();
		HMENU hFileMenu = CreateMenu();
		HMENU hHelpMenu = CreateMenu();

		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_NEW, L"�½�");
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_OPEN, L"��...");
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVE, L"����");
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_IMPORT, L"����...");
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVEAS, L"����...");
		AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(hFileMenu, MF_STRING, IDM_FILE_EXIT, L"�˳�");

		AppendMenu(hHelpMenu, MF_STRING, IDM_HELP_ABOUT, L"����");
		AppendMenu(hHelpMenu, MF_STRING, IDM_HELP_SETTINGS, L"����");

		AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"�ļ�");
		AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, L"����");

		SetMenu(hWnd, hMenu);

		// ����״̬��
		hStatusBar = CreateWindowEx(
			0, STATUSCLASSNAME, NULL,
			WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
			hWnd, (HMENU)IDM_STATUSBAR, hInst, NULL
		);

		// ����ListView
		InitListView(hWnd);

		// ������ť
		hCopyBtn = CreateWindow(
			L"BUTTON", L"��������",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			10, 400, 80, 30, hWnd, (HMENU)IDC_COPY_BUTTON, hInst, NULL
		);

		hUnameBtn = CreateWindow(
			L"BUTTON", L"�����˺�",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			100, 400, 80, 30, hWnd, (HMENU)IDC_UNAME_BUTTON, hInst, NULL
		);

		hAddBtn = CreateWindow(
			L"BUTTON", L"���",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			190, 400, 80, 30, hWnd, (HMENU)IDC_ADD_BUTTON, hInst, NULL
		);

		hDeleteBtn = CreateWindow(
			L"BUTTON", L"ɾ��",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			280, 400, 80, 30, hWnd, (HMENU)IDC_DELETE_BUTTON, hInst, NULL
		);

		hGenerateBtn = CreateWindow(
			L"BUTTON", L"��������",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			370, 400, 100, 30, hWnd, (HMENU)IDC_GENERATE_BUTTON, hInst, NULL
		);

		// ��������
		SendMessage(hListView, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hCopyBtn, WM_SETFONT, (WPARAM)hBoldFont, TRUE);
		SendMessage(hAddBtn, WM_SETFONT, (WPARAM)hBoldFont, TRUE);
		SendMessage(hDeleteBtn, WM_SETFONT, (WPARAM)hBoldFont, TRUE);
		SendMessage(hUnameBtn, WM_SETFONT, (WPARAM)hBoldFont, TRUE);
		SendMessage(hGenerateBtn, WM_SETFONT, (WPARAM)hBoldFont, TRUE);

		UpdateStatusBar(L"����");
		break;
	}

	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case IDM_FILE_NEW:
			passwordEntries.clear();
			ListView_DeleteAllItems(hListView);
			currentFilePath.clear();
			SetWindowText(hMainWindow, L"���������");
			UpdateStatusBar(L"�Ѵ����µ������");
			break;

		case IDM_FILE_OPEN: {
			OPENFILENAME ofn;
			wchar_t szFile[MAX_PATH] = { 0 };

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"JSON�ļ� (*.json)\0*.json\0�����ļ� (*.*)\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn)) {
				currentFilePath = szFile;
				LoadPasswordsFromFile(currentFilePath);
				std::wstring title = L"��������� - " + currentFilePath;
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
				UpdateStatusBar(L"�ѱ��������");
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
			ofn.lpstrFilter = L"JSON�ļ� (*.json)\0*.json\0�����ļ� (*.*)\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
			ofn.lpstrDefExt = L"json";

			if (GetSaveFileName(&ofn)) {
				ExportPasswordsToFile(szFile);
				UpdateStatusBar(L"�ѱ��������");
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
			ofn.lpstrFilter = L"JSON�ļ� (*.json)\0*.json\0�����ļ� (*.*)\0*.*\0";
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
				UpdateStatusBar(L"�˺��Ѹ��Ƶ�������");
				SetFocus(hMainWindow);
			}
			else {
				MessageBox(hWnd, L"����ѡ��һ��������Ŀ", L"��ʾ", MB_ICONINFORMATION);
			}
			break;
		}

		case IDC_DELETE_BUTTON: {
			int selectedIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
			if (selectedIndex != -1) {
				if (MessageBox(hWnd, L"ȷ��Ҫɾ�����������Ŀ��", L"ȷ��ɾ��",
					MB_YESNO | MB_ICONWARNING) == IDYES) {
					DeletePasswordEntry(selectedIndex);
					UpdateStatusBar(L"��ɾ��������Ŀ");
				}
			}
			else {
				MessageBox(hWnd, L"����ѡ��һ��������Ŀ", L"��ʾ", MB_ICONINFORMATION);
			}
			break;
		}

		case IDC_COPY_BUTTON: {
			int selectedIndex = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
			if (selectedIndex != -1) {
				CopyPasswordToClipboard(selectedIndex);
				UpdateStatusBar(L"�����Ѹ��Ƶ�������");
				SetFocus(hMainWindow);
			}
			else {
				MessageBox(hWnd, L"����ѡ��һ��������Ŀ", L"��ʾ", MB_ICONINFORMATION);
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
				MessageBox(hWnd, L"������������ɲ����Ƶ�������", L"��������", MB_ICONINFORMATION);
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
					std::wstring status = L"��ѡ��: " + passwordEntries[index].website;
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

		// ����ListView��С
		MoveWindow(hListView, 10, 10, width - 20, 380, TRUE);

		// ������ťλ��
		MoveWindow(hCopyBtn, 10, height - 100, 80, 30, TRUE);
		MoveWindow(hUnameBtn, 100, height - 100, 80, 30, TRUE);
		MoveWindow(hAddBtn, 190, height - 100, 80, 30, TRUE);
		MoveWindow(hDeleteBtn, 280, height - 100, 80, 30, TRUE);
		MoveWindow(hGenerateBtn, 370, height - 100, 100, 30, TRUE);

		// ����״̬��
		SendMessage(hStatusBar, WM_SIZE, 0, 0);
		break;
	}

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			// ��鵱ǰ�Ƿ���ģ̬�Ի���򽹵���ĳ���ؼ���
			HWND hFocus = GetFocus();
			if (GetFocus() == hListView) {
				SetFocus(hMainWindow);
			}
			if (hFocus == hMainWindow || hFocus == NULL) {  // �����������ǽ���ʱ���˳�
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