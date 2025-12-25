// Pulse Engine
// File: platform
// Created on 24.12.2025
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#include "SystemInfo.h"
#include "QSysInfo"
#include <QThread>

void SystemInfo::Detect() {
    SystemInfoData SID;

    SID.osName = QSysInfo::prettyProductName().toStdString();
    SID.osVersion = QSysInfo::productVersion().toStdString();
    SID.cpuThreads = QThread::idealThreadCount();
    SID.cpuCores = QThread::idealThreadCount() /  2;
}

void SystemInfo::Print() {
    
}
