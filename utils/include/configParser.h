#ifndef CONFIG_PARSE_H
#define CONFIG_PARSE_H
#include <string>
#include <fstream>
#include "Types.h"
namespace Banking
{
    class ConfigParse
    {
        private:
            ConfigParse() {} // Private constructor to prevent instantiation
            ConfigParse(const ConfigParse&) = delete; // Delete copy constructor
            ConfigParse& operator=(const ConfigParse&) = delete; // Delete copy assignment operator
            ConfigParse(ConfigParse&&) = delete; // Delete move constructor
            ConfigParse& operator=(ConfigParse&&) = delete; // Delete move assignment operator
        public:
            static ConfigParse& getInstance();
            void parseConfigFile(const std::string& configFilePath);
            Banking::Types::SectionMap getConfigMap(std::string section) const;
            Banking::Types::ConfigMap configMap;
            std::string getValue(const std::string& section, const std::string& key) const;
            std::string toString(const Banking::Types::SectionMap& sectionMap) const;
            ~ConfigParse()=default;


    };
}

#endif // CONFIG_PARSE_H