// Pulse Engine
// File: ${NAME}
// Created on ${DATE}
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#include "main_loop.h"

#include "../system/SystemInfo.h"

void main_loop::Run()
{
    uint64_t Frequency = SystemInfo::GetHighResFrequency();
    uint64_t lastTime = SystemInfo::GetHighResCounter();

    while (m_IsRunning)
    {
        uint64_t currentTime = SystemInfo::GetHighResCounter();

        uint64_t deltaTicks = currentTime - lastTime;

        double deltaTime = static_cast<double>(deltaTicks) / static_cast<double>(Frequency);

        lastTime = currentTime;

    }

}

