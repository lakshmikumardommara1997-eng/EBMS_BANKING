# Enterprise Banking System Specification

## Purpose
This document defines the architecture, functional requirements, and implementation roadmap for an enterprise-level C++ banking system. It is designed to help you implement the system step by step while applying industry-level coding standards, including constants, enums, macros, and modular architecture.

## Goals
- Build a reusable, modular banking system in C++
- Learn advanced C++ concepts: classes, inheritance, polymorphism, templates, RAII, smart pointers, and design patterns
- Apply industry coding standards: constants, `enum class`, macros, naming conventions, clear module separation, and error handling
- Keep the system extensible and realistic for enterprise banking

## Recommended Architecture

### 1. Layered Architecture
- `UI / App` layer: user interface and experience
- `Service` layer: business rules, validation, transaction orchestration
- `Domain` layer: core banking entities and behavior
- `Persistence` layer: data storage and retrieval
- `Common / Utilities` layer: shared constants, logging, error handling

### 2. Suggested Modules
- `account/`: account data and account operations
- `customer/`: customer profile, authentication, KYC
- `database/`: persistence adapters and file or database access
- `Logger/`: logging utilities and audit trail
- `common/`: shared definitions, macros, constants, error codes
- `apps/BankingApp/`: main application entrypoint and demo flows
- `utils/`: helper functions, date/time utilities, validation utilities

## Core Domain Concepts

### Entities
- `Customer`
  - `CustomerID`
  - `Name`, `Address`, `Email`, `Phone`
  - `KYCStatus`, `CustomerType`
- `Account`
  - `AccountID`
  - `AccountType` (`SAVINGS`, `CHECKING`, `LOAN`, `CREDIT`)
  - `Balance`, `Currency`, `Status`
- `Transaction`
  - `TransactionID`
  - `TransactionType` (`DEPOSIT`, `WITHDRAWAL`, `TRANSFER`, `PAYMENT`)
  - `Amount`, `Timestamp`, `SourceAccount`, `DestinationAccount`
- `AuditRecord`
  - `EventType`, `Timestamp`, `ActorID`, `Details`

### Business Rules
- Accounts cannot go below authorized overdraft limit
- Transfers require source and destination account validation
- Withdrawals require sufficient funds and status checks
- Customer data modification must be authorized and audited
- Transactions must be persisted atomically

## Industry Coding Standards

### Naming Conventions
- Classes and structs: `PascalCase`
- Functions and methods: `camelCase`
- Variables: `snake_case` for internal or `camelCase` for members
- Constants: `kCamelCase` for `static const`, `kUPPER_CASE` for macros
- Member variables: trailing underscore (`balance_`)
- Namespaces: lower_case

### Constants and Enums
- Use `constexpr` and `const` whenever possible
- Use `enum class` for strongly typed enumerations
- Prefer compile-time constants over preprocessor macros except for include guards and platform-specific build flags

Example:
```cpp
namespace banking {
    enum class AccountType {
        Savings,
        Checking,
        Loan,
        Credit
    };

    constexpr double kDefaultSavingsInterest = 0.035;
    static constexpr int kMaxCustomerNameLength = 100;
}
```

### Macros
- Use macros only for include guards and compile-time feature toggles
- Avoid functional macros in business logic

Example:
```cpp
#ifndef BANKING_SYSTEM_COMMON_H
#define BANKING_SYSTEM_COMMON_H

#define BANKING_SYSTEM_VERSION "1.0.0"

#endif // BANKING_SYSTEM_COMMON_H
```

### Error Handling
- Use exceptions for recoverable runtime errors
- Create domain-specific exception types
- Use `std::optional`, `std::variant`, or `Expected<T, E>` style handling for safe outcomes

## Suggested Class Design

### Account Class
Responsibilities:
- store and update balances
- validate deposits, withdrawals, transfers
- expose account status and type

Key methods:
- `deposit(double amount)`
- `withdraw(double amount)`
- `transferTo(Account& destination, double amount)`
- `getBalance() const`

### Customer Class
Responsibilities:
- manage customer profile data
- validate identity and KYC
- link accounts to a customer

Key methods:
- `addAccount(std::shared_ptr<Account> account)`
- `removeAccount(AccountID account_id)`
- `isKycComplete() const`

### Transaction Class / Factory
Responsibilities:
- represent a single transaction event
- create records for deposit, withdrawal, transfer

Key fields:
- `transaction_type_`
- `amount_`
- `source_account_id_`
- `destination_account_id_`
- `timestamp_`

### Service Layer
Implement separate services for business logic:
- `AccountService`
- `CustomerService`
- `TransactionService`
- `ReportingService`

Each service should expose public methods but keep internal validation private.

### Persistence Layer
Keep storage code out of business logic.
- `IDataStore` interface or abstract base class
- concrete implementation: `FileDataStore`, `SQLiteDataStore`, or `JsonDataStore`
- serialization helpers for account/customer/transaction objects

## Step-by-Step Implementation Guide

