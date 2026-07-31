#include "Logger.h"
#include <iostream>
#include <fstream>
#include "Logwritter.h"


Banking::Logger& Banking::Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Banking::Logger::log(const std::string& message,const std::string& file, int line, const std::string& function, Banking::LogLevel level)
{
    const auto& timestamp = systemTime.currentDateTime(appConfig.getSysDateTimeFormat());
    std::string formattedmessage = "[" + timestamp + "]" + "[" + logLevelToString(level) + "] " + message + " (" + file + ":" + std::to_string(line) + " in " + function + ")";
    if (appConfig.isLoggingDisabled() || (!shouldLog(level))) {
        return; // Skip logging if disabled
    }
    if (appConfig.isLogToConsole()) {
        std::cout << formattedmessage << std::endl;
    }
    if (appConfig.isLogToFile()) {
        std::lock_guard<std::mutex> lock(loggerMutex); // Lock the mutex to ensure thread safety
        logWriter.write(formattedmessage);

        // Unlock the mutex after writing to the log file
    }
}

void Banking::Logger::info(const std::string& message,const std::string& file, int line, const std::string& function)
{
   log(message, file, line, function, Banking::LogLevel::INFO);
   
}
void Banking::Logger::warn(const std::string& message,const std::string& file, int line, const std::string& function)
{
    log(message, file, line, function, Banking::LogLevel::WARN);
}
void Banking::Logger::error(const std::string& message,const std::string& file, int line, const std::string& function)
{
    log(message, file, line, function, Banking::LogLevel::ERROR);
}
void Banking::Logger::debug(const std::string& message,const std::string& file, int line, const std::string& function)
{
    log(message, file, line, function, Banking::LogLevel::DEBUG);
}
void Banking::Logger::init()
{
    
    if (appConfig.isLoggingDisabled()) {
        return; // Skip initialization if logging is disabled
    }
   logWriter.makedirectory(appConfig.getLogFilePath().substr(0, appConfig.getLogFilePath().find_last_of("/")));
   if (!logWriter.open(appConfig.getLogFilePath())) {
        std::cerr << "Failed to open log file: " << appConfig.getLogFilePath() << std::endl;
    }
}

bool Banking::Logger::shouldLog(Banking::LogLevel messageLevel) const
{
    auto configuredLevel = appConfig.getLogLevel();

    if (configuredLevel == Banking::LogLevel::NONE)
        return false;

    return messageLevel >= configuredLevel;
}
Banking::Logger::~Logger()

{
    // Cleanup resources if needed
    logWriter.flush();
    logWriter.close();
}