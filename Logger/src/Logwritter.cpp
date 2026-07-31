#include "Logwritter.h"
#include <iostream>
bool Banking::LogWriter::open(const std::string& path)
{
    
    if(logFile.is_open())
        return true;

    logFile.open(path, std::ios::app);

    return logFile.is_open();
}

void Banking::LogWriter::write(const std::string& message)
{
    if(!logFile.is_open())
        return;

    logFile << message << '\n';
}

void Banking::LogWriter::flush()
{
    if(logFile.is_open())
        logFile.flush();
}

void Banking::LogWriter::close()
{
    if(logFile.is_open())
        logFile.close();
}

bool Banking::LogWriter::isOpen() const
{
    return logFile.is_open();
}
void Banking::LogWriter::makedirectory(const std::string& path)
{
    std::cout << "Creating directory: " << path << std::endl;
    std::cout << "Path exists: " << std::filesystem::exists(path) << std::endl;
    std::cout << "Is directory: " << std::filesystem::is_directory(path) << std::endl;
    std::filesystem::path dirPath(path);
    if (!std::filesystem::exists(dirPath))
    {
        std::filesystem::create_directories(dirPath);
    }
}