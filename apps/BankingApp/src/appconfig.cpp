#include "appconfig.h"
#include <iostream>
void Banking::AppConfig::setAppName(const std::string& appName)
{
    APP_NAME = appName;
}
void Banking::AppConfig::setAppVersion(const std::string& appVersion)
{
    APP_VERSION = appVersion;
}
void Banking::AppConfig::setConfigFilePath(const std::string& configFilePath)
{
    CONFIG_FILE_PATH = configFilePath;
}
void Banking::AppConfig::setLogFilePath(const std::string& logFilePath)
{
    LOG_FILE_PATH = logFilePath;
}
void Banking::AppConfig::setLogLevel(const std::string& logLevel)
{
    LOG_LEVEL = logLevel;
}
void Banking::AppConfig::setLogFormat(const std::string& logFormat)
{
    LOG_FORMAT = logFormat;
}
void Banking::AppConfig::setLogToConsole(bool logToConsole)
{
    LOG_TO_CONSOLE = logToConsole;
}
void Banking::AppConfig::setLogToFile(bool logToFile)
{
    LOG_TO_FILE = logToFile;
}
void Banking::AppConfig::setLoggingDisabled(bool disableLogging)
{
    DISABLE_LOGGING = disableLogging;
}
std::string Banking::AppConfig::getAppName() const
{
    return APP_NAME;
}
std::string Banking::AppConfig::getAppVersion() const
{
    return APP_VERSION;
}
std::string Banking::AppConfig::getConfigFilePath() const
{
    return CONFIG_FILE_PATH;
}
std::string Banking::AppConfig::getLogFilePath() const
{
    return LOG_FILE_PATH;
}
std::string Banking::AppConfig::getLogLevel() const
{
    return LOG_LEVEL;
}
std::string Banking::AppConfig::getLogFormat() const
{
    return LOG_FORMAT;
}
bool Banking::AppConfig::isLogToConsole() const
{
    return LOG_TO_CONSOLE;
}
bool Banking::AppConfig::isLogToFile() const
{
    return LOG_TO_FILE;
}
bool Banking::AppConfig::isLoggingDisabled() const
{
    return DISABLE_LOGGING;
}
