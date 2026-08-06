#include "ConnectionPool.h"
#include <memory>
#include <string>
#include "Oracleconnection.h"
#include <iostream>

namespace Banking{

   bool ConnectionPool::initialize(const std::string& user,const std::string& password,const std::string& connectString,int poolSize)
    {
        for(int i=0;i<poolSize;i++)
        {
            std::unique_ptr<IDatabaseConnection> conn = std::make_unique<OracleConnection>();
            if(conn->connect(user,password,connectString))
            {
                m_connections.push_back(std::move(conn));
            }
            else
            {
                std::cerr<<"Failed to create connection "<<i<<std::endl;
            }
        }
        return true;
    }

    void ConnectionPool::shutdown()
    {
        for(auto& conn:m_connections)
        {
            conn->disconnect();
        }
        m_connections.clear();
    }
    std::unique_ptr<IDatabaseConnection> ConnectionPool::acquireConnection()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock,[this](){return !m_connections.empty();});
        auto conn = std::move(m_connections.front());
        m_connections.pop_front();
        return conn;
    }
    void ConnectionPool::releaseConnection(std::unique_ptr<IDatabaseConnection> conn)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_connections.push_back(std::move(conn));
        m_condition.notify_one();
    }

}