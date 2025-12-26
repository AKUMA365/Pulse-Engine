// Pulse Engine
// File: ${NAME}
// Created on ${DATE}
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.
#define CORE_TRACE

#include "app.h"
#include "logginer.h"
#include  "../log/logs.h"
#include <memory>
#include "../system/SystemInfo.h"

void app::Run() {
    while (m_IsRunning) {

    }
}

void app::Init() {
    Log::Init();

    m_Logs = std::make_unique<Logs>();
    m_Logs->PE_INFO("Logs Initialized");

    SystemInfo SI;

    m_Logs->PE_INFO("Detected system info");
    SI.Detect();
    auto data = SI.GetData();
    SI.Print();


}
