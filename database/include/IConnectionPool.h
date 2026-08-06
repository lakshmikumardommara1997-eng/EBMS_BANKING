#ifndef ICONNECTIONPOOL_H
#define ICONNECTIONPOOL_H
#include <string>
#include <memory>
#include "IDatabaseConnection.h"

namespace Banking
{
    class IConnectionPool
    {
        public:
            virtual ~IConnectionPool() = default;
            virtual bool initialize(const std::string& user, const std::string& password, const std::string& connectString, int poolSize) = 0;
            virtual void shutdown() = 0;
            virtual std::unique_ptr<IDatabaseConnection> acquireConnection() = 0;
            virtual void releaseConnection(std::unique_ptr<IDatabaseConnection> conn) = 0;
    };
}
#endif // ICONNECTIONPOOL_H