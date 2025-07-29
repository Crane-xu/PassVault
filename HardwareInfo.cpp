#include "HardwareInfo.h"

// 获取CPU序列号
std::wstring GetCPUSerial() {
    int CPUInfo[4] = { -1 };
    __cpuid(CPUInfo, 1);
    wchar_t buffer[64];
    swprintf(buffer, 64, L"%08X%08X", CPUInfo[3], CPUInfo[0]);
    return buffer;
}

// 获取硬盘序列号
std::wstring GetDiskSerial() {
    wchar_t volumeName[MAX_PATH + 1] = { 0 };
    wchar_t fileSystemName[MAX_PATH + 1] = { 0 };
    DWORD serialNumber = 0;
    DWORD maxComponentLen = 0;
    DWORD fileSystemFlags = 0;

    if (GetVolumeInformation(
        L"C:\\",
        volumeName,
        ARRAYSIZE(volumeName),
        &serialNumber,
        &maxComponentLen,
        &fileSystemFlags,
        fileSystemName,
        ARRAYSIZE(fileSystemName))) {
        wchar_t buffer[64];
        swprintf(buffer, 64, L"%08X", serialNumber);
        return buffer;
    }
    return L"";
}

// 获取MAC地址
std::wstring GetMACAddress() {
    IP_ADAPTER_INFO adapterInfo[16];
    DWORD dwBufLen = sizeof(adapterInfo);

    if (GetAdaptersInfo(adapterInfo, &dwBufLen) == ERROR_SUCCESS) {
        PIP_ADAPTER_INFO pAdapterInfo = adapterInfo;
        if (pAdapterInfo) {
            wchar_t buffer[64];
            swprintf(buffer, 64, L"%02X%02X%02X%02X%02X%02X",
                pAdapterInfo->Address[0], pAdapterInfo->Address[1],
                pAdapterInfo->Address[2], pAdapterInfo->Address[3],
                pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
            return buffer;
        }
    }
    return L"";
}

// 生成硬件密钥
std::wstring GenerateHardwareKey() {
    std::wstring key = GetCPUSerial() + GetDiskSerial() + GetMACAddress();
    // 可以在这里添加更多的硬件信息
    return key;
}