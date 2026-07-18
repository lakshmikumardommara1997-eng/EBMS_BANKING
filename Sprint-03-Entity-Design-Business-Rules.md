# Sprint 03 - Entity Design and Business Rules

## Sprint Title
Sprint 03 - Entity Design and Business Rules

## Objectives
- Define the core domain entities for EBMS.
- Describe business rules and where they belong in the architecture.
- Map entities to classes, modules, and implementation responsibilities.

Why we are building this sprint
- A banking system needs a clear domain model before services and persistence are implemented.
- This sprint solves the problem of inconsistent data definitions and misplaced business logic.
- It fits into the architecture by providing the domain layer that the service layer, persistence layer, and API layer all depend upon.

## Core Entities

### 1. Customer
Represents a bank customer and their identity details.

Fields
- `CustomerID` (string or integer)
- `first_name`
- `last_name`
- `date_of_birth`
- `email`
- `phone_number`
- `address`
- `customer_type` (`Retail`, `Corporate`, `VIP`)
- `kyc_status` (`Pending`, `Verified`, `Rejected`)
- `status` (`Active`, `Inactive`, `Suspended`)
- `created_at`
- `updated_at`

Primary responsibilities
- Manage identity data.
- Validate required fields.
- Provide read-only access for services.

Where to implement
- `model/include/Customer.h`
- `model/src/Customer.cpp`

What belongs here
- Data fields and simple value validation.
- State transitions related to `status` and `kyc_status`.
- No transaction logic or persistence.

What does not belong here
- Deposit, withdrawal, or transfer logic.
- API request handling.
- Direct database code.

### 2. Account
Represents a bank account with balance and type.

Fields
- `AccountID`
- `customer_id` (owner reference)
- `account_type` (`Savings`, `Checking`, `Loan`, `Credit`)
- `balance`
- `currency` (`USD`, `EUR`, etc.)
- `status` (`Open`, `Closed`, `Frozen`)
- `created_at`
- `updated_at`
- `overdraft_limit` or `credit_limit`

Primary responsibilities
- Maintain a consistent balance.
- Enforce transaction-level invariants.
- Provide thread-safe updates.

Where to implement
- `model/include/Account.h`
- `model/src/Account.cpp`

What belongs here
- `deposit()` and `withdraw()` methods with input validation.
- `getBalance()` and status checks.
- Account-level business rules such as overdraft, minimum balance, or closed account checks.

What does not belong here
- High-level transfer orchestration between accounts.
- Customer creation or KYC handling.
- Persistence and API translations.

### 3. Transaction
Represents a record of money movement.

Fields
- `TransactionID`
- `transaction_type` (`Deposit`, `Withdrawal`, `Transfer`, `Fee`, `Interest`)
- `source_account_id`
- `destination_account_id` (optional)
- `amount`
- `currency`
- `timestamp`
- `status` (`Pending`, `Completed`, `Failed`)
- `reason`

Primary responsibilities
- Model a single financial event.
- Hold enough metadata for auditing and reconciliation.

Where to implement
- `model/include/Transaction.h`
- `model/src/Transaction.cpp`

What belongs here
- Serialization helpers for persistence.
- Simple validation of required fields.

What does not belong here
- Performing calculations or altering account state.
- Managing thread safety for account updates.

### 4. AuditRecord
Represents audit events required by enterprise banking.

Fields
- `AuditID`
- `event_type` (`AccountCreated`, `TransactionProcessed`, `LoginAttempt`, etc.)
- `actor_id` (user, system, or service)
- `target_id` (account or customer ID)
- `timestamp`
- `details`

Primary responsibilities
- Capture change events for later review.
- Support compliance and forensic analysis.

Where to implement
- `model/include/AuditRecord.h`
- `model/src/AuditRecord.cpp`

What belongs here
- Data structure only.
- Helper methods to format messages.

### 5. User / Employee (optional sprint extension)
Represents authenticated users of the system.

Fields
- `UserID`
- `username`
- `role` (`Teller`, `Manager`, `Auditor`, `Admin`)
- `status`
- `assigned_branch`

Primary responsibilities
- Support role-based access control.
- Track who performed operations.

Where to implement
- `model/include/User.h`
- `model/src/User.cpp`

## Business Rules

### Customer Rules
- A customer must have a valid email and phone number.
- KYC must be completed before the customer can open a new account.
- `customer_type` governs allowable account packaging.
- Inactive or suspended customers cannot perform transactions.

