/*
 * Example usage of Query Builder and Parameter Binding
 * 
 * This file demonstrates best practices for using the parameterized query system
 * to safely execute SQL queries with variable data.
 */

#include "Oracleconnection.h"
#include "ConnectionPool.h"
#include "IConnectionPool.h"
#include "Query.h"
#include "QueryBuilder.h"

// Example 1: Direct parameterized query using Query class
void example_direct_query(Banking::IDatabaseConnection& connection)
{
    // Create a parameterized query
    Banking::Query query("INSERT INTO users (id, name, balance) VALUES (?, ?, ?)");
    query.bind(101)                      // Bind integer ID
         .bind("John Doe")               // Bind string name
         .bind(5000.50);                 // Bind double balance

    if (connection.executeQuery(query))
    {
        std::cout << "User inserted successfully" << std::endl;
    }
    else
    {
        std::cout << "Failed to insert user" << std::endl;
    }
}

// Example 2: Using QueryBuilder for SELECT queries
void example_select_query(Banking::IDatabaseConnection& connection)
{
    Banking::QueryBuilder builder;
    Banking::Query query = builder
        .select("id, name, balance")
        .from("users")
        .where("balance > ?")
        .and_("status = ?")
        .orderBy("balance", false)  // descending
        .build();

    query.bind(1000.0)      // balance > 1000
         .bind("ACTIVE");   // status = 'ACTIVE'

    if (connection.executeQuery(query))
    {
        std::cout << "Query executed successfully" << std::endl;
    }
}

// Example 3: UPDATE query with parameter binding
void example_update_query(Banking::IDatabaseConnection& connection)
{
    Banking::QueryBuilder builder;
    Banking::Query query = builder
        .update("accounts")
        .set("balance", "?")
        .set("last_updated", "?")
        .where("account_id = ?")
        .build();

    query.bind(7500.00)     // new balance
         .bind(std::time(nullptr))  // current timestamp
         .bind(12345);       // account ID

    if (connection.executeQuery(query))
    {
        if (connection.commit())
        {
            std::cout << "Account updated and committed" << std::endl;
        }
    }
}

// Example 4: DELETE query with conditions
void example_delete_query(Banking::IDatabaseConnection& connection)
{
    Banking::Query query("DELETE FROM logs WHERE created_date < ? AND status = ?");
    query.bind(std::time(nullptr) - (30 * 24 * 60 * 60))  // 30 days ago
         .bind("ARCHIVED");

    if (connection.executeQuery(query))
    {
        connection.commit();
    }
    else
    {
        connection.rollback();
    }
}

// Example 5: Transaction with multiple queries
void example_transaction(Banking::IConnectionPool& pool)
{
    auto connection = pool.acquireConnection();
    if (!connection || !connection->isConnected())
    {
        std::cerr << "Failed to acquire connection" << std::endl;
        return;
    }

    if (!connection->beginTransaction())
    {
        std::cerr << "Failed to begin transaction" << std::endl;
        pool.releaseConnection(std::move(connection));
        return;
    }

    try
    {
        // Query 1: Debit from account
        Banking::Query debitQuery("UPDATE accounts SET balance = balance - ? WHERE account_id = ?");
        debitQuery.bind(500.0).bind(101);
        
        if (!connection->executeQuery(debitQuery))
        {
            throw std::runtime_error("Debit failed");
        }

        // Query 2: Credit to another account
        Banking::Query creditQuery("UPDATE accounts SET balance = balance + ? WHERE account_id = ?");
        creditQuery.bind(500.0).bind(102);
        
        if (!connection->executeQuery(creditQuery))
        {
            throw std::runtime_error("Credit failed");
        }

        // Query 3: Log the transaction
        Banking::Query logQuery(
            "INSERT INTO transaction_log (from_account, to_account, amount, status) VALUES (?, ?, ?, ?)"
        );
        logQuery.bind(101)
                .bind(102)
                .bind(500.0)
                .bind("SUCCESS");
        
        if (!connection->executeQuery(logQuery))
        {
            throw std::runtime_error("Logging failed");
        }

        // Commit all changes
        if (connection->commit())
        {
            std::cout << "Transaction committed successfully" << std::endl;
        }
        else
        {
            throw std::runtime_error("Commit failed");
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Transaction error: " << e.what() << std::endl;
        connection->rollback();
    }

    pool.releaseConnection(std::move(connection));
}

// Example 6: Handling NULL values
void example_null_binding(Banking::IDatabaseConnection& connection)
{
    Banking::Query query("INSERT INTO users (id, name, email) VALUES (?, ?, ?)");
    query.bind(201)
         .bind("Jane Smith")
         .bindNull();  // email is NULL

    if (connection.executeQuery(query))
    {
        connection.commit();
    }
}

// Example 7: Reusing a query with different parameters (IMPORTANT: Query must be reset)
void example_query_reuse(Banking::IDatabaseConnection& connection)
{
    Banking::Query query("SELECT * FROM users WHERE user_id = ?");

    std::vector<int> userIds = {1, 2, 3, 4, 5};
    
    for (int userId : userIds)
    {
        query.clearBindings();  // Clear previous bindings
        query.bind(userId);

        if (connection.executeQuery(query))
        {
            std::cout << "Retrieved user " << userId << std::endl;
        }
    }
}
