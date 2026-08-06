#include "ConnectionPool.h"
#include "Oracleconnection.h"
#include <iostream>

namespace Banking
{
    bool ConnectionPool::initialize(const std::string& user, const std::string& password, const std::string& connectString, int poolSize)
    {
        if (poolSize <= 0)
            return false;

        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_shutdown)
            return false;

        m_connections.clear();
        for (int i = 0; i < poolSize; ++i)
        {
            auto conn = std::make_unique<OracleConnection>();
            if (!conn->connect(user, password, connectString))
            {
                std::cerr << "Failed to create connection " << i << std::endl;
                for (auto& createdConn : m_connections)
                {
                    if (createdConn && createdConn->isConnected())
                    {
                        createdConn->disconnect();
                    }
                }
                m_connections.clear();
                return false;
            }
            m_connections.push_back(std::move(conn));
        }

        return !m_connections.empty();
    }

    void ConnectionPool::shutdown()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_shutdown = true;
        m_condition.notify_all();

        for (auto& conn : m_connections)
        {
            if (conn && conn->isConnected())
            {
                conn->disconnect();
            }
        }
        m_connections.clear();
    }

    std::unique_ptr<IDatabaseConnection> ConnectionPool::acquireConnection()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this]() { return m_shutdown || !m_connections.empty(); });

        if (m_shutdown && m_connections.empty())
            return nullptr;

        auto conn = std::move(m_connections.front());
        m_connections.pop_front();
        return conn;
    }

    void ConnectionPool::releaseConnection(std::unique_ptr<IDatabaseConnection> conn)
    {
        if (!conn)
            return;

        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_shutdown)
        {
            if (conn->isConnected())
            {
                conn->disconnect();
            }
            return;
        }

        m_connections.push_back(std::move(conn));
        m_condition.notify_one();
    }
}