### Account Rules
- Only `Active` accounts may accept deposits and withdrawals.
- Closed accounts reject all financial operations.
- Savings accounts require a minimum balance; withdrawals below minimum may be denied.
- Checking accounts may allow overdraft up to `overdraft_limit`.
- Loan accounts can only accept payments, not withdrawals.
- Credit accounts may have `credit_limit` and must track available credit.

### Transaction Rules
- All transactions must be atomic: either complete or fail without partial state change.
- Transfers require both source and destination accounts to be valid and active.
- Amounts must be positive and in valid currency.
- The source account must have sufficient funds (or credit) before withdrawal or transfer.
- Transaction status should be recorded as `Pending` until completion.

### Audit Rules
- Every account creation, account closure, deposit, withdrawal, and transfer must generate an audit entry.
- Audit records must include actor identity and a textual reason.
- Audit records should be immutable once created.

### Authorization Rules
- Teller can create customers and process deposits/withdrawals.
- Manager can approve large transfers and account exceptions.
- Auditor can read transactions and audit records, but cannot change account state.

## Business Logic Placement

### Domain layer logic
Implement business invariants in domain classes when they belong to a single entity and do not require coordination across multiple entities.

Examples
- `Account::withdraw(double amount)` checks status and balance.
- `Customer::isKycComplete()` returns whether the customer is verified.
- `Transaction::isValid()` verifies the type and amount.

### Service layer logic
Implement operations that involve orchestration or multiple entities.

Examples
- `AccountService::processTransfer(source, destination, amount)` locks two accounts, executes withdraw and deposit, and logs a transaction.
- `CustomerService::createCustomer()` validates customer data, sets initial `kyc_status`, and persists the new record.
- `AuditService::recordEvent()` stores audit metadata.

### API layer logic
Implement request translation, input validation, authentication, and authorization.

Examples
- Convert JSON or CLI input into request structs.
- Check `UserRole` before forwarding a deposit request to the service.
- Map service results into API responses.

### Persistence layer logic
Implement storage, retrieval, and mapping between in-memory entities and persisted formats.

Examples
- `AccountRepository::save(Account const&)`
- `CustomerRepository::load(customer_id)`
- `TransactionRepository::append(Transaction const&)`

## Entity-to-Class Mapping

### Customer entity
- `model/include/Customer.h`
- `model/src/Customer.cpp`
- `service/include/CustomerService.h`
- `service/src/CustomerService.cpp`
- `database/include/CustomerRepository.h`
- `database/src/CustomerRepository.cpp`

### Account entity
- `model/include/Account.h`
- `model/src/Account.cpp`
- `service/include/AccountService.h`
- `service/src/AccountService.cpp`
- `database/include/AccountRepository.h`
- `database/src/AccountRepository.cpp`

### Transaction entity
- `model/include/Transaction.h`
- `model/src/Transaction.cpp`
- `service/include/TransactionService.h`
- `service/src/TransactionService.cpp`
- `database/include/TransactionRepository.h`
- `database/src/TransactionRepository.cpp`

### AuditRecord entity
- `model/include/AuditRecord.h`
- `model/src/AuditRecord.cpp`
- `service/include/AuditService.h`
- `service/src/AuditService.cpp`
- `database/include/AuditRepository.h`
- `database/src/AuditRepository.cpp`

## Logical Class Responsibilities

### `Customer`
Responsibility: hold customer identity and state.
Methods:
- `bool isKycComplete() const;`
- `bool isActive() const;`
- `void approveKyc();`
- `void suspend();`

### `Account`
Responsibility: hold account state, protect balance, and perform account-specific validation.
Methods:
- `void deposit(double amount);`
- `void withdraw(double amount);`
- `double getBalance() const;`
- `AccountStatus getStatus() const;`
- `bool canWithdraw(double amount) const;`

### `Transaction`
Responsibility: represent a financial event and validate its shape.
Methods:
- `bool isTransfer() const;`
- `bool isValid() const;`

### `AccountService`
Responsibility: coordinate account operations and apply cross-account rules.
Methods:
- `void createAccount(CustomerID customer_id, AccountType type);`
- `void processDeposit(AccountID account_id, double amount);`
- `void processWithdrawal(AccountID account_id, double amount);`
- `void processTransfer(AccountID source, AccountID destination, double amount);`

### `TransactionService`
Responsibility: log and persist transactions, coordinate status updates.
Methods:
- `TransactionID submitTransaction(Transaction const& transaction);`
- `void finalizeTransaction(TransactionID id, TransactionStatus status);`
- `std::vector<Transaction> getTransactionHistory(AccountID account_id);`

