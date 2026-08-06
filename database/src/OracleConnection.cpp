#include "Oracleconnection.h"

#include <iostream>
#include <memory>

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

    private:
        OracleEnvironment()
            : m_context(nullptr)
        {
            if (dpiContext_create(DPI_MAJOR_VERSION,
                                  DPI_MINOR_VERSION,
                                  &m_context,
                                  nullptr) < 0)
            {
                std::cerr << "Failed to initialize ODPI-C context" << std::endl;
                m_context = nullptr;
            }
        }

        ~OracleEnvironment()
        {
            if (m_context)
            {
                dpiContext_destroy(m_context);
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
            std::cerr << prefix << "Unknown ODPI-C error" << std::endl;
            return;
        }
        dpiErrorInfo errorInfo;
        dpiContext_getError(ctx, &errorInfo);
        if (errorInfo.code != 0)
        {
            std::cerr << prefix << errorInfo.message << std::endl;
        }
        else
        {
            std::cerr << prefix << "Unknown ODPI-C error" << std::endl;
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

bool Banking::OracleConnection::setAutoCommit(bool enabled)
{
    if (!m_connection)
        return false;

    return true;
}

bool Banking::OracleConnection::executeQuery(const std::string& query)
{
    if (!m_connection)
    {
        std::cerr << "Not connected to the database." << std::endl;
        return false;
    }

    if (query.empty())
    {
        std::cerr << "Query is empty." << std::endl;
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


