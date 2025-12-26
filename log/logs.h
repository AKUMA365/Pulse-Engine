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
#include <memory>

class Logs {
    public:
    void PE_INFO(const std::string& text);
    void PE_WARN(const std::string& text);
    void PE_ERROR(const std::string& text);
    void PE_DEBUG(const std::string& text);
    private:
};

#endif