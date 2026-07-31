#ifndef LOGGER_WRITTER_H
#define LOGGER_WRITTER_H
#include <string>
#include <fstream>
#include <filesystem>
namespace Banking
{
   class LogWriter
    {
    private:
        std::ofstream logFile;

    public:
        LogWriter() = default;

        bool open(const std::string& path);

        void write(const std::string& message);

        void flush();

        void close();
        bool isOpen() const;
        void makedirectory(const std::string& path);

        ~LogWriter()=default;
    };
}
#endif // LOGGER_WRITTER_H