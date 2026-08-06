#ifndef CUSTOMERMANAGER_H
#define CUSTOMERMANAGER_H

#include <memory>
#include "CustomerService.h"

namespace Banking
{
    class CustomerManager
    {
    public:
        explicit CustomerManager(std::shared_ptr<CustomerService> service);

        bool registerCustomer(const Customer& customer);
        bool modifyCustomer(const Customer& customer);
        bool removeCustomer(Banking::Types::CUSTOMER_ID id);

    private:
        std::shared_ptr<CustomerService> m_service;
    };
}

#endif // CUSTOMERMANAGER_H
