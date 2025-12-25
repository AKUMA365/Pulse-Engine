// Pulse Engine
// File: logs
// Created on 25.12.2025
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#pragma once
#ifndef PULSEENGINE_LOGGING_H
#define PULSEENGINE_LOGGING_H
#include <string>

class Logs {
    public:
    void PE_INFO(std::string& text);
    void PE_WARN(std::string& text);
    void PE_ERROR(std::string& text);
    void PE_DEBUG(std::string& text);
    private:
};

#endif