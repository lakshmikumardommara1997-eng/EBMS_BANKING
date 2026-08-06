#include "CustomerManager.h"
#include <iostream>

namespace Banking
{
    CustomerManager::CustomerManager(std::shared_ptr<CustomerService> service)
        : m_service(std::move(service))
    {
    }

    bool CustomerManager::registerCustomer(const Customer& customer)
    {
        // Business rules could be applied here (e.g., uniqueness checks)
        // For now delegate to service
        return m_service->createCustomer(customer);
    }

    bool CustomerManager::modifyCustomer(const Customer& customer)
    {
        // Additional business validations could be added here
        return m_service->updateCustomer(customer);
    }

    bool CustomerManager::removeCustomer(Banking::Types::CUSTOMER_ID id)
    {
        // Potential business checks before deletion can be placed here
        return m_service->deleteCustomer(id);
    }
}
