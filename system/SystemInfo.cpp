// Pulse Engine
// File: SystemInfo.cpp
// © 2025. All rights reserved.

#include "SystemInfo.h"

#include <QSysInfo>
#include <QThread>
#include "../log/logs.h"
#include <fmt/format.h>
#include <filesystem>
#include <vector>
#include <fstream>
#include <array>

#if defined(_WIN32)
    #include <windows.h>
    #include <intrin.h>
#elif defined(__linux__) || defined(__APPLE__)
    #if defined(__i386__) || defined(__x86_64__)
        #include <cpuid.h>
    #endif

    #if defined(__linux__)
        #include <sys/sysinfo.h>
    #else
        #include <sys/types.h>
        #include <sys/sysctl.h>
    #endif
#endif

namespace fs = std::filesystem;

uint64_t SystemInfo::GetTotalRAM() {
#if defined(_WIN32)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys;
#elif defined(__linux__)
    struct sysinfo info;
    sysinfo(&info);
    return static_cast<uint64_t>(info.totalram) * info.mem_unit;
#elif defined(__APPLE__)
    int mib[2] = {CTL_HW, HW_MEMSIZE};
    uint64_t size = 0;
    size_t len = sizeof(size);
    sysctl(mib, 2, &size, &len, nullptr, 0);
    return size;
#endif
}

std::string SystemInfo::GetPlatform() {
    return QSysInfo::prettyProductName().toStdString();
}

std::string SystemInfo::GetOSVersion() {
    return QSysInfo::productVersion().toStdString();
}

uint64_t SystemInfo::GetCPUcore() {
    return static_cast<uint64_t>(QThread::idealThreadCount());
}

std::string SystemInfo::GetCPUName() {
#if defined(_WIN32)
    char cpuString[49] = {};
    int data[4];
    for (int i = 0; i < 3; ++i) {
        __cpuid(data, 0x80000002 + i);
        memcpy(cpuString + i * 16, data, 16);
    }
    return std::string(cpuString);
#elif defined(__linux__)
    std::ifstream file("/proc/cpuinfo");
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("model name") != std::string::npos) {
            size_t pos = line.find(": ");
            if (pos != std::string::npos) return line.substr(pos + 2);
        }
    }
    return "Unknown Processor";
#elif defined(__APPLE__)
    char buffer[256] = {};
    size_t bufferSize = sizeof(buffer);
    if (sysctlbyname("machdep.cpu.brand_string", &buffer, &bufferSize, nullptr, 0) == 0) {
        return std::string(buffer);
    }
    return "Apple Silicon / Unknown Mac CPU";
#endif
}

std::string SystemInfo::GetSupport() {
    bool sse = false, avx = false;

#if (defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64))
    std::array<unsigned int, 4> cpui{};
    #if defined(_WIN32)
        __cpuid(reinterpret_cast<int*>(cpui.data()), 1);
    #else
        __get_cpuid(1, &cpui[0], &cpui[1], &cpui[2], &cpui[3]);
    #endif
    sse = (cpui[3] & (1 << 25)) != 0;
    avx = (cpui[2] & (1 << 28)) != 0;
#elif defined(__arm64__) || defined(__aarch64__) || defined(_M_ARM64)
    // На ARM (Apple Silicon) SSE/AVX нет, там NEON.
    // Можно считать, что базовые SIMD всегда есть.
    return "ARM NEON support (Apple Silicon)";
#endif

    if(sse && avx) return "sse and avx support enabled";
    if(sse)        return "sse support enabled";
    if(avx)        return "avx support enabled";
    return "No x86 SIMD support (or non-x86)";
}

void SystemInfo::Detect() {
    m_Data.sseSupport = false;
    m_Data.avxSupport = false;

#if (defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64))
    std::array<unsigned int, 4> cpui{};
    #if defined(_WIN32)
        __cpuid(reinterpret_cast<int*>(cpui.data()), 1);
    #else
        __get_cpuid(1, &cpui[0], &cpui[1], &cpui[2], &cpui[3]);
    #endif
    m_Data.sseSupport = (cpui[3] & (1 << 25)) != 0;
    m_Data.avxSupport = (cpui[2] & (1 << 28)) != 0;
#endif

    m_Data.totalRAM = GetTotalRAM();
    m_Data.osName = GetPlatform();
    m_Data.osVersion = GetOSVersion();
    m_Data.cpuCores = GetCPUcore();
}

void SystemInfo::Print() {
    m_Logs = std::make_unique<Logs>();
    m_Logs->PE_INFO("OS: " + m_Data.osName + " " + m_Data.osVersion);
    m_Logs->PE_INFO("CPU Cores: " + std::to_string(m_Data.cpuCores));
    m_Logs->PE_INFO(GetSupport());

    double ramGB = static_cast<double>(m_Data.totalRAM) / (1024.0 * 1024.0 * 1024.0);
    m_Logs->PE_INFO(fmt::format("RAM: {:.2f} GB", ramGB));
}