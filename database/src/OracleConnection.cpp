#include "Oracleconnection.h"
#include "IQuery.h"

#include <iostream>
#include <memory>
#include <cstring>
#include "Logger.h"

namespace
{
    class OracleEnvironment
    {
    public:
        static OracleEnvironment& instance()
        {
            static OracleEnvironment env;
            return env;
        }

        dpiContext* getContext() const
        {
            return m_context;
        }
        OracleEnvironment()
            : m_context(nullptr)
        {
            if (dpiContext_create(DPI_MAJOR_VERSION,
                                  DPI_MINOR_VERSION,
                                  &m_context,
                                  nullptr) < 0)
            {
                Banking::Logger::getInstance().error("Failed to initialize ODPI-C context", __FILE__, __LINE__, __FUNCTION__);
                m_context = nullptr;
            }
        }
        
        OracleEnvironment(const OracleEnvironment&) = delete;
        OracleEnvironment& operator=(const OracleEnvironment&) = delete;

        dpiContext* m_context;
    };

    void logOracleError(dpiContext* ctx, const std::string& prefix)
    {
        if (!ctx)
        {
            Banking::Logger::getInstance().error(prefix + std::string("Unknown ODPI-C error"), __FILE__, __LINE__, __FUNCTION__);
            return;
        }
        dpiErrorInfo errorInfo;
        dpiContext_getError(ctx, &errorInfo);
        if (errorInfo.code != 0)
        {
            Banking::Logger::getInstance().error(prefix + std::string(errorInfo.message), __FILE__, __LINE__, __FUNCTION__);
        }
        else
        {
            Banking::Logger::getInstance().error(prefix + std::string("Unknown ODPI-C error"), __FILE__, __LINE__, __FUNCTION__);
        }
    }

    std::string normalizeOracleBindVariables(const std::string& sql)
    {
        std::string result;
        result.reserve(sql.size());
        size_t bindIndex = 1;

        for (char ch : sql)
        {
            if (ch == '?')
            {
                result += ':';
                result += std::to_string(bindIndex);
                ++bindIndex;
            }
            else
            {
                result += ch;
            }
        }

        return result;
    }

    struct ParameterBinding
    {
        ParameterBinding()
            : intValue(0), doubleValue(0.0), isNull(false)
        {
            std::memset(&var, 0, sizeof(var));
        }

        ~ParameterBinding()
        {
            if (!stringValue.empty())
            {
                stringValue.clear();
            }
        }

        dpiVar* var = nullptr;
        int intValue;
        double doubleValue;
        std::string stringValue;
        int boolValue;
        bool isNull;
    };

