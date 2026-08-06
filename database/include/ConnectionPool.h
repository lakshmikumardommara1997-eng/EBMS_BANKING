#include "IConnectionPool.h"
#include "IDatabaseConnection.h"
#include <memory>
#include <deque>
#include <mutex>
#include <condition_variable>

namespace Banking
{
    class ConnectionPool : public IConnectionPool
    {
         public:
         std::mutex m_mutex;
         std::condition_variable m_condition;
         std::deque<std::unique_ptr<IDatabaseConnection>> m_connections;
        ConnectionPool() = default;
        ~ConnectionPool() override = default;
         bool initialize(const std::string& user, const std::string& password, const std::string& connectString, int poolSize) override;
        void shutdown() override;
        std::unique_ptr<IDatabaseConnection> acquireConnection() override;
        void releaseConnection(std::unique_ptr<IDatabaseConnection> conn) override;     

    };
}