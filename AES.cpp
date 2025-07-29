#include "AES.h"

// 加密函数
std::wstring AES::EncryptString(const std::wstring& input, const std::wstring& key) {
    HCRYPTPROV hProv;
    HCRYPTHASH hHash;
    HCRYPTKEY hKey;

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        return L"";
    }

    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        CryptReleaseContext(hProv, 0);
        return L"";
    }

    if (!CryptHashData(hHash, (BYTE*)key.c_str(), key.size() * sizeof(wchar_t), 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return L"";
    }

    if (!CryptDeriveKey(hProv, CALG_AES_256, hHash, 0, &hKey)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return L"";
    }

    DWORD dwLength = input.size() * sizeof(wchar_t);
    DWORD dwBufferLen = dwLength;

    // 加密数据需要额外的空间
    if (!CryptEncrypt(hKey, 0, TRUE, 0, NULL, &dwBufferLen, 0)) {
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return L"";
    }

    BYTE* pbBuffer = (BYTE*)malloc(dwBufferLen);
    memcpy(pbBuffer, input.c_str(), dwLength);

    if (!CryptEncrypt(hKey, 0, TRUE, 0, pbBuffer, &dwLength, dwBufferLen)) {
        free(pbBuffer);
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return L"";
    }

    // 转换为Base64方便存储
    DWORD dwBase64Len = 0;
    CryptBinaryToString(pbBuffer, dwLength, CRYPT_STRING_BASE64, NULL, &dwBase64Len);

    wchar_t* pszBase64 = (wchar_t*)malloc(dwBase64Len * sizeof(wchar_t));
    CryptBinaryToString(pbBuffer, dwLength, CRYPT_STRING_BASE64, pszBase64, &dwBase64Len);

    std::wstring result(pszBase64);

    free(pbBuffer);
    free(pszBase64);
    CryptDestroyKey(hKey);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return result;
}

// 解密函数
std::wstring AES::DecryptString(const std::wstring& input, const std::wstring& key) {
    HCRYPTPROV hProv;
    HCRYPTHASH hHash;
    HCRYPTKEY hKey;

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        return L"";
    }

    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        CryptReleaseContext(hProv, 0);
        return L"";
    }

    if (!CryptHashData(hHash, (BYTE*)key.c_str(), key.size() * sizeof(wchar_t), 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return L"";
    }

    if (!CryptDeriveKey(hProv, CALG_AES_256, hHash, 0, &hKey)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return L"";
    }

    // 从Base64转换回二进制
    DWORD dwBinaryLen = 0;
    CryptStringToBinary(input.c_str(), input.size(), CRYPT_STRING_BASE64, NULL, &dwBinaryLen, NULL, NULL);

    BYTE* pbBinary = (BYTE*)malloc(dwBinaryLen);
    CryptStringToBinary(input.c_str(), input.size(), CRYPT_STRING_BASE64, pbBinary, &dwBinaryLen, NULL, NULL);

    if (!CryptDecrypt(hKey, 0, TRUE, 0, pbBinary, &dwBinaryLen)) {
        free(pbBinary);
        CryptDestroyKey(hKey);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return L"";
    }

    std::wstring result((wchar_t*)pbBinary, dwBinaryLen / sizeof(wchar_t));

    free(pbBinary);
    CryptDestroyKey(hKey);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return result;
}