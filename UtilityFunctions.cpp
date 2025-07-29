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
		L"��������� v1.0\n\n"
		L"����һ���򵥵���������ߣ�ʹ��Win32 API������\n"
		L"֧���������ӡ��༭��ɾ�������ƺ����ɹ��ܡ�\n"
		L"����������JSON��ʽ���浽�����ļ���\n\n"
		L"? 2025 ������",
		L"����", MB_ICONINFORMATION);
}

void ShowSettingsDialog(HWND hWnd) {
	MessageBox(hWnd, L"���ù�����δʵ��", L"��ʾ", MB_ICONINFORMATION);
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