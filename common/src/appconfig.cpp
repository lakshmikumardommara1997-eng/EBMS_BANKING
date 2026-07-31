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
    LOG_LEVEL = stringToLogLevel(logLevel);
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
Banking::LogLevel Banking::AppConfig::getLogLevel() const
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
void Banking::AppConfig::setSysDateTimeFormat(const std::string& sysDateTimeFormat)
{
    SYSDATE_TIME_FORMAT = sysDateTimeFormat;
}
std::string Banking::AppConfig::getSysDateTimeFormat() const
{
    return SYSDATE_TIME_FORMAT;
}

std::string Banking::AppConfig::toString() const
{
    return "AppConfig [APP_NAME=" + APP_NAME + ", APP_VERSION=" + APP_VERSION +
           ", CONFIG_FILE_PATH=" + CONFIG_FILE_PATH + ", LOG_FILE_PATH=" + LOG_FILE_PATH +
           ", LOG_LEVEL=" + logLevelToString(LOG_LEVEL) + ", LOG_FORMAT=" + LOG_FORMAT +
           ", LOG_TO_CONSOLE=" + (LOG_TO_CONSOLE ? "true" : "false") +
           ", LOG_TO_FILE=" + (LOG_TO_FILE ? "true" : "false") +
           ", DISABLE_LOGGING=" + (DISABLE_LOGGING ? "true" : "false") + ", SYSDATE_TIME_FORMAT=" + SYSDATE_TIME_FORMAT + "]";
}
