# Customer Flow Documentation

## Overview
This document describes how a `Customer` object moves through the application layers from the demo entry point through the repository and database.

## Layer Responsibilities

### 1. CustomerManager (Business Layer)
- Entry point for customer operations in the application.
- Contains business-level orchestration and policies.
- Delegates to `CustomerService` for validation and repository access.

### 2. CustomerService (Service Layer)
- Contains business rules and validation logic.
- Validates customer data before it is persisted.
- Calls repository methods after validation passes.

### 3. CustomerRepository (Data Access Layer)
- Responsible for persistence operations.
- Builds SQL queries using `QueryBuilder` and `Query`.
- Uses `IConnectionPool` to acquire/release database connections.
- Executes SQL with parameter binding through `IDatabaseConnection`.

## Call Chain for `registerCustomer`

1. `main_demo()` creates a `CustomerManager` with a `CustomerService` and `CustomerRepository`.
2. `main_demo()` constructs a `Customer` object:
   - `Banking::Customer c(1, "John", "Doe", "123 Main St", "555-1234", "john@example.com");`
3. `main_demo()` calls:
   - `manager.registerCustomer(c)`
4. `CustomerManager::registerCustomer(const Customer&)` calls:
   - `m_service->createCustomer(customer)`
5. `CustomerService::createCustomer(const Customer&)` does:
   - `validateEmail(customer.getEmail())`
   - `validatePhone(customer.getPhoneNumber())`
   - If validation succeeds, calls `m_repo->addCustomer(customer)`
6. `CustomerRepository::addCustomer(const Customer&)` does:
   - Acquire database connection: `m_pool.acquireConnection()`
   - Build insert SQL with `QueryBuilder`
   - Bind customer values into query parameters
   - Begin transaction: `conn->beginTransaction()`
   - Execute query: `conn->executeQuery(q)`
   - Commit or rollback depending on result
   - Release connection back into pool: `m_pool.releaseConnection(std::move(conn))`

## Data Flow Details

### Customer object fields used
- `customer.getId()`
- `customer.getFirstName()`
- `customer.getLastName()`
- `customer.getAddress()`
- `customer.getPhoneNumber()`
- `customer.getEmail()`
- `customer.getCity()`
- `customer.getState()`
- `customer.getZipCode()`
- `customer.getCreatedAt()`

### Query building and binding
- SQL statement is created by `QueryBuilder`
- Parameter placeholders `?` are used in SQL
- Actual values are bound with `Query::bind(...)`
- This prevents SQL injection and ensures typed parameter passing

## Transaction behavior
- `CustomerRepository` begins a transaction before executing the SQL
- If execution succeeds, it commits
- If execution fails, it rolls back
- This ensures data integrity for write operations

## Connection pool usage
- `ConnectionPool` provides a reusable database connection
- `CustomerRepository` acquires a connection for each operation
- After the operation, the connection is returned to the pool

## Notes
- Read operations such as `getCustomerById()` and `findCustomersByName()` are currently placeholders and do not yet map query results back to `Customer` objects.
- The current flow is focused on write operations: add, update, and remove.

## Example flow summary
```text
main_demo() -> CustomerManager::registerCustomer() -> CustomerService::createCustomer() -> CustomerRepository::addCustomer() -> DB connection -> Query execution -> commit -> release connection
```