    bool bindParameter(dpiStmt* stmt, dpiConn* conn, size_t paramIndex, const Banking::QueryParameter& param, dpiContext* ctx)
    {
        if (!stmt || !conn)
            return false;

        size_t oracleIndex = paramIndex + 1;

        try
        {
            if (std::holds_alternative<nullptr_t>(param))
            {
                dpiVar* var = nullptr;
                dpiData* data = nullptr;
                // allocate at least 1 byte for NULL string variable to avoid partial-multibyte issues
                if (dpiConn_newVar(conn, DPI_ORACLE_TYPE_VARCHAR, DPI_NATIVE_TYPE_BYTES, 1, 1, 0, 0, nullptr, &var, &data) < 0)
                {
                    logOracleError(ctx, "Failed to create NULL variable: ");
                    return false;
                }
                if (dpiStmt_bindByPos(stmt, oracleIndex, var) < 0)
                {
                    logOracleError(ctx, "Failed to bind NULL parameter: ");
                    return false;
                }
                return true;
            }

            if (std::holds_alternative<int>(param))
            {
                dpiVar* var = nullptr;
                dpiData* data = nullptr;
                if (dpiConn_newVar(conn, DPI_ORACLE_TYPE_NUMBER, DPI_NATIVE_TYPE_INT64, 1, 0, 0, 0, nullptr, &var, &data) < 0)
                {
                    logOracleError(ctx, "Failed to create integer variable: ");
                    return false;
                }

                int64_t value = std::get<int>(param);
                data[0].isNull = 0;
                data[0].value.asInt64 = value;

                if (dpiStmt_bindByPos(stmt, oracleIndex, var) < 0)
                {
                    logOracleError(ctx, "Failed to bind integer parameter: ");
                    return false;
                }
                return true;
            }

            if (std::holds_alternative<double>(param))
            {
                dpiVar* var = nullptr;
                dpiData* data = nullptr;
                if (dpiConn_newVar(conn, DPI_ORACLE_TYPE_NATIVE_DOUBLE, DPI_NATIVE_TYPE_DOUBLE, 1, 0, 0, 0, nullptr, &var, &data) < 0)
                {
                    logOracleError(ctx, "Failed to create double variable: ");
                    return false;
                }

                double value = std::get<double>(param);
                data[0].isNull = 0;
                data[0].value.asDouble = value;

                if (dpiStmt_bindByPos(stmt, oracleIndex, var) < 0)
                {
                    logOracleError(ctx, "Failed to bind double parameter: ");
                    return false;
                }
                return true;
            }

            if (std::holds_alternative<std::string>(param))
            {
                const std::string& strValue = std::get<std::string>(param);
                dpiVar* var = nullptr;
                dpiData* data = nullptr;

                // allocate element size = string byte length + 1 to accommodate multibyte encodings and terminator
                if (dpiConn_newVar(conn, DPI_ORACLE_TYPE_VARCHAR, DPI_NATIVE_TYPE_BYTES, 1, static_cast<uint32_t>(strValue.size() + 1), 0, 0, nullptr, &var, &data) < 0)
                {
                    logOracleError(ctx, "Failed to create string variable: ");
                    return false;
                }

                // Log raw bytes (hex) and length to help diagnose encoding issues
                {
                    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(strValue.c_str());
                    std::string hex;
                    hex.reserve(strValue.size() * 3);
                    for (size_t bi = 0; bi < strValue.size(); ++bi)
                    {
                        char buf[4];
                        snprintf(buf, sizeof(buf), "%02X ", bytes[bi]);
                        hex += buf;
                    }
                    Banking::Logger::getInstance().debug(std::string("[DB DUMP] binding string bytes length=") + std::to_string(strValue.size()) + " hex=" + hex, __FILE__, __LINE__, __FUNCTION__);
                }

                if (dpiVar_setFromBytes(var, 0, strValue.c_str(), static_cast<uint32_t>(strValue.length())) < 0)
                {
                    logOracleError(ctx, "Failed to set string value: ");
                    return false;
                }

                if (dpiStmt_bindByPos(stmt, oracleIndex, var) < 0)
                {
                    logOracleError(ctx, "Failed to bind string parameter: ");
                    return false;
                }
                return true;
            }

            if (std::holds_alternative<bool>(param))
            {
                dpiVar* var = nullptr;
                dpiData* data = nullptr;
                if (dpiConn_newVar(conn, DPI_ORACLE_TYPE_NUMBER, DPI_NATIVE_TYPE_INT64, 1, 0, 0, 0, nullptr, &var, &data) < 0)
                {
                    logOracleError(ctx, "Failed to create boolean variable: ");
                    return false;
                }

                int64_t value = std::get<bool>(param) ? 1 : 0;
                data[0].isNull = 0;
                data[0].value.asInt64 = value;

                if (dpiStmt_bindByPos(stmt, oracleIndex, var) < 0)
                {
                    logOracleError(ctx, "Failed to bind boolean parameter: ");
                    return false;
                }
                return true;
            }

            return false;
        }
        catch (const std::exception& e)
        {
            Banking::Logger::getInstance().error(std::string("Exception in bindParameter: ") + e.what(), __FILE__, __LINE__, __FUNCTION__);
            return false;
        }
    }
}

Banking::OracleConnection::OracleConnection()
    : m_context(OracleEnvironment::instance().getContext()),
      m_connection(nullptr)
{
}

Banking::OracleConnection::~OracleConnection()
{
    disconnect();
}

bool Banking::OracleConnection::connect(const std::string& user,
                               const std::string& password,
                               const std::string& connectString)
{
    if (!m_context)
        return false;

    if (m_connection)
        disconnect();

    if (dpiConn_create(
            m_context,
            user.c_str(),
            user.length(),
            password.c_str(),
            password.length(),
            connectString.c_str(),
            connectString.length(),
            nullptr,
            nullptr,
            &m_connection) < 0)
    {
        logOracleError(m_context, "Oracle Error: ");
        m_connection = nullptr;
        return false;
    }

    return true;
}

