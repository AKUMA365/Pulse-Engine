#include "SystemInfo.h"

#include <QThread>
#include "../log/logs.h"
#include <fmt/format.h>
#include <filesystem>
#include <fstream>
#include <array>
#include <QMessageBox>
#include <cstdlib>

#if defined(_WIN32)
    #include <windows.h>
    #include <intrin.h>
#elif defined(__linux__) || defined(__APPLE__)
    #if defined(__i386__) || defined(__x86_64__)
        #include <cpuid.h>
    #endif

    #if defined(__linux__)
        #include <sys/sysinfo.h>
    #else
        #include <sys/types.h>
        #include <sys/sysctl.h>
    #endif
#endif

namespace fs = std::filesystem;

uint64_t SystemInfo::GetHighResFrequency()
{
    return 1000000000ULL;
}

uint64_t SystemInfo::GetHighResCounter()
{
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

uint64_t SystemInfo::GetTotalRAM() {
#if defined(_WIN32)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys;
#elif defined(__linux__)
    struct sysinfo info;
    sysinfo(&info);
    return static_cast<uint64_t>(info.totalram) * info.mem_unit;
#elif defined(__APPLE__)
    int mib[2] = {CTL_HW, HW_MEMSIZE};
    uint64_t size = 0;
    size_t len = sizeof(size);
    sysctl(mib, 2, &size, &len, nullptr, 0);
    return size;
#endif
}

std::string SystemInfo::GetPlatform() {
    return QSysInfo::prettyProductName().toStdString();
}

std::string SystemInfo::GetOSVersion() {
    return QSysInfo::productVersion().toStdString();
}

std::string SystemInfo::GetArchitecture()
{
    std::string arch;
#if defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64)
    arch = "x86";
#elif defined(__arm64__) || defined(__aarch64__) || defined(_M_ARM64)
    arch = "arm64";
#endif
    return arch;
}

uint64_t SystemInfo::GetCPUcore() {
    return static_cast<uint64_t>(QThread::idealThreadCount());
}

std::string SystemInfo::GetCPUName() {
#if defined(_WIN32)
    char cpuString[49] = {};
    int data[4];
    for (int i = 0; i < 3; ++i) {
        __cpuid(data, 0x80000002 + i);
        memcpy(cpuString + i * 16, data, 16);
    }
    return std::string(cpuString);
#elif defined(__linux__)
    std::ifstream file("/proc/cpuinfo");
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("model name") != std::string::npos) {
            size_t pos = line.find(": ");
            if (pos != std::string::npos) return line.substr(pos + 2);
        }
    }
    return "Unknown Processor";
#elif defined(__APPLE__)
    char buffer[256] = {};
    size_t bufferSize = sizeof(buffer);
    if (sysctlbyname("machdep.cpu.brand_string", &buffer, &bufferSize, nullptr, 0) == 0) {
        return std::string(buffer);
    }
    return "Apple Silicon / Unknown Mac CPU";
#endif
}

std::string SystemInfo::GetSupport() {
    bool sse = false, avx = false;

#if (defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64))
    std::array<unsigned int, 4> cpui{};
    #if defined(_WIN32)
        __cpuid(reinterpret_cast<int*>(cpui.data()), 1);
    #else
        __get_cpuid(1, &cpui[0], &cpui[1], &cpui[2], &cpui[3]);
    #endif
    sse = (cpui[3] & (1 << 25)) != 0;
    avx = (cpui[2] & (1 << 28)) != 0;
#elif defined(__arm64__) || defined(__aarch64__) || defined(_M_ARM64)
    return "ARM NEON support (Apple Silicon)";
#endif

    if(sse && avx) return "sse and avx support enabled";
    if(sse)        return "sse support enabled";
    if(avx)        return "avx support enabled";

    return "No x86 SIMD support (or non-x86)";
}

fs::path SystemInfo::GetHomePath()
{
    fs::path home;
    const char* env_p = nullptr;

#ifdef _WIN32
    env_p = std::getenv("USERPROFILE");
#else
    env_p = std::getenv("HOME");
#endif

    if (env_p) {
        home = fs::path(env_p);
    } else {
        home = fs::current_path();
    }

    return home;
}

fs::path SystemInfo::GetTempPath()
{
    return fs::temp_directory_path();
}

fs::path SystemInfo::GetConfigPath()
{
    fs::path configPath = SystemInfo::GetPulsePath() / "config";

    if (fs::exists(configPath) && !fs::is_directory(configPath))
    {
        fmt::print("Error: {} exists but is a file\n", configPath.string());
        return {};
    }

    if (!fs::exists(configPath))
    {
        try {
            fs::create_directories(configPath);
        } catch (const fs::filesystem_error& e) {
            fmt::print("Error creating config dir: {}\n", e.what());
            return {};
        }
    }

    return configPath;
}

