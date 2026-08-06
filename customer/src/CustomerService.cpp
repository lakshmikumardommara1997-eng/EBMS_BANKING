#include "CustomerService.h"
#include <iostream>

namespace Banking
{
    CustomerService::CustomerService(std::shared_ptr<ICustomerRepository> repo)
        : m_repo(std::move(repo))
    {
    }

    bool CustomerService::validateEmail(const std::string& email) const
    {
        if (email.empty())
            return false;
        // simple email regex (not exhaustive)
        static const std::regex pattern(R"(([^@\s]+)@([^@\s]+)\.([^@\s]+))");
        return std::regex_match(email, pattern);
    }

    bool CustomerService::validatePhone(const std::string& phone) const
    {
        if (phone.empty())
            return false;
        // allow digits, spaces, dashes, parentheses
        static const std::regex pattern(R"(^[0-9\-\+\(\)\s]+$)");
        return std::regex_match(phone, pattern);
    }

    bool CustomerService::createCustomer(const Customer& customer)
    {
        if (!validateEmail(customer.getEmail()))
        {
            std::cerr << "Invalid email: " << customer.getEmail() << std::endl;
            return false;
        }
        if (!validatePhone(customer.getPhoneNumber()))
        {
            std::cerr << "Invalid phone number: " << customer.getPhoneNumber() << std::endl;
            return false;
        }

        return m_repo->addCustomer(customer);
    }

    bool CustomerService::updateCustomer(const Customer& customer)
    {
        if (!validateEmail(customer.getEmail()))
        {
            std::cerr << "Invalid email: " << customer.getEmail() << std::endl;
            return false;
        }
        if (!validatePhone(customer.getPhoneNumber()))
        {
            std::cerr << "Invalid phone number: " << customer.getPhoneNumber() << std::endl;
            return false;
        }

        return m_repo->updateCustomer(customer);
    }

    bool CustomerService::deleteCustomer(Banking::Types::CUSTOMER_ID id)
    {
        return m_repo->removeCustomer(id);
    }
}
