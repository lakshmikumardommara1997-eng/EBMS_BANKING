#ifndef TYPES_H
#define TYPES_H
#include <cstdint> 
#include <map>
#include <string>

namespace Banking::Types
{
    using CUSTOMER_ID = std::uint32_t;
    using ACCOUNT_ID = std::uint32_t;
    typedef std::map<std::string,std::string> SectionMap;
    typedef std::map<std::string,SectionMap> ConfigMap;
    
}
#endif // TYPES_H