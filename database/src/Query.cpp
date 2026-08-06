#include "Query.h"

namespace Banking
{
    Query::Query(const std::string& sql)
        : m_sql(sql)
    {
    }

    const std::string& Query::getSql() const
    {
        return m_sql;
    }

    size_t Query::getParameterCount() const
    {
        return m_parameters.size();
    }

    const QueryParameter& Query::getParameter(size_t index) const
    {
        if (index >= m_parameters.size())
        {
            static const QueryParameter nullParam = nullptr;
            return nullParam;
        }
        return m_parameters[index];
    }

    bool Query::hasParameters() const
    {
        return !m_parameters.empty();
    }

    Query& Query::bind(int value)
    {
        m_parameters.push_back(value);
        return *this;
    }

    Query& Query::bind(double value)
    {
        m_parameters.push_back(value);
        return *this;
    }

    Query& Query::bind(const std::string& value)
    {
        m_parameters.push_back(value);
        return *this;
    }

    Query& Query::bind(bool value)
    {
        m_parameters.push_back(value);
        return *this;
    }

    Query& Query::bind(std::time_t value)
    {
        m_parameters.push_back(value);
        return *this;
    }

    Query& Query::bindNull()
    {
        m_parameters.push_back(nullptr);
        return *this;
    }

    Query& Query::reset()
    {
        clearBindings();
        return *this;
    }

    void Query::clearBindings()
    {
        m_parameters.clear();
    }
}
