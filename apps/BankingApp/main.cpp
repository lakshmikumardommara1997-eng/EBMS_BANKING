#include "Common.h"
#include "Customer.h"
#include <vector>
#include <iostream>
#include <memory>
#include <deque>
#include <thread>
#include "Logger.h"
#include <mutex>
#include <condition_variable>
#include "Oracleconnection.h"
#include "IDatabaseConnection.h"
#include "IConnectionPool.h"
#include "ConnectionPool.h"


#define MAX_QUEUE_SIZE 10000

std::mutex queueMutex;
std::condition_variable conditionVar;
bool isdone = false;

void ProduceCustomerData(std::deque<std::unique_ptr<Banking::Customer>>& customers, int startId, int count,Banking::Logger& logger)
{
    for (int i = 0; i < count; ++i)
    {
       
        Banking::Types::CUSTOMER_ID customerId = startId + i;
        std::string name = "Customer " + std::to_string(customerId);
        std::string address = "Address " + std::to_string(customerId);
        std::string phone = "555-000" + std::to_string(customerId);
        std::string email = "customer" + std::to_string(customerId) + "@example.com";

        auto customer = std::make_unique<Banking::Customer>(customerId, name, address, phone, email);
         std::unique_lock<std::mutex> lock(queueMutex);
        conditionVar.wait(lock,[&customers,&logger](){
            logger.debug("Waiting for space in the queue to produce customer data...", __FILE__, __LINE__, __FUNCTION__);
            return customers.size() < MAX_QUEUE_SIZE;
        });
        customers.push_back(std::move(customer));
        conditionVar.notify_one();
    }
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        isdone = true;
        
    }
    conditionVar.notify_all();
}

void ConsumeCustomerData(std::deque<std::unique_ptr<Banking::Customer>>& customers, Banking::Logger& logger)
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        conditionVar.wait(lock, [&customers,&logger]() {
            logger.debug("Waiting for customer data to consume...", __FILE__, __LINE__, __FUNCTION__);
            return !customers.empty() || isdone;
        });
        if(customers.empty() && isdone)
        {
            logger.info("No more customer data to consume. Exiting consumer thread.", __FILE__, __LINE__, __FUNCTION__);
            break;
        }
        auto customer = std::move(customers.front());
        
        customers.pop_front();
        conditionVar.notify_one();
        if (customer)
        {
           // customer->printInfo();
            logger.info("Consuming customer data: " + customer->toString(), __FILE__, __LINE__, __FUNCTION__);
        }
    }
}
void initLogger(Banking::Logger& logger)
{
    logger.init();
}

void intiConnectionPool(Banking::IConnectionPool& connectionPool)
{
    Banking::AppConfig& appConfig = Banking::AppConfig::getInstance();
    if (connectionPool.initialize(appConfig.getDBUser(), appConfig.getDBPassword(), appConfig.getDBConnectString(), 5))
    {
        Banking::Logger::getInstance().info("Connection pool initialized successfully", __FILE__, __LINE__, __FUNCTION__);
    }
    else
    {
        Banking::Logger::getInstance().error("Failed to initialize connection pool", __FILE__, __LINE__, __FUNCTION__);
    }
}
int main()
{
    Banking::Common common;
    Banking::AppConfig& appConfig = Banking::AppConfig::getInstance();
    appConfig.LoadConfigFromFile("param/Ebms.cfg");
    Banking::Logger& logger = Banking::Logger::getInstance();
    initLogger(logger);
    
    logger.info("Starting Banking Application", __FILE__, __LINE__, __FUNCTION__);
    logger.info("Application Configuration: " + appConfig.toString(), __FILE__, __LINE__, __FUNCTION__);


    // Create a new customer
    Banking::Types::CUSTOMER_ID customerId = 1;
    Banking::Customer customer(customerId, "John Doe", "123 Main St", "555-1234", "john.doe@example.com");
    std::deque<std::unique_ptr<Banking::Customer>> customers;
    customers.push_back(std::make_unique<Banking::Customer>(customer));
   
    logger.info("Customer created: " + customers[0]->toString(), __FILE__, __LINE__, __FUNCTION__);
    std::thread producerThread(ProduceCustomerData, std::ref(customers), 2, 10000,std::ref(logger));
    std::thread consumerThread(ConsumeCustomerData, std::ref(customers), std::ref(logger));
    producerThread.join();
    consumerThread.join();
    std::unique_ptr<Banking::IConnectionPool> connectionPool = std::make_unique<Banking::ConnectionPool>();
    
    intiConnectionPool(*connectionPool);
    std::unique_ptr<Banking::IDatabaseConnection> dbConnection = connectionPool->acquireConnection();
    if(dbConnection && dbConnection->isConnected())
    {
        logger.info("Connected to Oracle database successfully", __FILE__, __LINE__, __FUNCTION__);
        // Perform database operations here
        dbConnection->disconnect();
        logger.info("Disconnected from Oracle database", __FILE__, __LINE__, __FUNCTION__);
    }
    else
    {
        logger.error("Failed to connect to Oracle database", __FILE__, __LINE__, __FUNCTION__);
        logger.info("user: " + appConfig.getDBUser() + ", password: " + appConfig.getDBPassword() + ", connect string: " + appConfig.getDBConnectString(), __FILE__, __LINE__, __FUNCTION__);
    }

    
    /*
    if (connectionPool->initialize(appConfig.getDBUser(), appConfig.getDBPassword(), appConfig.getDBConnectString(), 5))
    {
        logger.info("Connection pool initialized successfully", __FILE__, __LINE__, __FUNCTION__);
    }
    else
    {
        logger.error("Failed to initialize connection pool", __FILE__, __LINE__, __FUNCTION__);
    }
    std::unique_ptr<Banking::IDatabaseConnection> dbConnection = connectionPool->acquireConnection();
    
    
    
    if (dbConnection && dbConnection->isConnected())
    { 
        logger.info("Connected to Oracle database successfully", __FILE__, __LINE__, __FUNCTION__);
        // Perform database operations here
        dbConnection->disconnect();
        logger.info("Disconnected from Oracle database", __FILE__, __LINE__, __FUNCTION__);
    }
    else
    {   
        logger.info("Failed to connect to Oracle database", __FILE__, __LINE__, __FUNCTION__);
        logger.info("user: " + appConfig.getDBUser() + ", password: " + appConfig.getDBPassword() + ", connect string: " + appConfig.getDBConnectString(), __FILE__, __LINE__, __FUNCTION__);
        logger.error("Failed to connect to Oracle database", __FILE__, __LINE__, __FUNCTION__);
    }  
    connectionPool->releaseConnection(std::move(dbConnection)); 
    connectionPool->shutdown();
    */

    logger.info("Banking Application finished", __FILE__, __LINE__, __FUNCTION__);
    return 0;

   
}
