#ifndef IDATABASECONNECTION_H
#define IDATABASECONNECTION_H

namespace Banking
{
    class IDatabaseConnection
    {
        public:
            virtual ~IDatabaseConnection() = default;
            virtual bool connect(const std::string& user, const std::string& password, const std::string& connectString) = 0;
            virtual void disconnect() = 0;
            virtual bool isConnected() const = 0;
            virtual void commit() = 0;
            virtual void rollback() = 0;
            virtual void executeQuery(const std::string& query) = 0;
            
    };
}
#endif // IDATABASECONNECTION_H