#include "PasswordManager.h"

std::wstring GetCurrentDateTime() {
	SYSTEMTIME st;
	GetLocalTime(&st);

	wchar_t buffer[26];
	swprintf_s(buffer, L"%04d-%02d-%02d %02d:%02d:%02d",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond);

	return std::wstring(buffer);
}
int FindPasswordIndexById(const std::wstring& id) {
	for (size_t i = 0; i < passwordEntries.size(); ++i) {
		if (passwordEntries[i].id == id) {
			return static_cast<int>(i);
		}
	}
	return -1;
}

void UpdateStatusBar(const std::wstring& message) {
	SendMessage(hStatusBar, SB_SETTEXT, 0, (LPARAM)message.c_str());
}

void ShowAboutDialog(HWND hWnd) {
	MessageBox(hWnd,
		L"密码管理器 v1.0\n\n"
		L"这是一个简单的密码管理工具，使用Win32 API开发。\n"
		L"支持密码的添加、编辑、删除、复制和生成功能。\n"
		L"密码数据以JSON格式保存到本地文件。\n\n"
		L"? 2025 开发者",
		L"关于", MB_ICONINFORMATION);
}

void ShowSettingsDialog(HWND hWnd) {
	MessageBox(hWnd, L"设置功能尚未实现", L"提示", MB_ICONINFORMATION);
}

std::wstring Widen(const std::string& str) {
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, len);
	std::wstring result(wstr);
	delete[] wstr;
	return result;
}

std::string Narrow(const std::wstring& wstr) {
	int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	char* str = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, str, len, NULL, NULL);
	std::string result(str);
	delete[] str;
	return result;
}