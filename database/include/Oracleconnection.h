#pragma once

#include <dpi.h>
#include <string>
#include "IDatabaseConnection.h"

namespace Banking
{
class OracleConnection : public IDatabaseConnection
{
public:
    OracleConnection();
    ~OracleConnection();

    OracleConnection(const OracleConnection&) = delete;
    OracleConnection& operator=(const OracleConnection&) = delete;

    bool connect(const std::string& user,
                 const std::string& password,
                 const std::string& connectString) override;

    void disconnect() override;

    bool isConnected() const override;
    void commit() override;
    void rollback() override;
    void executeQuery(const std::string& query) override;

    dpiConn* getConnection() const;

private:
    dpiContext* m_context;
    dpiConn* m_connection;
};
}