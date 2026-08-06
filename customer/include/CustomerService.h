#ifndef CUSTOMERSERVICE_H
#define CUSTOMERSERVICE_H

#include <memory>
#include <regex>
#include "ICustomerRepository.h"

namespace Banking
{
    class CustomerService
    {
    public:
        explicit CustomerService(std::shared_ptr<ICustomerRepository> repo);

        bool createCustomer(const Customer& customer);
        bool updateCustomer(const Customer& customer);
        bool deleteCustomer(Banking::Types::CUSTOMER_ID id);

        bool validateEmail(const std::string& email) const;
        bool validatePhone(const std::string& phone) const;

    private:
        std::shared_ptr<ICustomerRepository> m_repo;
    };
}

#endif // CUSTOMERSERVICE_H
