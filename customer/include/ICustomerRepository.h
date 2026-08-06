#ifndef ICUSTOMERREPOSITORY_H
#define ICUSTOMERREPOSITORY_H

#include <vector>
#include <optional>
#include "Customer.h"

namespace Banking
{
    class ICustomerRepository
    {
    public:
        virtual ~ICustomerRepository() = default;

        virtual bool addCustomer(const Customer& customer) = 0;
        virtual bool updateCustomer(const Customer& customer) = 0;
        virtual bool removeCustomer(Banking::Types::CUSTOMER_ID id) = 0;
        virtual std::optional<Customer> getCustomerById(Banking::Types::CUSTOMER_ID id) = 0;
        virtual std::vector<Customer> findCustomersByName(const std::string& name) = 0;
    };
}

#endif // ICUSTOMERREPOSITORY_H
