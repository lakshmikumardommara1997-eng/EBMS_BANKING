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
void Banking::AppConfig::LoadConfigFromFile(const std::string& configFilePath)
{
    try
    {
        configParser.parseConfigFile(configFilePath);
        auto appSection = configParser.getConfigMap("App");
        if (!appSection.empty())
        {
            setAppName(appSection["APP_NAME"]);
            setAppVersion(appSection["APP_VERSION"]);
            setConfigFilePath(appSection["CONFIG_FILE_PATH"]);
            setLogFilePath(appSection["LOG_FILE_PATH"]);
        }
        else
        {
            std::cerr << "App section not found in config file: " << configFilePath << std::endl;
        }
        auto logSection = configParser.getConfigMap("Logging");
        if (!logSection.empty())
        {
            setLogLevel(logSection["LOG_LEVEL"]);
            setLogFormat(logSection["LOG_FORMAT"]);
            setLogToConsole(logSection["LOG_TO_CONSOLE"] == "true");
            setLogToFile(logSection["LOG_TO_FILE"] == "true");
            setLoggingDisabled(logSection["DISABLE_LOGGING"] == "true");
            setSysDateTimeFormat(logSection["SYSDATE_TIME_FORMAT"]);
        }
        else
        {
            std::cerr << "Logging section not found in config file: " << configFilePath << std::endl;
        }
        auto dbSection = configParser.getConfigMap("Database");
        if (!dbSection.empty())
        {
            setDBUser(dbSection["DB_USER"]);
            setDBPassword(dbSection["DB_PASSWORD"]);
            setDBConnectString(dbSection["DB_CONNECTION_STRING"]);
        }
        else
        {
            std::cerr << "Database section not found in config file: " << configFilePath << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error loading configuration from file: " << e.what() << std::endl;
    }
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
void Banking::AppConfig::setDBUser(const std::string& dbUser)
{
    DB_USER = dbUser;
}
void Banking::AppConfig::setDBPassword(const std::string& dbPassword)
{
    DB_PASSWORD = dbPassword;
}
void Banking::AppConfig::setDBConnectString(const std::string& dbConnectString)
{
    DB_CONNECT_STRING = dbConnectString;
}
std::string Banking::AppConfig::getDBUser() const
{
    return DB_USER;
}
std::string Banking::AppConfig::getDBPassword() const
{
    return DB_PASSWORD;
}
std::string Banking::AppConfig::getDBConnectString() const
{
    return DB_CONNECT_STRING;
}
