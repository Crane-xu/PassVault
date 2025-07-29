#pragma once

#define WIN32_LEAN_AND_MEAN  // 减少不必要的头文件包含
#include <windows.h>
#include <wtypes.h>          // 包含基本类型定义
#include <wincrypt.h>
#include <string>

#pragma comment(lib, "crypt32.lib")

namespace AES {
	std::wstring EncryptString(const std::wstring& input, const std::wstring& key);
	std::wstring DecryptString(const std::wstring& input, const std::wstring& key);
}
