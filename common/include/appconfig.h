#ifndef APPCONFIG_H
#define APPCONFIG_H
#include <string>
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
            std::string APP_NAME;
            std::string APP_VERSION;
            std::string CONFIG_FILE_PATH;
            std::string LOG_FILE_PATH;
            std::string LOG_LEVEL;
            std::string LOG_FORMAT;
            std::string SYSDATE_TIME_FORMAT;
            bool LOG_TO_CONSOLE;
            bool LOG_TO_FILE;
            bool DISABLE_LOGGING;
            //AppConfig()=default;
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
            void setSysDateTimeFormat(const std::string& sysDateTimeFormat);
            std::string getSysDateTimeFormat() const;
            std::string getAppName() const;
            std::string getAppVersion() const;
            std::string getConfigFilePath() const;
            std::string getLogFilePath() const;
            std::string getLogLevel() const;
            std::string getLogFormat() const;
            bool isLogToConsole() const;
            bool isLogToFile() const;
            bool isLoggingDisabled() const;
            std::string toString() const;
           
    };
}
#endif // APPCONFIG_H