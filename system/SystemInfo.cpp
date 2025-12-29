// Pulse Engine
// File: platform
// Created on 24.12.2025
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#include "SystemInfo.h"
#include "QSysInfo"
#include <QThread>
#include  "../log/logs.h"
#include  <fmt/format.h>
#include <filesystem>

#if defined(Q_OS_WIN)
#include <windows.h>
#include <intrin.h> // for __cpuid on Windows
#elif defined(Q_OS_linux)
#include <sys/sysinfo.h>
#elif defined(Q_OS_MAC)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

namespace fs = std::filesystem;

uint64_t SystemInfo::GetTotalRAM() {
    #if defined(Q_OS_WIN)
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status.ullTotalPhys;
    #elif defined(Q_OS_LINUX)
        struct sysinfo info;
        sysinfo(&info);
        return static_cast<uint64_t>(info.totalram) * info.mem_unit;
    #elif defined(Q_OS_MAC)
        int mib[2] = {CTL_HW, HW_MEMSIZE};
        uint64_t size = 0;
        size_t len = sizeof(size);
        sysctl(mib, 2, &size, &len, nullptr, 0);
        return size;
    #endif
}

std::string SystemInfo::GetPlatform() {
    std::string platform = QSysInfo::prettyProductName().toStdString();
    return platform;
}

std::string SystemInfo::GetOSVersion() {
    std::string osVersion = QSysInfo::productVersion().toStdString();
    return osVersion;
}

uint64_t SystemInfo::GetCPUcore() {
    uint64_t cpucore = QThread::idealThreadCount();
}

std::string SystemInfo::GetCPUName() {
#ifdef defined Q_OS_WIN
    char cpuString[49];
    std::vector<int> data(4);

    for (int i = 0; i < 3; ++i) {
        __cpuid(data.data(), 0x80000002 + i);
        for (int j = 0; j < 4; ++j) {
            // Копируем 4 байта из data[j]
            cpuString[i * 4 + j] = (char)(data[j] & 0xFF);
            cpuString[i * 4 + j + 4] = (char)((data[j] >> 8) & 0xFF);
            cpuString[i * 4 + j + 8] = (char)((data[j] >> 16) & 0xFF);
            cpuString[i * 4 + j + 12] = (char)((data[j] >> 24) & 0xFF);
        }
    }
    cpuString[48] = '\0';
    return std::string(cpuString);
#elif defined(Q_OS_LINUX)
    std::ifstream file("/proc/cpuinfo");
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("model name") != std::string::npos) {
            size_t pos = line.find(": ");
            if (pos != std::string::npos) {
                return line.substr(pos + 2);
            }
        }
    }
    return "Unknown Processor";
#elif defined(Q_OS_MAC)
    char buffer[256];
    size_t bufferSize = sizeof(buffer);

    if (sysctlbyname("machdep.cpu.brand_string", &buffer, &bufferSize, NULL, 0) == 0) {
        return std::string(buffer);
    }

    if (sysctlbyname("hw.model", &buffer, &bufferSize, NULL, 0) == 0) {
        return std::string(buffer);
    }

    return "Unknown Mac CPU";
#endif
}

std::string SystemInfo::GetSupport() {
    std::array<int, 4> cpui;

#ifdef _WIN32
    __cpuid(cpui.data(), 1);
#else
    __get_cpuid(1, &cpui[0], &cpui[1], &cpui[2], &cpui[3]);
#endif

    bool sse = (cpui[3] & (1 << 25)) != 0;

    bool avx = (cpui[2] & (1 << 28)) != 0;

#ifdef _WIN32
    __cpuidex(cpui.data(), 7, 0);
#else
    __get_cpuid_count(7, 0, &cpui[0], &cpui[1], &cpui[2], &cpui[3]);
#endif

    std::string support;

    if (sse == true || avx == true) {
        support.append("sse and avx support enabled");
    } else if (sse == true || avx == false) {
        support.append("sse support enabled");
    } else if (sse == false || avx == true) {
        support.append("avx support enabled");
    } else {
        support.append("Unknown supper");
    }

    return support;
}

uint64_t SystemInfo::GetHighResFrequency() {

}

uint64_t SystemInfo::GetHighResCounter() {

}

std::string SystemInfo::GetHomePath() {
    std::string HomeDir;

}

std::string SystemInfo::GetCachePath() {

}

void SystemInfo::Platform() {

}


void SystemInfo::Detect() {

    std::array<int, 4> cpui;

    #ifdef _WIN32
        __cpuid(cpui.data(), 1);
    #else
        __get_cpuid(1, &cpui[0], &cpui[1], &cpui[2], &cpui[3]);
    #endif

    m_Data.sseSupport = (cpui[3] & (1 << 25)) != 0;

    m_Data.avxSupport = (cpui[2] & (1 << 28)) != 0;

    #ifdef _WIN32
        __cpuidex(cpui.data(), 7, 0);
    #else
        __get_cpuid_count(7, 0, &cpui[0], &cpui[1], &cpui[2], &cpui[3]);
    #endif

    m_Data.totalRAM = SystemInfo::GetTotalRAM();
}

void SystemInfo::Print() {

    m_Logs = std::make_unique<Logs>();

    m_Logs->PE_INFO(m_Data.osName);
    m_Logs->PE_INFO(m_Data.osVersion);

    std::string cores = std::to_string(m_Data.cpuCores);
    m_Logs->PE_INFO(cores);

    if (m_Data.sseSupport == true) {
        m_Logs->PE_INFO("sse support enabled");
    }
    if (m_Data.avxSupport == true); {
        m_Logs->PE_INFO("avx support enabled");
    }

    std::string RAM = std::to_string(m_Data.totalRAM);

    double ramGB =  static_cast<double>(m_Data.totalRAM) / 1024.0 / 1024.0 / 1024.0;

    m_Logs->PE_INFO(fmt::format("RAM: {:.2f} GB", ramGB));
}
