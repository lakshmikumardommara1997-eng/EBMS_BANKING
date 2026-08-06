#include "Oracleconnection.h"

#include <iostream>



Banking::OracleConnection::OracleConnection()
    : m_context(nullptr),
      m_connection(nullptr)
{
    if (dpiContext_create(DPI_MAJOR_VERSION,
                          DPI_MINOR_VERSION,
                          &m_context,
                          nullptr) < 0)
    {
        std::cerr << "Failed to initialize ODPI-C context\n";
        m_context = nullptr;
    }
}

Banking::OracleConnection::~OracleConnection()
{
    disconnect();

    if (m_context)
    {
        dpiContext_destroy(m_context);
        m_context = nullptr;
    }
}

bool Banking::OracleConnection::connect(const std::string& user,
                               const std::string& password,
                               const std::string& connectString)
{
    if (!m_context)
        return false;

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
        dpiErrorInfo errorInfo;
        dpiContext_getError(m_context, &errorInfo);

        std::cerr << "Oracle Error : "
                  << errorInfo.message
                  << std::endl;

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
void Banking::OracleConnection::commit()
{
    if (m_connection)
    {
        if (dpiConn_commit(m_connection) < 0)
        {
            dpiErrorInfo errorInfo;
            dpiContext_getError(m_context, &errorInfo);
            std::cerr << "Commit failed: " << errorInfo.message << std::endl;
        }
    }
}
void Banking::OracleConnection::rollback()
{
    if (m_connection)
    {
        if (dpiConn_rollback(m_connection) < 0)
        {
            dpiErrorInfo errorInfo;
            dpiContext_getError(m_context, &errorInfo);
            std::cerr << "Rollback failed: " << errorInfo.message << std::endl;
        }
    }
}
void Banking::OracleConnection::executeQuery(const std::string& query)
{
    if (!m_connection)
    {
        std::cerr << "Not connected to the database." << std::endl;
        return;
    }

    dpiStmt* stmt = nullptr;
    if (dpiConn_prepareStmt(m_connection, 0, query.c_str(), query.length(), nullptr, 0, &stmt) < 0)
    {
        dpiErrorInfo errorInfo;
        dpiContext_getError(m_context, &errorInfo);
        std::cerr << "Failed to prepare statement: " << errorInfo.message << std::endl;
        return;
    }

    if (dpiStmt_execute(stmt, 0, nullptr) < 0)
    {
        dpiErrorInfo errorInfo;
        dpiContext_getError(m_context, &errorInfo);
        std::cerr << "Failed to execute statement: " << errorInfo.message << std::endl;
    }

    dpiStmt_release(stmt);
}


