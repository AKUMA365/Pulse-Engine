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
    int cpuCores;
    int cpuThreads;
    bool sseSupport;
    bool avxSupport;
    size_t totalRAM = 0;
};

class SystemInfo {
    public:
        void Detect();
        void Print();
        static uint64_t GetTotalRAM();

        const SystemInfoData& GetData() const { return m_Data; }
    private:
        SystemInfoData m_Data;
        std::unique_ptr<Logs> m_Logs;
};

#endif
