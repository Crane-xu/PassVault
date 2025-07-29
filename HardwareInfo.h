#include <string>
#include <windows.h>
#include <intrin.h>
#include <iphlpapi.h>
#include <string>

#pragma once
#pragma comment(lib, "iphlpapi.lib")

std::wstring GenerateHardwareKey();