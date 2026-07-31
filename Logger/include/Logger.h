#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include "systemtime.h"
#include "appconfig.h"
#include "Logwritter.h"
namespace Banking
{
  
    class Logger
    {
        private:
        Logger() {} // Private constructor to prevent instantiation
        Logger(const Logger&) = delete; // Delete copy constructor
        Logger& operator=(const Logger&) = delete; // Delete copy assignment operator
        Logger(Logger&&) = delete; // Delete move constructor
        Logger& operator=(Logger&&) = delete; // Delete move assignment operator
        Banking::LogWriter logWriter;
        public:
        static Logger& getInstance();
        void init() ;

        void log(const std::string& message,const std::string& file, int line, const std::string& function, const std::string& level);
        void info(const std::string& message,const std::string& file, int line, const std::string& function);
        void warn(const std::string& message,const std::string& file, int line, const std::string& function);
        void error(const std::string& message,const std::string& file, int line, const std::string& function);
        void debug(const std::string& message,const std::string& file, int line, const std::string& function);

        ~Logger();
    };

}
#endif // LOGGER_H