### Stage 1: Foundation
1. Define shared constants and enums in `common/include/Common.h`:
   - `enum class AccountType`
   - `enum class TransactionType`
   - `enum class CustomerType`
   - `constexpr` values and string conversion helpers
2. Create core domain classes in `account/` and `customer/`:
   - `Account`, `SavingsAccount`, `CheckingAccount`
   - `Customer`, `CustomerProfile`
3. Implement constructors with member initializer lists
4. Add basic getters/setters and `const` correctness
5. Add unit tests if desired to verify class behavior

### Stage 2: Account and Transaction Logic
1. Implement `deposit`, `withdraw`, and `transferTo`
2. Add validation rules using private helper methods
3. Create `Transaction` class and transaction record creation
4. Add `TransactionType` and `TransactionStatus` enums
5. Log every transaction request through `Logger`

### Stage 3: Service Layer
1. Build `AccountService` with operations:
   - `createAccount`
   - `closeAccount`
   - `getAccountSummary`
2. Build `CustomerService` with operations:
   - `createCustomer`
   - `updateCustomer`
   - `findCustomer`
3. Build `TransactionService`:
   - `processDeposit`
   - `processWithdrawal`
   - `processTransfer`
   - `getTransactionHistory`
4. Enforce business rules in services, not in UI

### Stage 4: Persistence and Data Storage
1. Define persistence interfaces in `database/`:
   - `IDataStore`
   - `ICustomerRepository`
   - `IAccountRepository`
   - `ITransactionRepository`
2. Implement at least one storage backend:
   - file-based persistence using JSON, CSV, or binary
   - or use a simple embedded database adapter
3. Add save / load methods for accounts, customers, and transaction history
4. Ensure persistence uses RAII and closes resources cleanly

### Stage 5: Logging and Auditing
1. Add `Logger` module with severity levels:
   - `Info`, `Warning`, `Error`, `Audit`
2. Log all important events:
   - account creation
   - failed authentication
   - completed transfers
   - persistence errors
3. Include timestamps and context in log messages

### Stage 6: Command-Line App and Demo
1. Build a simple CLI in `apps/BankingApp/main.cpp`
2. Provide user flows:
   - create customer
   - open account
   - deposit / withdraw
   - transfer funds
   - print statement
3. Keep CLI logic thin: delegate core behavior to services

### Stage 7: Advanced Enhancements
1. Add role-based access control:
   - `enum class UserRole { Teller, Manager, Auditor }`
2. Add account limits and overdraft policies
3. Add concurrency support for transaction processing
   - protect shared account state with `std::mutex`
4. Add reporting features:
   - daily transaction summaries
   - account balance reports
   - audit trail export
5. Add unit tests and integration tests

## Implementation Checklist

### Domain Definitions
- [ ] `AccountType` enum
- [ ] `TransactionType` enum
- [ ] `CustomerType` enum
- [ ] `Status` enum for account and transaction states
- [ ] `constexpr` constants for business rules

### Modules
- [ ] `common/`
- [ ] `account/`
- [ ] `customer/`
- [ ] `database/`
- [ ] `Logger/`
- [ ] `apps/BankingApp/`
- [ ] `utils/`

### Coding Standards
- [ ] Use `constexpr` and `const`
- [ ] Use `enum class`
- [ ] Keep macros limited to include guards and compile-time flags
- [ ] Use smart pointers (`std::unique_ptr`, `std::shared_ptr`)
- [ ] Prefer interfaces and dependency injection
- [ ] Ensure `const` correctness on methods
- [ ] Keep functions small and single-purpose

## Suggested Folder and File Layout

```text
common/
  include/Common.h
  src/Common.cpp
account/
  include/Account.h
  src/Account.cpp
customer/
  include/Customer.h
  src/Customer.cpp
database/
  include/IDataStore.h
  src/FileDataStore.cpp
Logger/
  include/Logger.h
  src/Logger.cpp
apps/BankingApp/
  main.cpp
utils/
  include/Validation.h
  src/Validation.cpp
```

## Best Practices for C++ Implementation
- Use RAII for resource management
- Prefer `std::string` and `std::vector` over raw pointers
- Avoid global variables
- Use `explicit` constructors where appropriate
- Use `override` for virtual methods
- Write clear interface contracts
- Document public APIs with comments

## Example Enum and Constant Usage

```cpp
namespace banking {

enum class AccountStatus {
    Active,
    Suspended,
    Closed
};

enum class TransactionType {
    Deposit,
    Withdrawal,
    Transfer,
    Fee
};

constexpr double kMinimumBalanceSavings = 100.00;
constexpr double kMaximumTransferAmount = 100000.00;

#define BANKING_SYSTEM_BUILD_DATE __DATE__

}
```

## Next Steps
1. Start by implementing `common/include/Common.h` and `account/include/Account.h`
2. Add domain classes and validate business rules with unit tests
3. Build service classes and persistence adapters
4. Add logging and a small CLI demo
5. Extend with advanced enterprise features like audit trails, RBAC, and concurrency

---

This specification is intentionally modular and incremental so you can build your enterprise banking system step by step while practicing advanced C++ design and coding standards.