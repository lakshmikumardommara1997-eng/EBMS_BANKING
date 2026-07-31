#include "systemtime.h"
Banking::SystemTime::SystemTime() {}

std::string Banking::SystemTime::getCurrentTimeString() const {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm* localTime = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::time_t Banking::SystemTime::now()
{
    return std::time(nullptr);
}

std::tm Banking::SystemTime::toLocalTime(std::time_t current)
{
    std::tm tm{};

#ifdef _WIN32
    localtime_s(&tm, &current);
#else
    localtime_r(&current, &tm);
#endif

    return tm;
}

std::string Banking::SystemTime::formatTime(
    const std::tm& tm,
    const std::string& format)
{
    std::ostringstream oss;
    oss << std::put_time(&tm, format.c_str());
    return oss.str();
}

std::string Banking::SystemTime::currentDateTime(
    const std::string& format)
{
    return formatTime(toLocalTime(now()), format);
}
Banking::SystemTime::~SystemTime() {}