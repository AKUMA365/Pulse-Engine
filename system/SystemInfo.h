// Pulse Engine
// File: platform
// Created on 24.12.2025
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#ifndef PULSEENGINE_PLATFORM_H
#define PULSEENGINE_PLATFORM_H
#include <memory>
#include <string>
#include "../log/logs.h"

struct SystemInfoData {
    std::string osName;
    std::string osVersion;
    std::string Architecture;
    bool Is64BitSystem;

    int cpuCores;
    std::string cpuName;

    bool sseSupport;
    bool avxSupport;

    size_t totalRAM = 0;

    std::string configPath;
    std::string homePath;
    std::string tempPath;
    std::string executablePath;
    std::string cachePath;
    std::string savePath;
};

class SystemInfo {
public:
    void Print();
    void Detect();
    void Platform();

    //Platform & OS
    static std::string GetPlatform();
    static std::string GetOSVersion();

    //hardware
    static uint64_t GetTotalRAM();
    static uint64_t GetCPUcore();
    static std::string GetCPUName();
    //static std::string GetGPUName();

    // avx/sse check
    static std::string GetSupport();

    //High-res timer
    static uint64_t GetHighResFrequency();
    static uint64_t GetHighResCounter();

    //check path
    static std::string GetCachePath();
    static std::string GetTempPath();
    static std::string GetHomePath();
    static std::string GetExecutablePath();
    static std::string GetSavePath();
    static std::string GetConfigPath();

    //Write access
    static bool HasWriteAccess(const std::string& path);

    // enum data
    const SystemInfoData& GetData() const { return m_Data; }
private:
    SystemInfoData m_Data;
    std::unique_ptr<Logs> m_Logs;
};

#endif
