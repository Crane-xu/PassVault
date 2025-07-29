#pragma once

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <shellapi.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <shlobj.h> 
#include "ControlsID.h"
#include "HardwareInfo.h"
#include "AES.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")

using json = nlohmann::json;

// 密码条目结构
struct PasswordEntry {
	std::wstring id;
	std::wstring website;
	std::wstring username;
	std::wstring password;
	std::wstring createdDate;
};

// 全局变量
extern HWND hMainWindow, hListView, hStatusBar;
extern HWND hAddBtn, hDeleteBtn, hCopyBtn, hGenerateBtn, hUnameBtn;
extern HMENU hMenu;
extern std::vector<PasswordEntry> passwordEntries;
extern std::wstring currentFilePath;
extern HINSTANCE hInst;
extern HFONT hFont, hBoldFont;

// 保存原始窗口过程
extern WNDPROC g_originalListViewProc;
// 函数声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void InitListView(HWND hWnd);
void LoadPasswordsFromFile(const std::wstring& filePath);
void ImportPasswordsFromFile(const std::wstring& filePath);
void SavePasswordsToFile(const std::wstring& filePath);
void ExportPasswordsToFile(const std::wstring& filePath);
void AddPasswordEntry(const PasswordEntry& entry);
void UpdatePasswordEntry(int index, const PasswordEntry& entry);
void DeletePasswordEntry(int index);
void CopyPasswordToClipboard(int index);
void CopyUnameToClipboard(int index);
std::wstring GenerateRandomPassword(int length = 16);
std::wstring GetCurrentDateTime();
int FindPasswordIndexById(const std::wstring& id);
void UpdateStatusBar(const std::wstring& message);
void ShowPasswordEntryDialog(HWND hWnd, const PasswordEntry* entry = nullptr);
void ShowAboutDialog(HWND hWnd);
void ShowSettingsDialog(HWND hWnd);
std::wstring Widen(const std::string& str);
std::string Narrow(const std::wstring& wstr);
void SetupListView(HWND hListView);
LRESULT CALLBACK ListViewSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);