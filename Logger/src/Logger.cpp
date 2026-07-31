#include "Logger.h"
#include <iostream>
#include <fstream>
#include "Logwritter.h"
#include <mutex>

static std::mutex loggerMutex;

Banking::Logger& Banking::Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Banking::Logger::log(const std::string& message,const std::string& file, int line, const std::string& function, const std::string& level)
{
    // Implement logging logic here
    // For example, write to console or file based on the configuration
    if (level == "INFO")
    {
        info(message, file, line, function);
    }
    else if (level == "WARN")
    {
        warn(message, file, line, function);
    }
    else if (level == "ERROR")
    {
        error(message, file, line, function);
    }
    else if (level == "DEBUG")
    {
        debug(message, file, line, function);
    }
}
void Banking::Logger::info(const std::string& message,const std::string& file, int line, const std::string& function)
{
    // Implement info level logging with timing information
    if (AppConfig::getInstance().isLoggingDisabled() || AppConfig::getInstance().getLogLevel() != "INFO") {

        return; // Skip logging if disabled
    }
    if (AppConfig::getInstance().isLogToConsole()) {
        std::cout << "["<< SystemTime::currentDateTime(AppConfig::getInstance().getSysDateTimeFormat())<<"]" << "[INFO] " << message << " (" << file << ":" << line << " in " << function << ")" << std::endl;
    }
    if (AppConfig::getInstance().isLogToFile()&& AppConfig::getInstance().getLogLevel() == "INFO") {
        std::lock_guard<std::mutex> lock(loggerMutex); // Lock the mutex to ensure thread safety
        logWriter.write("[" + SystemTime::currentDateTime(AppConfig::getInstance().getSysDateTimeFormat()) + "]" + "[INFO] " + message + " (" + file + ":" + std::to_string(line) + " in " + function + ")");
        
        // Unlock the mutex after writing to the log file
    }
   // std::cout << "["<< SystemTime::currentDateTime(AppConfig::getInstance().getSysDateTimeFormat())<<"]" << "[INFO] " << message << " (" << file << ":" << line << " in " << function << ")" << std::endl;
}
void Banking::Logger::warn(const std::string& message,const std::string& file, int line, const std::string& function)
{
    if (AppConfig::getInstance().isLoggingDisabled() || AppConfig::getInstance().getLogLevel() != "WARN") {
        return; // Skip logging if disabled
    }
    // Implement warn level logging
    std::cout << "["<< SystemTime::currentDateTime(AppConfig::getInstance().getSysDateTimeFormat())<<"]" << "[WARN] " << message << " (" << file << ":" << line << " in " << function << ")" << std::endl;
}
void Banking::Logger::error(const std::string& message,const std::string& file, int line, const std::string& function)
{
    if (AppConfig::getInstance().isLoggingDisabled() || AppConfig::getInstance().getLogLevel() != "ERROR") {
        return; // Skip logging if disabled
    }
    // Implement error level logging
    std::cout << "["<< SystemTime::currentDateTime(AppConfig::getInstance().getSysDateTimeFormat())<<"]" << "[ERROR] " << message << " (" << file << ":" << line << " in " << function << ")" << std::endl;
}
void Banking::Logger::debug(const std::string& message,const std::string& file, int line, const std::string& function)
{
    if (AppConfig::getInstance().isLoggingDisabled() || AppConfig::getInstance().getLogLevel() != "DEBUG") {
        return; // Skip logging if disabled
    }
    // Implement debug level logging
    std::cout << "["<< SystemTime::currentDateTime(AppConfig::getInstance().getSysDateTimeFormat())<<"]" << "[DEBUG] " << message << " (" << file << ":" << line << " in " << function << ")" << std::endl;
}
void Banking::Logger::init()
{
    auto& appConfig = AppConfig::getInstance();
    if (appConfig.isLoggingDisabled()) {
        return; // Skip initialization if logging is disabled
    }
   logWriter.makedirectory(appConfig.getLogFilePath().substr(0, appConfig.getLogFilePath().find_last_of("/")));
   if (!logWriter.open(appConfig.getLogFilePath())) {
        std::cerr << "Failed to open log file: " << appConfig.getLogFilePath() << std::endl;
    }
}
Banking::Logger::~Logger()

{
    // Cleanup resources if needed
    logWriter.close();
    logWriter.flush();
}