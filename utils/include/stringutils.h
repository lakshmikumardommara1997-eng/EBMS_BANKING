#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#include <string>
namespace Banking
{
    class StringUtils
    {
        public:
            static std::string trim(const std::string& str);
            static std::string toLower(const std::string& str);
            static std::string toUpper(const std::string& str);
            static bool startsWith(const std::string& str, const std::string& prefix);
            static bool endsWith(const std::string& str, const std::string& suffix);
            ~StringUtils()=default;
    };
}
#endif // STRINGUTILS_H