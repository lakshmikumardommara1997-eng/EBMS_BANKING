#include "configParser.h"
#include "stringutils.h"
#include <string>

Banking::ConfigParse& Banking::ConfigParse::getInstance()
{
    static ConfigParse instance;
    return instance;
}
void Banking::ConfigParse::parseConfigFile(const std::string& configFilePath)
{
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        throw std::runtime_error("Could not open config file: " + configFilePath);
    }

    std::string line;
    std::string currentSection;

    while (std::getline(configFile, line))
    {
        // Trim whitespace from the line
        line = Banking::StringUtils::trim(line);

        if (line.empty() || line[0] == '#') // Skip empty lines and comments
            continue;

        if (line.front() == '[' && line.back() == ']') // Section header
        {
            currentSection = line.substr(1, line.size() - 2);
            configMap[currentSection] = {}; // Initialize a new section
        }
        else // Key-value pair
        {
            auto delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos)
                continue; // Skip lines without '='

            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            // Trim whitespace from key and value
            key = Banking::StringUtils::trim(key);
            value = Banking::StringUtils::trim(value);

            if (!currentSection.empty())
                configMap[currentSection][key] = value; // Store the key-value pair in the current section
        }
    }
}
Banking::Types::SectionMap Banking::ConfigParse::getConfigMap(std::string section) const
{
    auto it = configMap.find(section);
    if (it != configMap.end())
    {
        return it->second;
    }
    return {}; // Return an empty section map if the section is not found
}
std::string Banking::ConfigParse::getValue(const std::string& section, const std::string& key) const
{
    auto sectionIt = configMap.find(Banking::StringUtils::trim(section));
    if(sectionIt != configMap.end())
    {
        auto keyIt = sectionIt->second.find(Banking::StringUtils::trim(key));
        if(keyIt != sectionIt->second.end())
        {
            return keyIt->second;
        }
    }
    return ""; // Return an empty string if the section or key is not found
}

std::string Banking::ConfigParse::toString(const Banking::Types::SectionMap& sectionMap) const
{
    std::string result;
    for (const auto& [key, value] : sectionMap)
    {
        result += key + "=" + value + "\n";
    }
    return result;
}