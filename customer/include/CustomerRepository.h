#ifndef CUSTOMERREPOSITORY_H
#define CUSTOMERREPOSITORY_H

#include "ICustomerRepository.h"
#include <memory>
#include "IConnectionPool.h"

namespace Banking
{
    class CustomerRepository : public ICustomerRepository
    {
    public:
        explicit CustomerRepository(Banking::IConnectionPool& pool);
        ~CustomerRepository() override = default;

        bool addCustomer(const Customer& customer) override;
        bool updateCustomer(const Customer& customer) override;
        bool removeCustomer(Banking::Types::CUSTOMER_ID id) override;
        std::optional<Customer> getCustomerById(Banking::Types::CUSTOMER_ID id) override;
        std::vector<Customer> findCustomersByName(const std::string& name) override;

    private:
        Banking::IConnectionPool& m_pool;
    };
}

#endif // CUSTOMERREPOSITORY_H
