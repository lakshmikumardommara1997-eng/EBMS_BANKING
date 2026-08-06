#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include "Query.h"
#include <sstream>
#include <vector>

namespace Banking
{
    class QueryBuilder
    {
    public:
        QueryBuilder();

        QueryBuilder& select(const std::string& columns);
        QueryBuilder& from(const std::string& table);
        QueryBuilder& where(const std::string& condition);
        QueryBuilder& and_(const std::string& condition);
        QueryBuilder& or_(const std::string& condition);
        QueryBuilder& orderBy(const std::string& column, bool ascending = true);
        QueryBuilder& limit(int count);
        QueryBuilder& offset(int count);
        QueryBuilder& insert(const std::string& table);
        QueryBuilder& values(const std::vector<std::string>& columns);
        QueryBuilder& update(const std::string& table);
        QueryBuilder& set(const std::string& column, const std::string& placeholder);
        QueryBuilder& deleteFrom(const std::string& table);
        QueryBuilder& join(const std::string& table, const std::string& condition, const std::string& type = "INNER");

        Query build();
        std::string buildString() const;
        void reset();

    private:
        std::ostringstream m_queryStream;
        std::string m_lastClause;
        bool m_needsSpace = false;
        Query m_query;

        void addSpace();
        void addClause(const std::string& clause);
    };
}

#endif // QUERYBUILDER_H
