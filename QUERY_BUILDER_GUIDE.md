# Query Builder and Parameter Binding System

## Overview

The EBMS Banking System provides a comprehensive query builder and parameter binding system for safe, type-aware database queries. This system prevents SQL injection attacks and provides a clean, fluent API for constructing SQL statements.

## Key Components

### 1. IQuery Interface (`IQuery.h`)
Base interface defining the contract for queryable objects:
- `getSql()` - Returns the SQL string
- `getParameterCount()` - Number of bound parameters
- `getParameter(index)` - Retrieves parameter at given index
- `hasParameters()` - Checks if query has parameters

### 2. Query Class (`Query.h` / `Query.cpp`)
Direct SQL query implementation with parameter binding:
```cpp
Banking::Query query("INSERT INTO users (id, name, email) VALUES (?, ?, ?)");
query.bind(101)                    // int
     .bind("John Doe")             // std::string
     .bind("john@example.com");    // std::string
```

**Supported Types:**
- `int` - Integer values
- `double` - Floating point values
- `std::string` - Character/text data
- `bool` - Boolean values
- `std::time_t` - Timestamp values
- `nullptr` - NULL values via `bindNull()`

**Methods:**
- `bind(value)` - Bind parameter (chainable)
- `bindNull()` - Bind NULL value
- `clearBindings()` - Remove all bound parameters
- `reset()` - Clear bindings and reset state

### 3. QueryBuilder Class (`QueryBuilder.h` / `QueryBuilder.cpp`)
Fluent API for constructing SQL queries programmatically:

**SELECT Queries:**
```cpp
Banking::QueryBuilder builder;
Banking::Query query = builder
    .select("id, name, balance")
    .from("accounts")
    .where("balance > ?")
    .and_("status = ?")
    .orderBy("balance", false)
    .limit(10)
    .offset(5)
    .build();
```

**INSERT Queries:**
```cpp
Banking::QueryBuilder builder;
Banking::Query query = builder
    .insert("users")
    .values({"id", "name", "email"})
    .build();
```

**UPDATE Queries:**
```cpp
Banking::QueryBuilder builder;
Banking::Query query = builder
    .update("accounts")
    .set("balance", "?")
    .set("last_modified", "?")
    .where("account_id = ?")
    .build();
```

**DELETE Queries:**
```cpp
Banking::QueryBuilder builder;
Banking::Query query = builder
    .deleteFrom("logs")
    .where("created_date < ?")
    .and_("status = ?")
    .build();
```

**JOIN Support:**
```cpp
Banking::QueryBuilder builder;
Banking::Query query = builder
    .select("a.id, a.balance, c.name")
    .from("accounts a")
    .join("customers c", "a.customer_id = c.id", "INNER")
    .where("a.balance > ?")
    .build();
```

## Best Practices

### 1. Always Use Parameter Binding
❌ **DON'T** - SQL Injection Risk:
```cpp
std::string userId = getUserInput();
std::string sql = "SELECT * FROM users WHERE id = " + userId;
connection.executeQuery(sql);
```

✅ **DO** - Safe Parameter Binding:
```cpp
Banking::Query query("SELECT * FROM users WHERE id = ?");
query.bind(userId);
connection.executeQuery(query);
```

### 2. Handle Transactions Properly
```cpp
connection.beginTransaction();
try 
{
    query1.bind(value1);
    if (!connection.executeQuery(query1))
        throw std::runtime_error("Query 1 failed");
    
    query2.bind(value2);
    if (!connection.executeQuery(query2))
        throw std::runtime_error("Query 2 failed");
    
    connection.commit();
}
catch (const std::exception& e)
{
    connection.rollback();
    // Handle error
}
```

### 3. Always Release Connections Back to Pool
```cpp
auto connection = pool.acquireConnection();
// Use connection...
pool.releaseConnection(std::move(connection));  // Return to pool
```

### 4. Clear Bindings Before Reusing Queries
```cpp
Banking::Query query("SELECT * FROM users WHERE id = ?");

for (int id : ids)
{
    query.clearBindings();  // IMPORTANT: Clear previous bindings
    query.bind(id);
    connection.executeQuery(query);
}
```

### 5. Use Type-Safe Binding
The system uses C++17 `std::variant` for type safety:
```cpp
query.bind(123);              // Binds as INTEGER
query.bind(45.67);            // Binds as DOUBLE
query.bind("text");           // Binds as VARCHAR
query.bind(true);             // Binds as NUMBER (1 or 0)
query.bindNull();             // Binds as NULL
```

## Parameter Binding Details

### How Binding Works
1. Parameters are stored in order during `bind()` calls
2. Database uses positional binding (? in SQL becomes parameter 1, 2, 3, etc.)
3. Type information is preserved for proper Oracle type mapping
4. NULL values are handled explicitly via `bindNull()`

### ODPI-C Type Mapping
Internal mapping to Oracle types:
- `int` → `DPI_ORACLE_TYPE_NUMBER` / `DPI_NATIVE_TYPE_INT64`
- `double` → `DPI_ORACLE_TYPE_NATIVE_DOUBLE` / `DPI_NATIVE_TYPE_DOUBLE`
- `std::string` → `DPI_ORACLE_TYPE_VARCHAR` / `DPI_NATIVE_TYPE_BYTES`
- `bool` → `DPI_ORACLE_TYPE_NUMBER` / `DPI_NATIVE_TYPE_INT64` (0 or 1)
- `nullptr` → NULL-bound variable

## Error Handling

All methods report errors through the return values and ODPI-C error logging:

```cpp
Banking::Query query("SELECT * FROM users WHERE id = ?");
query.bind(invalidValue);

if (!connection.executeQuery(query))
{
    // Error logged to stderr via logOracleError()
    // Check application logs for detailed error message
}
```

## Performance Considerations

1. **Prepared Statements** - Each executeQuery creates a prepared statement
2. **Connection Pool** - Always acquire/release connections properly
3. **Transaction Scope** - Keep transactions short to avoid deadlocks
4. **Parameter Count** - No limit on number of parameters (depends on ODPI-C)

## Thread Safety

- **Query objects** - NOT thread-safe; create per-thread instances
- **Connection pooling** - Thread-safe with mutex protection
- **Individual connections** - Use one per transaction/thread

## Example: Complete Banking Transfer

```cpp
void transfer_funds(Banking::IConnectionPool& pool, int fromId, int toId, double amount)
{
    auto connection = pool.acquireConnection();
    if (!connection) return;

    connection.beginTransaction();
    try 
    {
        // Debit from source
        Banking::Query debit("UPDATE accounts SET balance = balance - ? WHERE id = ?");
        debit.bind(amount).bind(fromId);
        if (!connection.executeQuery(debit)) throw std::exception();

        // Credit to destination
        Banking::Query credit("UPDATE accounts SET balance = balance + ? WHERE id = ?");
        credit.bind(amount).bind(toId);
        if (!connection.executeQuery(credit)) throw std::exception();

        // Log transaction
        Banking::Query log("INSERT INTO logs (src, dst, amt, ts) VALUES (?, ?, ?, ?)");
        log.bind(fromId)
           .bind(toId)
           .bind(amount)
           .bind(std::time(nullptr));
        if (!connection.executeQuery(log)) throw std::exception();

        connection.commit();
    }
    catch (...)
    {
        connection.rollback();
    }
    
    pool.releaseConnection(std::move(connection));
}
```

## Future Enhancements

Potential improvements:
- Result set fetching and mapping
- Batch operations/bulk inserts
- Cursor-based result iteration
- ORM-like entity mapping
- Query logging and profiling
- Connection retry logic
- Statement caching
