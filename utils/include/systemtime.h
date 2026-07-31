#ifndef SYSTEMTIME_H
#define SYSTEMTIME_H

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>


namespace Banking {
    class SystemTime {
        private:
        // Private member variables and methods can be added here if needed
        std::string systemTimeFormat;
    public:
        SystemTime();
        ~SystemTime();
        static std::time_t now();
        static std::tm toLocalTime(std::time_t);
        std::string getCurrentTimeString() const;
        static std::string formatTime(const std::tm&,const std::string&);
        static std::string currentDateTime(const std::string& format ="%d-%b-%Y %H:%M:%S");
        int getCurrentHour() const;
        int getCurrentMinute() const;
        int getCurrentSecond() const;
    };
}

#endif // SYSTEMTIME_H