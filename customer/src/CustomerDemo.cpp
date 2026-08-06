#include <iostream>
#include <memory>

#include "CustomerRepository.h"
#include "CustomerService.h"
#include "CustomerManager.h"
#include "../../database/include/ConnectionPool.h"
#include "../../database/include/IConnectionPool.h"
#include "../../common/include/appconfig.h"

int main_demo()
{
    Banking::AppConfig& cfg = Banking::AppConfig::getInstance();
    cfg.LoadConfigFromFile("param/Ebms.cfg");

    auto pool = std::make_unique<Banking::ConnectionPool>();
    if (!pool->initialize(cfg.getDBUser(), cfg.getDBPassword(), cfg.getDBConnectString(), 3))
    {
        std::cerr << "Failed to initialize DB pool" << std::endl;
        return 1;
    }

    auto repo = std::make_shared<Banking::CustomerRepository>(*pool);
    auto service = std::make_shared<Banking::CustomerService>(repo);
    Banking::CustomerManager manager(service);

    Banking::Customer c(1, "John", "Doe", "123 Main St", "555-1234", "john@example.com");
    if (manager.registerCustomer(c))
    {
        std::cout << "Customer registered: " << c.toString() << std::endl;
    }
    else
    {
        std::cout << "Failed to register customer" << std::endl;
    }

    pool->shutdown();
    return 0;
}
