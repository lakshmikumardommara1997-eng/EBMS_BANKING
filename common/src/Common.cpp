#include "Common.h"
#include "Logger.h"

#include <iostream>

namespace Banking
{

Common::Common()
{
}

std::string Common::getVersion() const
{
    return "1.0.0";
}

void Common::printWelcome() const
{
    Logger::getInstance().info("Welcome to the Enterprise Banking Management System", __FILE__, __LINE__, __FUNCTION__);

}

}
