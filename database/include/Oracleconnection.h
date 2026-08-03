#pragma once

#include <dpi.h>
#include <string>

namespace Banking
{
class OracleConnection
{
public:
    OracleConnection();
    ~OracleConnection();

    OracleConnection(const OracleConnection&) = delete;
    OracleConnection& operator=(const OracleConnection&) = delete;

    bool connect(const std::string& user,
                 const std::string& password,
                 const std::string& connectString);

    void disconnect();

    bool isConnected() const;

    dpiConn* getConnection() const;

private:
    dpiContext* m_context;
    dpiConn* m_connection;
};
}