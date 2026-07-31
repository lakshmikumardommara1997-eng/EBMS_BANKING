#include "Common.h"
#include "Customer.h"
#include <vector>
#include <iostream>
#include <memory>
#include "Logger.h"


int main()
{
    Banking::Common common;
    Banking::Logger::getInstance().init();
    Banking::Logger& logger = Banking::Logger::getInstance();
    
    logger.info("Starting Banking Application", __FILE__, __LINE__, __FUNCTION__);
    common.printWelcome();

    // Create a new customer
    Banking::Types::CUSTOMER_ID customerId = 1;
    Banking::Customer customer(customerId, "John Doe", "123 Main St", "555-1234", "john.doe@example.com");
    std::vector<std::unique_ptr<Banking::Customer>> customers;
    customers.push_back(std::make_unique<Banking::Customer>(customer));
    customers[0]->printInfo();

    return 0;
}