### `AuditService`
Responsibility: capture audit events for compliance.
Methods:
- `void logAccountCreated(Account const& account, UserID actor_id);`
- `void logTransactionProcessed(Transaction const& transaction, UserID actor_id);`

## Implementation Guidance

### 1. Start with the model layer
- Define entities in `model/include/`.
- Keep methods focused on the entity itself.
- Use `enum class` for statuses and types.

### 2. Add business rules to methods
- In `Account::withdraw`, implement:
  - status check
  - positive amount check
  - sufficient balance or overdraft check
- In `Account::deposit`, implement:
  - status check
  - positive amount check

### 3. Keep orchestration in services
- In `AccountService::processTransfer`, do not alter balances directly.
- Acquire locks on both account objects in a consistent order to prevent deadlock.
- Call `withdraw()` on source and `deposit()` on destination.
- Create a `Transaction` record and pass it to transaction handling.

### 4. Use repositories for persistence
- Keep domain classes free of file or database code.
- Have repositories accept and return entity objects.
- Example: `std::optional<Account> AccountRepository::find(AccountID account_id);`

### 5. Add validation helpers in common
- Put shared validation utilities in `common/include/Common.h` or `utils/Validation.h`.
- Example: `bool isValidCurrency(std::string const& currency);`

## Example Business Rule Implementations

### Account withdrawal rule
- `Account::withdraw(double amount)` should:
  - throw if `amount <= 0`
  - throw if `status != Active`
  - if `account_type == Savings` and `balance - amount < minimum_balance`, reject
  - if `account_type == Checking` and `balance - amount < -overdraft_limit`, reject
  - subtract amount from balance

### Transfer orchestration rule
- `AccountService::processTransfer(source, destination, amount)` should:
  - validate that source and destination are different
  - load both accounts
  - check both accounts are active
  - lock accounts in ID order
  - withdraw from source
  - deposit into destination
  - create and persist a transaction record
  - create and persist an audit record

### Customer creation rule
- `CustomerService::createCustomer(data)` should:
  - validate required personal data
  - set `kyc_status = Pending`
  - set `status = Active` or `Inactive` depending on business policy
  - persist the customer and return `CustomerID`

### Audit rule
- `AuditService` should be called after every meaningful state change.
- The service should not modify account balance or customer data; it only records.

## Where to implement each class

### `model/`
- Entities and domain behavior.
- No persistence code.
- No service orchestration.

### `service/`
- Business use cases involving multiple entities.
- Thread safety at a higher level.
- Transaction creation and audit invocation.

### `database/`
- Repositories and storage adapters.
- Mapping entities to persisted formats.
- Optional adapter interfaces for SQLite, PostgreSQL, and Oracle.

### `common/` or `utils/`
- Shared constants, enums, validation functions, and conversion helpers.
- Reusable components across model, service, and repository layers.

## Class Relationships and Ownership

### Ownership model
- `AccountService` owns or references repositories.
- `Account` does not own repositories or services.
- `Transaction` is owned by the service until persisted.
- `AuditService` owns audit storage mechanisms.

### Dependencies
- `service` depends on `model` and `common`.
- `database` depends on `model` and `common`.
- `app` depends on `service` and `common`.

## Practical Implementation Notes

### Use `enum class` for types and statuses
- Example: `enum class AccountStatus { Open, Closed, Frozen };`
- Example: `enum class TransactionType { Deposit, Withdrawal, Transfer };`

### Use `struct` for simple data carriers
- Request and response models can be plain structs.
- Example: `struct DepositRequest { AccountID account_id; double amount; };`

### Keep methods small and testable
- One responsibility per method.
- Example: `Account::canWithdraw(double amount) const` separate from `Account::withdraw`.

### Avoid business logic leakage into persistence
- Example: do not compute minimum balance in `AccountRepository`.
- The repository should only save or load values.

## Summary
This sprint defines the core banking entities, places business rules in the correct layer, and maps classes to modules:
- `Customer`, `Account`, `Transaction`, `AuditRecord` live in `model/`
- `AccountService`, `CustomerService`, `TransactionService`, `AuditService` live in `service/`
- Repositories live in `database/`
- Shared types and validation helpers live in `common/` or `utils/`

By following these guidelines, you keep the domain clean, maintainable, and ready for later expansions such as threading, persistence, networking, and database integration.
