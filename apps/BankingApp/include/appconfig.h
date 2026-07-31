#ifndef APPCONFIG_H
#define APPCONFIG_H
namespace Banking
{
    class AppConfig
    {
        private:
            AppConfig() {} // Private constructor to prevent instantiation
            AppConfig(const AppConfig&) = delete; // Delete copy constructor
            AppConfig& operator=(const AppConfig&) = delete; // Delete copy assignment operator
            AppConfig(AppConfig&&) = delete; // Delete move constructor
            AppConfig& operator=(AppConfig&&) = delete; // Delete move assignment operator
            const std::string APP_NAME;
            const std::string APP_VERSION;
            const std::string CONFIG_FILE_PATH;
            const std::string LOG_FILE_PATH;
            const std::string LOG_LEVEL;
            const std::string LOG_FORMAT;
            const bool LOG_TO_CONSOLE;
            const bool LOG_TO_FILE;
            const bool DISABLE_LOGGING;
            AppConfig=default;
        public:
            static AppConfig& getInstance()
            {
                static AppConfig instance;
                return instance;
            }
            void setAppName(const std::string& appName);
            void setAppVersion(const std::string& appVersion);
            void setConfigFilePath(const std::string& configFilePath);
            void setLogFilePath(const std::string& logFilePath);
            void setLogLevel(const std::string& logLevel);
            void setLogFormat(const std::string& logFormat);
            void setLogToConsole(bool logToConsole);
            void setLogToFile(bool logToFile);
            void setLoggingDisabled(bool disableLogging);
            std::string getAppName() const { return APP_NAME; }
            std::string getAppVersion() const { return APP_VERSION; }
            std::string getConfigFilePath() const { return CONFIG_FILE_PATH; }
            std::string getLogFilePath() const { return LOG_FILE_PATH; }
            std::string getLogLevel() const { return LOG_LEVEL; }
            std::string getLogFormat() const { return LOG_FORMAT; }
            bool isLogToConsole() const { return LOG_TO_CONSOLE; }
            bool isLogToFile() const { return LOG_TO_FILE; }
            bool isLoggingDisabled() const { return DISABLE_LOGGING; }
           
    };
}
#endif // APPCONFIG_H