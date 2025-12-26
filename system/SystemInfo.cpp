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

#ifdef _WIN32
#include <intrin.h> // for __cpuid on Windows
#endif

#if defined(Q_OS_WIN)
#include <windows.h>
#elif defined(Q_OS_linux)
#include <sys/sysinfo.h>
#elif defined(Q_OS_MAC)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

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

void SystemInfo::Detect() {

    m_Data.osName = QSysInfo::prettyProductName().toStdString();
    m_Data.osVersion = QSysInfo::productVersion().toStdString();
    m_Data.cpuThreads = QThread::idealThreadCount();
    m_Data.cpuCores = QThread::idealThreadCount() /  2;

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
    std::string threads = std::to_string(m_Data.cpuThreads);
    std::string cores = std::to_string(m_Data.cpuCores);
    m_Logs->PE_INFO(cores + " - " + threads);

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
