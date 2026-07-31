#ifndef LOGLEVEL_H
#define LOGLEVEL_H

namespace Banking
{
    enum class LogLevel
    {
        DEBUG = 0,
        INFO = 1,
        WARN = 2,
        ERROR = 3,
        NONE = 4 // Represents no logging
    };
    inline std::string logLevelToString(LogLevel level)
    {
        switch (level)
        {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARN: return "WARN";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::NONE: return "NONE";
            default: return "UNKNOWN";
        }
    }
    inline LogLevel stringToLogLevel(const std::string& levelStr)
    {
        if (levelStr == "DEBUG") return LogLevel::DEBUG;
        if (levelStr == "INFO") return LogLevel::INFO;
        if (levelStr == "WARN") return LogLevel::WARN;
        if (levelStr == "ERROR") return LogLevel::ERROR;
        if (levelStr == "NONE") return LogLevel::NONE;
        return LogLevel::NONE; // Default to NONE for unrecognized strings
    }
}
#endif // LOGLEVEL_H