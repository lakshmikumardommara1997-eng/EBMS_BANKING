#ifndef COMMON_H
#define COMMON_H

#include <string>

namespace Banking
{
    class Common
    {
    public:
        Common();

        std::string getVersion() const;

        void printWelcome() const;
    };
}

#endif
