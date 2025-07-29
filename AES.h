#pragma once

#define WIN32_LEAN_AND_MEAN  // ���ٲ���Ҫ��ͷ�ļ�����
#include <windows.h>
#include <wtypes.h>          // �����������Ͷ���
#include <wincrypt.h>
#include <string>

#pragma comment(lib, "crypt32.lib")

namespace AES {
	std::wstring EncryptString(const std::wstring& input, const std::wstring& key);
	std::wstring DecryptString(const std::wstring& input, const std::wstring& key);
}
