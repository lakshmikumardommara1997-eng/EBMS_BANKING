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

int main()
{
    Banking::Common common;
    Banking::AppConfig& appConfig = Banking::AppConfig::getInstance();
    appConfig.LoadConfigFromFile("param/Ebms.cfg");
    Banking::Logger& logger = Banking::Logger::getInstance();
    Banking::Logger::getInstance().init();
    std::cout << "Application Name: " << appConfig.getAppName() << std::endl;
    std::cout << "Application Version: " << appConfig.getAppVersion() << std::endl;
    std::cout << "Configuration File Path: " << appConfig.getConfigFilePath() << std::endl;
    logger.info("Starting Banking Application", __FILE__, __LINE__, __FUNCTION__);
    logger.info("Application Configuration: " + appConfig.toString(), __FILE__, __LINE__, __FUNCTION__);
    common.printWelcome();

    // Create a new customer
    Banking::Types::CUSTOMER_ID customerId = 1;
    Banking::Customer customer(customerId, "John Doe", "123 Main St", "555-1234", "john.doe@example.com");
    std::deque<std::unique_ptr<Banking::Customer>> customers;
    customers.push_back(std::make_unique<Banking::Customer>(customer));
    //customers[0]->printInfo();
    logger.info("Customer created: " + customers[0]->toString(), __FILE__, __LINE__, __FUNCTION__);
    logger.debug("Debugging customer creation", __FILE__, __LINE__, __FUNCTION__);
    logger.warn("This is a warning message", __FILE__, __LINE__, __FUNCTION__);
    logger.error("This is an error message", __FILE__, __LINE__, __FUNCTION__);
    std::thread producerThread(ProduceCustomerData, std::ref(customers), 2, 10000,std::ref(logger));
    std::thread consumerThread(ConsumeCustomerData, std::ref(customers), std::ref(logger));
    producerThread.join();
    consumerThread.join();
     std::unique_ptr<Banking::IDatabaseConnection> dbConnection = std::make_unique<Banking::OracleConnection>();
    if (dbConnection->connect(appConfig.getDBUser(), appConfig.getDBPassword(), appConfig.getDBConnectString()))
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
    logger.info("Banking Application finished", __FILE__, __LINE__, __FUNCTION__);
    return 0;

   
}
