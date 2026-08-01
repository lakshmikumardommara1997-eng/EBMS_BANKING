#include "Common.h"
#include "Customer.h"
#include <vector>
#include <iostream>
#include <memory>
#include "Logger.h"



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
    std::vector<std::unique_ptr<Banking::Customer>> customers;
    customers.push_back(std::make_unique<Banking::Customer>(customer));
    //customers[0]->printInfo();
    logger.info("Customer created: " + customers[0]->toString(), __FILE__, __LINE__, __FUNCTION__);
    logger.debug("Debugging customer creation", __FILE__, __LINE__, __FUNCTION__);
    logger.warn("This is a warning message", __FILE__, __LINE__, __FUNCTION__);
    logger.error("This is an error message", __FILE__, __LINE__, __FUNCTION__);
    return 0;
}
