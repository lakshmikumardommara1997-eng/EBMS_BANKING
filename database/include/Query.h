#ifndef QUERY_H
#define QUERY_H

#include "IQuery.h"
#include <vector>

namespace Banking
{
    class Query : public IQuery
    {
    public:
        explicit Query(const std::string& sql);

        const std::string& getSql() const override;
        size_t getParameterCount() const override;
        const QueryParameter& getParameter(size_t index) const override;
        bool hasParameters() const override;

        Query& bind(int value);
        Query& bind(double value);
        Query& bind(const std::string& value);
        Query& bind(bool value);
        Query& bind(std::time_t value);
        Query& bindNull();

        Query& reset();
        void clearBindings();

    private:
        std::string m_sql;
        std::vector<QueryParameter> m_parameters;
    };
}

#endif // QUERY_H