void Banking::OracleConnection::disconnect()
{
    if (m_connection)
    {
        dpiConn_release(m_connection);
        m_connection = nullptr;
    }
}

bool Banking::OracleConnection::isConnected() const
{
    return m_connection != nullptr;
}

dpiConn* Banking::OracleConnection::getConnection() const
{
    return m_connection;
}

bool Banking::OracleConnection::beginTransaction()
{
    if (!m_connection)
        return false;

    return true;
}

bool Banking::OracleConnection::commit()
{
    if (!m_connection)
        return false;

    if (dpiConn_commit(m_connection) < 0)
    {
        logOracleError(m_context, "Commit failed: ");
        return false;
    }
    return true;
}

bool Banking::OracleConnection::rollback()
{
    if (!m_connection)
        return false;

    if (dpiConn_rollback(m_connection) < 0)
    {
        logOracleError(m_context, "Rollback failed: ");
        return false;
    }
    return true;
}

bool Banking::OracleConnection::setAutoCommit(bool /*enabled*/)
{
    if (!m_connection)
        return false;

    return true;
}

bool Banking::OracleConnection::executeQuery(const std::string& query)
{
    if (!m_connection)
    {
        Banking::Logger::getInstance().error("Not connected to the database.", __FILE__, __LINE__, __FUNCTION__);
        return false;
    }

    if (query.empty())
    {
        Banking::Logger::getInstance().error("Query is empty.", __FILE__, __LINE__, __FUNCTION__);
        return false;
    }

    dpiStmt* rawStmt = nullptr;
    std::unique_ptr<dpiStmt, decltype(&dpiStmt_release)> stmt(rawStmt, dpiStmt_release);
    if (dpiConn_prepareStmt(m_connection, 0, query.c_str(), query.length(), nullptr, 0, &rawStmt) < 0)
    {
        logOracleError(m_context, "Failed to prepare statement: ");
        return false;
    }
    stmt.reset(rawStmt);

    if (dpiStmt_execute(stmt.get(), 0, nullptr) < 0)
    {
        logOracleError(m_context, "Failed to execute statement: ");
        return false;
    }

    return true;
}

bool Banking::OracleConnection::executeQuery(const IQuery& query)
{
    if (!m_connection)
    {
        Banking::Logger::getInstance().error("Not connected to the database.", __FILE__, __LINE__, __FUNCTION__);
        return false;
    }

    const std::string& sql = query.getSql();
    if (sql.empty())
    {
        Banking::Logger::getInstance().error("Query is empty.", __FILE__, __LINE__, __FUNCTION__);
        return false;
    }

    dpiStmt* rawStmt = nullptr;
    std::unique_ptr<dpiStmt, decltype(&dpiStmt_release)> stmt(rawStmt, dpiStmt_release);
    if (dpiConn_prepareStmt(m_connection, 0, sql.c_str(), sql.length(), nullptr, 0, &rawStmt) < 0)
    {
        logOracleError(m_context, "Failed to prepare statement: ");
        return false;
    }
    stmt.reset(rawStmt);

    if (query.hasParameters())
    {
        std::string normalizedSql = normalizeOracleBindVariables(sql);
        if (normalizedSql != sql)
        {
            dpiStmt* normalizedStmt = nullptr;
            stmt.reset(nullptr);
            if (dpiConn_prepareStmt(m_connection, 0, normalizedSql.c_str(), normalizedSql.length(), nullptr, 0, &normalizedStmt) < 0)
            {
                logOracleError(m_context, "Failed to prepare normalized statement: ");
                return false;
            }
            stmt.reset(normalizedStmt);
        }

        for (size_t i = 0; i < query.getParameterCount(); ++i)
        {
            if (!bindParameter(stmt.get(), m_connection, i, query.getParameter(i), m_context))
            {
                logOracleError(m_context, "Failed to bind parameter at index " + std::to_string(i) + ": ");
                return false;
            }
        }
    }

    if (dpiStmt_execute(stmt.get(), 0, nullptr) < 0)
    {
        logOracleError(m_context, "Failed to execute statement: ");
        return false;
    }

    return true;
}


