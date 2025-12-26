// Pulse Engine
// File: logs
// Created on 25.12.2025
// Author: Dima Semchenko & AKUMA365
// © 2025. All rights reserved.
// Proprietary software. Academic evaluation only.

#include "logs.h"
#include "spdlog/spdlog.h"
#include "../EngineCore/logginer.h"

void Logs::PE_INFO(const std::string &text) {
    Log::GetCoreLogger()->info("{}", text);
}

void Logs::PE_WARN(const std::string &text) {
    Log::GetCoreLogger()->warn("{}", text);
}

void Logs::PE_ERROR(const std::string &text) {
    Log::GetCoreLogger()->error("{}", text);
}

void Logs::PE_DEBUG(const std::string &text) {
    Log::GetCoreLogger()->debug("{}", text);
}