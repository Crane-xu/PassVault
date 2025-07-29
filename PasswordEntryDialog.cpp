#include "PasswordManager.h"

void ShowPasswordEntryDialog(HWND hWnd, const PasswordEntry* entry) {
	// ע��Ի��򴰿���
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = [](HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
		switch (message) {
		case WM_CREATE: {
			// �����ؼ�
			CreateWindow(L"STATIC", L"����:", WS_VISIBLE | WS_CHILD, 20, 20, 80, 20,
				hDlg, (HMENU)IDC_WEBSITE_LABEL, hInst, NULL);
			CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 110, 20, 350, 25,
				hDlg, (HMENU)IDC_WEBSITE_EDIT, hInst, NULL);

			CreateWindow(L"STATIC", L"�˺�:", WS_VISIBLE | WS_CHILD, 20, 55, 80, 20,
				hDlg, (HMENU)IDC_USERNAME_LABEL, hInst, NULL);
			CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 110, 55, 350, 25,
				hDlg, (HMENU)IDC_USERNAME_EDIT, hInst, NULL);

			CreateWindow(L"STATIC", L"����:", WS_VISIBLE | WS_CHILD, 20, 90, 80, 20,
				hDlg, (HMENU)IDC_PASSWORD_LABEL, hInst, NULL);
			CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 110, 90, 250, 25,
				hDlg, (HMENU)IDC_PASSWORD_EDIT, hInst, NULL);
			CreateWindow(L"BUTTON", L"��������", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 370, 90, 90, 25,
				hDlg, (HMENU)IDC_GENERATE_PASSWORD, hInst, NULL);

			CreateWindow(L"BUTTON", L"ȷ��", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				250, 150, 80, 30, hDlg, (HMENU)IDOK, hInst, NULL);
			CreateWindow(L"BUTTON", L"ȡ��", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
				340, 150, 80, 30, hDlg, (HMENU)IDCANCEL, hInst, NULL);

			// ��������
			SendMessage(hDlg, WM_SETFONT, (WPARAM)hFont, TRUE);
			// �����������Ŀ���ݣ���䵽�ؼ���
			PasswordEntry* entry = reinterpret_cast<PasswordEntry*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
			if (entry) {
				SetWindowText(GetDlgItem(hDlg, IDC_WEBSITE_EDIT), entry->website.c_str());
				SetWindowText(GetDlgItem(hDlg, IDC_USERNAME_EDIT), entry->username.c_str());
				SetWindowText(GetDlgItem(hDlg, IDC_PASSWORD_EDIT), entry->password.c_str());
			}
			return 0;
		}

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
			case IDOK: {
				wchar_t buffer[1024];
				PasswordEntry newEntry;

				// ����Ǹ���������Ŀ������ԭID�ʹ�������
				PasswordEntry* oldEntry = reinterpret_cast<PasswordEntry*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
				if (oldEntry) {
					newEntry.id = oldEntry->id;
					newEntry.createdDate = oldEntry->createdDate;
				}
				else {
					// ����Ŀ��������ID�ʹ�������
					static int nextId = 1;
					newEntry.id = std::to_wstring(nextId++);
					newEntry.createdDate = GetCurrentDateTime();
				}

				// ��ȡ������
				GetDlgItemText(hDlg, IDC_WEBSITE_EDIT, buffer, 1024);
				newEntry.website = buffer;

				GetDlgItemText(hDlg, IDC_USERNAME_EDIT, buffer, 1024);
				newEntry.username = buffer;

				GetDlgItemText(hDlg, IDC_PASSWORD_EDIT, buffer, 1024);
				newEntry.password = buffer;

				// ������Ŀ
				if (oldEntry) {
					int index = FindPasswordIndexById(newEntry.id);
					if (index != -1) {
						UpdatePasswordEntry(index, newEntry);
					}
				}
				else {
					AddPasswordEntry(newEntry);
				}

				DestroyWindow(hDlg);
				return 0;
			}

			case IDCANCEL:
				DestroyWindow(hDlg);
				return 0;

			case IDC_GENERATE_PASSWORD: {
				std::wstring password = GenerateRandomPassword(16);
				SetWindowText(GetDlgItem(hDlg, IDC_PASSWORD_EDIT), password.c_str());
				return 0;
			}
			}
			break;
		}

		case WM_CLOSE:
			DestroyWindow(hDlg);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hDlg, message, wParam, lParam);
		};
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = L"PasswordEntryDialogClass";

	RegisterClassEx(&wc);

	// �����Ի��򴰿�
	HWND hDlg = CreateWindowEx(
		0,
		L"PasswordEntryDialogClass",
		entry ? L"�༭������Ŀ" : L"���������",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 250,
		hWnd,
		NULL,
		hInst,
		const_cast<PasswordEntry*>(entry)  // ��entryָ�봫�ݸ�����
	);

	if (!hDlg) {
		MessageBox(hWnd, L"�����Ի���ʧ��", L"����", MB_ICONERROR);
		return;
	}

	// ���ô����û�����Ϊentryָ��
	SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(const_cast<PasswordEntry*>(entry)));

	// ��Ϣѭ��
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!IsDialogMessage(hDlg, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// ����
	UnregisterClass(L"PasswordEntryDialogClass", hInst);
}