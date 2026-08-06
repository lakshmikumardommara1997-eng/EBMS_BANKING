#ifndef IDATABASECONNECTION_H
#define IDATABASECONNECTION_H

#include <string>
#include <memory>

namespace Banking
{
    class IQuery;

    class IDatabaseConnection
    {
        public:
            virtual ~IDatabaseConnection() = default;
            virtual bool connect(const std::string& user, const std::string& password, const std::string& connectString) = 0;
            virtual void disconnect() = 0;
            virtual bool isConnected() const = 0;
            virtual bool beginTransaction() = 0;
            virtual bool commit() = 0;
            virtual bool rollback() = 0;
            virtual bool setAutoCommit(bool enabled) = 0;
            virtual bool executeQuery(const std::string& query) = 0;
            virtual bool executeQuery(const IQuery& query) = 0;
    };
}
#endif // IDATABASECONNECTION_H