// Pulse Engine
// File: ${NAME}
// Created on ${DATE}
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#ifndef PULSEENGINE_CORE_H
#define PULSEENGINE_CORE_H


class app {
    public:
        void Run();
        void Stop();
        void Init();
        void shutdown();
    private:
        bool m_IsRunning = false;
};


#endif //PULSEENGINE_CORE_H