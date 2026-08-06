#ifndef IQUERY_H
#define IQUERY_H

#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <ctime>

namespace Banking
{
    using QueryParameter = std::variant<int, double, std::string, bool, std::time_t, nullptr_t>;

    class IQuery
    {
    public:
        virtual ~IQuery() = default;

        virtual const std::string& getSql() const = 0;
        virtual size_t getParameterCount() const = 0;
        virtual const QueryParameter& getParameter(size_t index) const = 0;
        virtual bool hasParameters() const = 0;
    };
}

#endif // IQUERY_H
