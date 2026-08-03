#include "OracleConnection.h"

#include <iostream>

namespace Banking
{

OracleConnection::OracleConnection()
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

OracleConnection::~OracleConnection()
{
    disconnect();

    if (m_context)
    {
        dpiContext_destroy(m_context);
        m_context = nullptr;
    }
}

bool OracleConnection::connect(const std::string& user,
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

void OracleConnection::disconnect()
{
    if (m_connection)
    {
        dpiConn_release(m_connection);
        m_connection = nullptr;
    }
}

bool OracleConnection::isConnected() const
{
    return m_connection != nullptr;
}

dpiConn* OracleConnection::getConnection() const
{
    return m_connection;
}

}