fs::path SystemInfo::GetSavePath()
{
    fs::path savePath = SystemInfo::GetPulsePath() / "project";

    if (fs::exists(savePath) && !fs::is_directory(savePath))
    {
        fmt::print("Error: {} exists but is a file!\n", savePath.string());
        return {};
    }

    if (!fs::exists(savePath))
    {
        try
        {
            fs::create_directories(savePath);
        }
        catch (const fs::filesystem_error& e)
        {
            fmt::print("Error: {}\n", e.what());
            return {};
        }
    }

    return savePath;
}

fs::path SystemInfo::GetCachePath()
{
    fs::path cachePath = SystemInfo::GetPulsePath() / "cache";

    if (fs::exists(cachePath) && !fs::is_directory(cachePath))
    {
        fmt::print("Error: {} exists but is a file!\n", cachePath.string());
        return {};
    }

    if (!fs::exists(cachePath))
    {
        try
        {
            fs::create_directories(cachePath);
        }
        catch (const fs::filesystem_error& e)
        {
            fmt::print("Error: {}\n", e.what());
            return {};
        }
    }

    return cachePath;
}

fs::path SystemInfo::GetPulsePath()
{
    fs::path pulsePath = SystemInfo::GetHomePath() / "pulse";

    if (fs::exists(pulsePath) && !fs::is_directory(pulsePath))
    {
        fmt::print("Error: {} exists but is a file!\n", pulsePath.string());
        return {};
    }

    if (!fs::exists(pulsePath))
    {
        try
        {
            fs::create_directories(pulsePath);
        }
        catch (const fs::filesystem_error& e)
        {
            fmt::print("Error: {}\n", e.what());
            return {};
        }
    }

    return pulsePath;
}

bool SystemInfo::HasWriteAccess(const std::string& path) {
    if (path.empty()) return false;

    fs::path testPath = fs::path(path) / "pulse_permission_test";
    try {
        std::ofstream file(testPath);
        if (file.is_open()) {
            file.close();
            fs::remove(testPath);
            return true;
        }
    } catch (...) {
        return false;
    }
    return false;
}

void SystemInfo::Platform()
{
    if (!m_Logs) {
        m_Logs = std::make_unique<Logs>();
    }

    m_Data.osName = GetPlatform();
    m_Data.osVersion = GetOSVersion();
    m_Data.Architecture = GetArchitecture();

    // ПРЕОБРАЗОВАНИЕ В СТРОКУ ЗДЕСЬ (path -> string)
    m_Data.homePath = GetHomePath().string();
    m_Data.pulsePath = GetPulsePath().string();

    m_Data.Access = HasWriteAccess(m_Data.homePath);

    if (m_Data.Access == false)
    {
        m_Logs->PE_ERROR("Error write access to Home Directory: " + m_Data.homePath);
    }

    // ПРЕОБРАЗОВАНИЕ В СТРОКУ ЗДЕСЬ
    m_Data.tempPath = GetTempPath().string();
    m_Data.savePath = GetSavePath().string();
    m_Data.configPath = GetConfigPath().string();
    m_Data.cachePath = GetCachePath().string();
}

void SystemInfo::Detect() {
    m_Data.sseSupport = false;
    m_Data.avxSupport = false;

#if (defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64))
    std::array<unsigned int, 4> cpui{};
    #if defined(_WIN32)
        __cpuid(reinterpret_cast<int*>(cpui.data()), 1);
    #else
        __get_cpuid(1, &cpui[0], &cpui[1], &cpui[2], &cpui[3]);
    #endif
    m_Data.sseSupport = (cpui[3] & (1 << 25)) != 0;
    m_Data.avxSupport = (cpui[2] & (1 << 28)) != 0;
#endif

    m_Data.totalRAM = GetTotalRAM();
    m_Data.osName = GetPlatform();
    m_Data.osVersion = GetOSVersion();
    m_Data.cpuCores = GetCPUcore();
}

void SystemInfo::Print() {
    if (!m_Logs) {
        m_Logs = std::make_unique<Logs>();
    }

    m_Logs->PE_INFO("OS: " + m_Data.osName + " " + m_Data.osVersion);
    m_Logs->PE_INFO("CPU Cores: " + std::to_string(m_Data.cpuCores));
    m_Logs->PE_INFO(GetSupport());

    m_Logs->PE_INFO("Home Path: " + m_Data.homePath);
    m_Logs->PE_INFO("Pulse Path: " + m_Data.pulsePath);
    m_Logs->PE_INFO("Temp Path: " + m_Data.tempPath);
    m_Logs->PE_INFO("Save Path: " + m_Data.savePath);
    m_Logs->PE_INFO("Cache Path: " + m_Data.cachePath);
    m_Logs->PE_INFO("Config Path: " + m_Data.configPath);
    m_Logs->PE_INFO("Architecture: " + m_Data.Architecture);

    double ramGB = static_cast<double>(m_Data.totalRAM) / (1024.0 * 1024.0 * 1024.0);
    m_Logs->PE_INFO(fmt::format("RAM: {:.2f} GB", ramGB));
}