// Pulse Engine
// File: ${NAME}
// Created on ${DATE}
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#ifndef PULSEENGINE_MAIN_LOOP_H
#define PULSEENGINE_MAIN_LOOP_H
#include <atomic>
#include <cstdint>


class main_loop {
    public:
        void Run();
        void stop() { m_IsRunning = false; }
    private:
        uint64_t Frequency;
        uint64_t lastTime;
        std::atomic<bool> m_IsRunning{true};
};


#endif //PULSEENGINE_MAIN_LOOP_H