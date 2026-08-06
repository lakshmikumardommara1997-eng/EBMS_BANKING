#include "QueryBuilder.h"

namespace Banking
{
    QueryBuilder::QueryBuilder()
        : m_query("")
    {
    }

    QueryBuilder& QueryBuilder::select(const std::string& columns)
    {
        reset();
        addClause("SELECT " + columns);
        return *this;
    }

    QueryBuilder& QueryBuilder::from(const std::string& table)
    {
        addClause("FROM " + table);
        return *this;
    }

    QueryBuilder& QueryBuilder::where(const std::string& condition)
    {
        addClause("WHERE " + condition);
        return *this;
    }

    QueryBuilder& QueryBuilder::and_(const std::string& condition)
    {
        addClause("AND " + condition);
        return *this;
    }

    QueryBuilder& QueryBuilder::or_(const std::string& condition)
    {
        addClause("OR " + condition);
        return *this;
    }

    QueryBuilder& QueryBuilder::orderBy(const std::string& column, bool ascending)
    {
        std::string order = ascending ? "ASC" : "DESC";
        addClause("ORDER BY " + column + " " + order);
        return *this;
    }

    QueryBuilder& QueryBuilder::limit(int count)
    {
        if (count > 0)
        {
            addClause("LIMIT " + std::to_string(count));
        }
        return *this;
    }

    QueryBuilder& QueryBuilder::offset(int count)
    {
        if (count >= 0)
        {
            addClause("OFFSET " + std::to_string(count));
        }
        return *this;
    }

    QueryBuilder& QueryBuilder::insert(const std::string& table)
    {
        reset();
        addClause("INSERT INTO " + table);
        return *this;
    }

    QueryBuilder& QueryBuilder::values(const std::vector<std::string>& columns)
    {
        std::string columnList = "(";
        for (size_t i = 0; i < columns.size(); ++i)
        {
            if (i > 0)
                columnList += ", ";
            columnList += columns[i];
        }
        columnList += ") VALUES (";

        std::string placeholders = "";
        for (size_t i = 0; i < columns.size(); ++i)
        {
            if (i > 0)
                placeholders += ", ";
            placeholders += "?";
        }
        placeholders += ")";

        addClause(columnList + placeholders);
        return *this;
    }

    QueryBuilder& QueryBuilder::update(const std::string& table)
    {
        reset();
        addClause("UPDATE " + table);
        return *this;
    }

    QueryBuilder& QueryBuilder::set(const std::string& column, const std::string& placeholder)
    {
        std::string setClause = "SET " + column + " = " + placeholder;
        if (m_lastClause != "SET" && !m_lastClause.empty())
        {
            setClause = ", " + column + " = " + placeholder;
        }
        addClause(setClause);
        m_lastClause = "SET";
        return *this;
    }

    QueryBuilder& QueryBuilder::deleteFrom(const std::string& table)
    {
        reset();
        addClause("DELETE FROM " + table);
        return *this;
    }

    QueryBuilder& QueryBuilder::join(const std::string& table, const std::string& condition, const std::string& type)
    {
        addClause(type + " JOIN " + table + " ON " + condition);
        return *this;
    }

    Query QueryBuilder::build()
    {
        return Query(buildString());
    }

    std::string QueryBuilder::buildString() const
    {
        return m_queryStream.str();
    }

    void QueryBuilder::reset()
    {
        m_queryStream.str("");
        m_queryStream.clear();
        m_lastClause = "";
        m_needsSpace = false;
    }

    void QueryBuilder::addSpace()
    {
        if (m_needsSpace && m_queryStream.tellp() > 0)
        {
            m_queryStream << " ";
        }
    }

    void QueryBuilder::addClause(const std::string& clause)
    {
        addSpace();
        m_queryStream << clause;
        m_needsSpace = true;
    }
}
