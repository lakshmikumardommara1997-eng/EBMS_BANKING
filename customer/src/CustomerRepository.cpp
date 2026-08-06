#include "CustomerRepository.h"
#include "../../database/include/QueryBuilder.h"
#include "../../database/include/Query.h"
#include <iostream>

namespace Banking
{
    CustomerRepository::CustomerRepository(Banking::IConnectionPool& pool)
        : m_pool(pool)
    {
    }

    bool CustomerRepository::addCustomer(const Customer& customer)
    {
        auto conn = m_pool.acquireConnection();
        if (!conn || !conn->isConnected())
            return false;

        QueryBuilder qb;
        Query q = qb.insert("customers")
                    .values({"id", "first_name", "last_name", "address", "phone", "email", "city", "state", "zip_code", "created_at"})
                    .build();

        q.bind(customer.getId())
         .bind(customer.getFirstName())
         .bind(customer.getLastName())
         .bind(customer.getAddress())
         .bind(customer.getPhoneNumber())
         .bind(customer.getEmail())
         .bind(customer.getCity())
         .bind(customer.getState())
         .bind(customer.getZipCode())
         .bind(customer.getCreatedAt());

        bool ok = false;
        if (conn->beginTransaction())
        {
            if (conn->executeQuery(q))
            {
                ok = conn->commit();
            }
            else
            {
                conn->rollback();
            }
        }
        m_pool.releaseConnection(std::move(conn));
        return ok;
    }

    bool CustomerRepository::updateCustomer(const Customer& customer)
    {
        auto conn = m_pool.acquireConnection();
        if (!conn || !conn->isConnected())
            return false;

        QueryBuilder qb;
        Query q = qb.update("customers")
                    .set("first_name", "?")
                    .set("last_name", "?")
                    .set("address", "?")
                    .set("phone", "?")
                    .set("email", "?")
                    .set("city", "?")
                    .set("state", "?")
                    .set("zip_code", "?")
                    .set("created_at", "?")
                    .where("id = ?")
                    .build();

        q.bind(customer.getFirstName())
         .bind(customer.getLastName())
         .bind(customer.getAddress())
         .bind(customer.getPhoneNumber())
         .bind(customer.getEmail())
         .bind(customer.getCity())
         .bind(customer.getState())
         .bind(customer.getZipCode())
         .bind(customer.getCreatedAt())
         .bind(customer.getId());

        bool ok = false;
        if (conn->beginTransaction())
        {
            if (conn->executeQuery(q))
            {
                ok = conn->commit();
            }
            else
            {
                conn->rollback();
            }
        }
        m_pool.releaseConnection(std::move(conn));
        return ok;
    }

    bool CustomerRepository::removeCustomer(Banking::Types::CUSTOMER_ID id)
    {
        auto conn = m_pool.acquireConnection();
        if (!conn || !conn->isConnected())
            return false;

        Query q("DELETE FROM customers WHERE id = ?");
        q.bind(static_cast<std::uint32_t>(id));

        bool ok = false;
        if (conn->beginTransaction())
        {
            if (conn->executeQuery(q))
            {
                ok = conn->commit();
            }
            else
            {
                conn->rollback();
            }
        }

        m_pool.releaseConnection(std::move(conn));
        return ok;
    }

    std::optional<Customer> CustomerRepository::getCustomerById(Banking::Types::CUSTOMER_ID id)
    {
        // NOTE: result set mapping is not implemented yet. Returning empty optional.
        // Implementing result fetching requires extending IDatabaseConnection to return results.
        (void)id;
        return std::nullopt;
    }

    std::vector<Customer> CustomerRepository::findCustomersByName(const std::string& name)
    {
        // NOTE: result fetching not implemented. Return empty vector for now.
        (void)name;
        return {};
    }
}
