#include "Common.h"

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
    std::cout << "=====================================\n";
    std::cout << " Enterprise Banking Management System\n";
    std::cout << " Version : " << getVersion() << "\n";
    std::cout << "=====================================\n";
}

}
