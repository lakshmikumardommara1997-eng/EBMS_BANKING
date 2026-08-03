#ifndef DBCONNECTION_H
#define DBCONNECTION_H
#include <string>
namespace Banking
{
    class DBConnection
    {
    public:
        DBConnection(const std::string& connectionString);
        ~DBConnection();

        bool connect();
        void disconnect();
        bool isConnected() const;

    private:
        std::string connectionString_;
        bool connected_;
    };
}

#endif // DBCONNECTION_H