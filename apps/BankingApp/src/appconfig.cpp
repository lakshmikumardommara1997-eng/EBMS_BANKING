void Banking::setAppName(const std::string& appName)
{
    APP_NAME = appName;
}
void Banking::setAppVersion(const std::string& appVersion)
{
    APP_VERSION = appVersion;
}
void Banking::setConfigFilePath(const std::string& configFilePath)
{
    CONFIG_FILE_PATH = configFilePath;
}
void Banking::setLogFilePath(const std::string& logFilePath)
{
    LOG_FILE_PATH = logFilePath;
}
void Banking::setLogLevel(const std::string& logLevel)
{
    LOG_LEVEL = logLevel;
}
void Banking::setLogFormat(const std::string& logFormat)
{
    LOG_FORMAT = logFormat;
}
void Banking::setLogToConsole(bool logToConsole)
{
    LOG_TO_CONSOLE = logToConsole;
}
void Banking::setLogToFile(bool logToFile)
{
    LOG_TO_FILE = logToFile;
}
void Banking::setLoggingDisabled(bool disableLogging)
{
    DISABLE_LOGGING = disableLogging;
}
std::string Banking::getAppName() const
{
    return APP_NAME;
}
std::string Banking::getAppVersion() const
{
    return APP_VERSION;
}
std::string Banking::getConfigFilePath() const
{
    return CONFIG_FILE_PATH;
}
std::string Banking::getLogFilePath() const
{
    return LOG_FILE_PATH;
}
std::string Banking::getLogLevel() const
{
    return LOG_LEVEL;
}
std::string Banking::getLogFormat() const
{
    return LOG_FORMAT;
}
bool Banking::isLogToConsole() const
{
    return LOG_TO_CONSOLE;
}
bool Banking::isLogToFile() const
{
    return LOG_TO_FILE;
}
bool Banking::isLoggingDisabled() const
{
    return DISABLE_LOGGING;
}
