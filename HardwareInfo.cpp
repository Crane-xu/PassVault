#include "HardwareInfo.h"

// ��ȡCPU���к�
std::wstring GetCPUSerial() {
    int CPUInfo[4] = { -1 };
    __cpuid(CPUInfo, 1);
    wchar_t buffer[64];
    swprintf(buffer, 64, L"%08X%08X", CPUInfo[3], CPUInfo[0]);
    return buffer;
}

// ��ȡӲ�����к�
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

// ��ȡMAC��ַ
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

// ����Ӳ����Կ
std::wstring GenerateHardwareKey() {
    std::wstring key = GetCPUSerial() + GetDiskSerial() + GetMACAddress();
    // ������������Ӹ����Ӳ����Ϣ
    return key;
}