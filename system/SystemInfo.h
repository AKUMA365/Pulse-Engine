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
#include <__filesystem/filesystem_error.h>

#include "../log/logs.h"

namespace fs = std::filesystem;

struct SystemInfoData {
    std::string osName;
    std::string osVersion;
    std::string Architecture;

    int cpuCores;
    std::string cpuName;

    bool sseSupport;
    bool avxSupport;

    size_t totalRAM = 0;

    std::string configPath;
    std::string homePath;
    std::string pulsePath;
    std::string tempPath;
    std::string executablePath;
    std::string cachePath;
    std::string savePath;

    bool Access;
};

class SystemInfo {
public:
    void Print();
    void Detect();
    void Platform();

    //Platform & OS
    static std::string GetPlatform();
    static std::string GetOSVersion();
    static std::string GetArchitecture();

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
    static fs::path GetCachePath();
    static fs::path GetTempPath();
    static fs::path GetHomePath();
    static fs::path GetExecutablePath();
    static fs::path GetPulsePath();
    static fs::path GetSavePath();
    static fs::path GetConfigPath();

    //Write access
    static bool HasWriteAccess(const std::string& path);

    // enum data
    const SystemInfoData& GetData() const { return m_Data; }
private:
    SystemInfoData m_Data;
    std::unique_ptr<Logs> m_Logs;
};

#endif
