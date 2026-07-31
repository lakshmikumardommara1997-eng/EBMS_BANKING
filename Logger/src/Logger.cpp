#include "Logger.h"
#include <iostream>
#include <fstream>


 Banking::Logger& Banking::Logger::getInstance()
{
    static Logger instance;
    return instance;
}
void Banking::Logger::init()
{
    // Load configuration from the specified file
    // For simplicity, we will just set some default values here
    LoggerConfig config;
    config.logFilePath = "logs/app.log";
    config.logLevel = "INFO";
    config.logFormat = "[%LEVEL%] %MESSAGE%";
    config.logToConsole = true;
    config.logToFile = true;
    config.disableLogging = false;
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
    
    std::cout << "["<< SystemTime::currentDateTime()<<"]" << "[INFO] " << message << " (" << file << ":" << line << " in " << function << ")" << std::endl;
}
void Banking::Logger::warn(const std::string& message,const std::string& file, int line, const std::string& function)
{
    // Implement warn level logging
    std::cout << "["<< SystemTime::currentDateTime()<<"]" << "[WARN] " << message << " (" << file << ":" << line << " in " << function << ")" << std::endl;
}
void Banking::Logger::error(const std::string& message,const std::string& file, int line, const std::string& function)
{
    // Implement error level logging
    std::cout << "["<< SystemTime::currentDateTime()<<"]" << "[ERROR] " << message << " (" << file << ":" << line << " in " << function << ")" << std::endl;
}
void Banking::Logger::debug(const std::string& message,const std::string& file, int line, const std::string& function)
{
    // Implement debug level logging
    std::cout << "["<< SystemTime::currentDateTime()<<"]" << "[DEBUG] " << message << " (" << file << ":" << line << " in " << function << ")" << std::endl;
}
Banking::Logger::~Logger()

{
    // Cleanup resources if needed
}