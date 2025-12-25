// Pulse Engine
// File: platform
// Created on 24.12.2025
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#ifndef PULSEENGINE_PLATFORM_H
#define PULSEENGINE_PLATFORM_H
#include <string>

struct SystemInfoData {
    std::string osName;
    std::string osVersion;
    int cpuCores;
    int cpuThreads;
    bool sseSupport;
    bool avxSupport;
    size_t totalRAM;
};

class SystemInfo {
    public:
        void Detect();
        void Print();
};

#endif
