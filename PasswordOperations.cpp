#include "PasswordManager.h"

void AddPasswordEntry(const PasswordEntry& entry) {
	passwordEntries.push_back(entry);

	// 添加到ListView
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = ListView_GetItemCount(hListView);
	lvItem.iSubItem = 0;
	lvItem.pszText = const_cast<wchar_t*>(entry.website.c_str());
	ListView_InsertItem(hListView, &lvItem);

	ListView_SetItemText(hListView, lvItem.iItem, 1, const_cast<wchar_t*>(entry.username.c_str()));
	ListView_SetItemText(hListView, lvItem.iItem, 2, const_cast<wchar_t*>(entry.createdDate.c_str()));

	// 保存到文件
	if (!currentFilePath.empty()) {
		SavePasswordsToFile(currentFilePath);
	}
}

void UpdatePasswordEntry(int index, const PasswordEntry& entry) {
	if (index >= 0 && index < passwordEntries.size()) {
		passwordEntries[index] = entry;

		// 更新ListView
		ListView_SetItemText(hListView, index, 0, const_cast<wchar_t*>(entry.website.c_str()));
		ListView_SetItemText(hListView, index, 1, const_cast<wchar_t*>(entry.username.c_str()));
		ListView_SetItemText(hListView, index, 2, const_cast<wchar_t*>(entry.createdDate.c_str()));

		// 保存到文件
		if (!currentFilePath.empty()) {
			SavePasswordsToFile(currentFilePath);
		}
	}
}

void DeletePasswordEntry(int index) {
	if (index >= 0 && index < passwordEntries.size()) {
		passwordEntries.erase(passwordEntries.begin() + index);
		ListView_DeleteItem(hListView, index);

		// 保存到文件
		if (!currentFilePath.empty()) {
			SavePasswordsToFile(currentFilePath);
		}
	}
}

void CopyPasswordToClipboard(int index) {
	if (index >= 0 && index < passwordEntries.size()) {
		if (OpenClipboard(hMainWindow)) {
			EmptyClipboard();
			const std::wstring& password = passwordEntries[index].password;
			HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, (password.length() + 1) * sizeof(wchar_t));
			wchar_t* pchData = (wchar_t*)GlobalLock(hClipboardData);
			wcscpy_s(pchData, password.length() + 1, password.c_str());
			GlobalUnlock(hClipboardData);
			SetClipboardData(CF_UNICODETEXT, hClipboardData);
			CloseClipboard();
		}
	}
}

void CopyUnameToClipboard(int index) {
	if (index >= 0 && index < passwordEntries.size()) {
		if (OpenClipboard(hMainWindow)) {
			EmptyClipboard();
			const std::wstring& uanme = passwordEntries[index].username;
			HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, (uanme.length() + 1) * sizeof(wchar_t));
			wchar_t* pchData = (wchar_t*)GlobalLock(hClipboardData);
			wcscpy_s(pchData, uanme.length() + 1, uanme.c_str());
			GlobalUnlock(hClipboardData);
			SetClipboardData(CF_UNICODETEXT, hClipboardData);
			CloseClipboard();
		}
	}
}

std::wstring GenerateRandomPassword(int length) {
	static const wchar_t charset[] =
		L"0123456789"
		L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		L"abcdefghijklmnopqrstuvwxyz"
		L"!@#$%^&*()_+-=[]{}|;':\",./<>?";

	std::wstring password;
	password.reserve(length);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, sizeof(charset) / sizeof(wchar_t) - 2);

	for (int i = 0; i < length; ++i) {
		password += charset[dis(gen)];
	}

	return password;
}
