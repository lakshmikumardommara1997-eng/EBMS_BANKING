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
    bool beginTransaction() override;
    bool commit() override;
    bool rollback() override;
    bool setAutoCommit(bool enabled) override;
    bool executeQuery(const std::string& query) override;
    bool executeQuery(const IQuery& query) override;

    dpiConn* getConnection() const;

private:
    dpiContext* m_context;
    dpiConn* m_connection;
};
}