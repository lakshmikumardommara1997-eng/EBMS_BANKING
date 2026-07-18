# Sprint 02 - Multithreading, API Design, and Synchronization

## Sprint Title
Sprint 02 - Multithreading, API Design, and Synchronization

## Objectives
- Add a multi-threaded service layer to the EBMS architecture.
- Design a clean API boundary for banking operations.
- Implement synchronization mechanisms that protect shared data.

Why we are building this sprint
- Banking systems process concurrent operations from tellers, ATMs, and online services.
- This sprint solves the problem of shared account state being accessed by multiple threads.
- It fits in the architecture as the service layer and platform layer that coordinate requests and protect domain entities.

## Prerequisites
- Completed Sprint 01: Common Library.
- Basic Linux knowledge.
- Familiarity with `g++`, Makefile, and compilation.
- Awareness of namespaces, header/source separation, enums, and constants.

## C++ Concepts Covered

### Threads and `std::thread`
- Why: to execute independent tasks concurrently.
- Concept: a `std::thread` represents an active thread of execution.
- Example: `std::thread worker([] { /* work */ });`

### Synchronization primitives
- `std::mutex`: exclusive lock for protecting shared state.
- `std::lock_guard`: scoped lock that unlocks automatically.
- `std::unique_lock`: flexible lock for condition variables and deferred locking.
- `std::shared_mutex`: allows multiple readers, single writer.
- `std::condition_variable`: coordinate thread work and waiting.

### Race conditions and data races
- Why: they cause unpredictable behavior when multiple threads access the same object unsafely.
- How to avoid: protect all shared mutable data with synchronization primitives.

### Thread safety
- Explain thread-safe interface design.
- Immutable data is safe; mutable shared state needs locks.
- Prefer local variables inside threads.

### `std::atomic`
- Why: lightweight atomic operations for simple shared flags and counters.
- When to use: for counters, status flags, and lock-free state.

### Thread lifecycle management
- Create threads, join them, and avoid detached threads unless necessary.
- Use RAII wrappers where possible.

### API Design
- Design request/response models.
- Use clean function signatures with value semantics and `const` references.
- Separate interface from implementation.
- Keep APIs stable and easy to consume.

### Synchronization design patterns
- Monitor pattern: protect an object with a mutex and expose synchronized methods.
- Producer-consumer: use queues and condition variables.
- Reader-writer lock: use `std::shared_mutex` for high read concurrency.

### Exception safety in multithreaded code
- Threads should not silently swallow exceptions.
- Use `std::promise`/`std::future` or a safe wrapper to propagate exceptions.

## Enterprise Concepts

### Why enterprise projects use this approach
- Large systems must handle dozens to hundreds of concurrent requests.
- APIs provide a contract for services and prevent tight coupling between layers.
- Synchronization avoids data corruption and supports regulatory accuracy in banking.

### Common mistakes
- Protecting only part of the shared state.
- Using raw threads without lifecycle management.
- Creating ad-hoc shared data structures without clear ownership.
- Blocking UI or request threads on long-running tasks.

### Industry best practices
- Design thread-safe services with minimal locking scope.
- Separate API contracts from implementation details.
- Use thread pools for request processing rather than raw thread creation per request.
- Keep shared library interfaces stable.

### How companies implement this
- Financial services use service layer APIs for command and query operations.
- They wrap concurrency in components that enforce invariants.
- They often use thread pools, connection pools, and asynchronous messaging.

## Project Structure

```text
EBMS_BANKING/
  common/
    include/Common.h
    src/Common.cpp
  service/
    include/ServiceAPI.h
    src/AccountService.cpp
    src/ThreadPool.cpp
  model/
    include/Account.h
    src/Account.cpp
  app/
    BankingApp/
      main.cpp
  lib/
  Makefile
  Sprint-02-Multithreading-API-Synchronization.md
```

### Folder explanation
- `service/`: service layer APIs and multithreading support.
- `service/include/`: public service headers.
- `service/src/`: service implementations and thread support.
- `model/`: core domain objects used by services.
- `app/BankingApp/`: sample program to exercise the new APIs.

### File explanation
- `service/include/ServiceAPI.h`: defines the service interface and request models.
- `service/src/AccountService.cpp`: implements account operations with synchronization.
- `service/src/ThreadPool.cpp`: implements a basic thread pool for request execution.
- `model/include/Account.h`: defines the account entity and thread-safe methods.
- `model/src/Account.cpp`: implements account behavior.
- `app/BankingApp/main.cpp`: test harness for multithreaded API requests.

## Class Diagram

```text
+--------------------------+
| Account                 |
+--------------------------+
| - account_id_           |
| - balance_              |
| - mutex_                |
+--------------------------+
| + deposit(double)
| + withdraw(double)
| + getBalance() const
+--------------------------+
         ^
         |
+--------------------------+
| AccountService           |
+--------------------------+
| - account_store_         |
| - thread_pool_           |
+--------------------------+
| + processDeposit(...)
| + processWithdrawal(...)
| + processTransfer(...)
| + submitRequest(...)
+--------------------------+
         |
         v
+--------------------------+
| ThreadPool               |
+--------------------------+
| - workers_               |
| - task_queue_            |
| - mutex_                 |
| - condition_             |
+--------------------------+
| + enqueue(Task)
| + shutdown()
+--------------------------+
```

### Explanation
- `Account` owns its own state and a mutex for synchronization.
- `AccountService` provides API-level methods and uses a thread pool to execute requests.
- `ThreadPool` manages worker threads and a bounded/unbounded task queue.
- `AccountService` depends on `Account` and `ThreadPool`.
- Ownership is explicit: `ThreadPool` owns worker threads; `AccountService` owns domain objects or references.

## Module Dependency Diagram

```text
app/BankingApp --> service --> model
service --> common
model --> common
```

### Coupling and cohesion
- `app` uses service APIs, not domain internals.
- `service` is coupled to `model` and `common`, but `model` should not depend on `service`.
- `common` stays low-level and reusable.
- Cohesion is high when each module has a single responsibility.

## Implementation Plan

### Task 1: Create service and model directories
- Create `service/include`, `service/src`, `model/include`, `model/src`.
- Expected output: directories exist.

### Task 2: Define `Account` in model
- Create `model/include/Account.h` with `AccountType`, `AccountStatus`, and thread-safe methods.
- Use `std::mutex` as a private member.
- Expected output: header defines account entity.

### Task 3: Implement `Account` methods
- Create `model/src/Account.cpp`.
- Implement `deposit`, `withdraw`, `getBalance`, and `transfer` if needed.
- Use `std::lock_guard<std::mutex>`.
- Expected output: account behavior compiles and is thread-safe.

### Task 4: Define service API
- Create `service/include/ServiceAPI.h`.
- Declare request/response structs and `AccountService` interface.
- Use `std::shared_ptr<Account>` where needed.
- Expected output: clear API contract for service consumers.

### Task 5: Create a thread pool
- Create `service/src/ThreadPool.cpp` and header in `service/include/ThreadPool.h`.
- Implement worker threads, task queue, enqueue, and shutdown.
- Use `std::condition_variable` and `std::mutex`.
- Expected output: a reusable thread pool implementation.

### Task 6: Implement `AccountService`
- Use the thread pool to process requests asynchronously.
- Protect shared account container with a mutex.
- Use `std::future` or callback results to deliver outcomes.
- Expected output: service can accept concurrent deposit and withdrawal requests.

### Task 7: Add demo app
- Update `app/BankingApp/main.cpp`.
- Create a few account objects, submit parallel requests, and print results.
- Expected output: app demonstrates concurrent operations without data corruption.

### Task 8: Update Makefile
- Add build rules for `model` and `service` shared libraries if desired.
- Ensure `-pthread` is included for threading support.
- Expected output: `make` compiles new modules into libraries/binaries.

### Task 9: Build and verify
- Run `make clean` and `make`.
- Run the demo app and confirm consistent balances across threads.
- Expected output: no crash, and results reflect correct synchronization.

## Makefile Changes

### New variables
- `CPPFLAGS` or `CXXFLAGS`: include `-pthread`, `-std=c++20`, `-Wall`, `-Wextra`.
- `SERVICE_INCLUDE`: add `service/include`.
- `MODEL_INCLUDE`: add `model/include`.

### Threading-specific flags
- `-pthread`: required for linking the C++ threading library and platform support.

### New targets
- `model`: build `lib/libModel.so` if domain code is shared.
- `service`: build `lib/libService.so` that depends on `libCommon.so` and `libModel.so`.
- `app`: build the sample executable linking to the service library.

### Why these changes
- The build must compile service and model separately for large-scale maintenance.
- Shared library boundaries mimic enterprise architecture.
- `-pthread` is necessary for any code using `std::thread` or synchronization primitives.

## Build Process

### Compilation
- Compile `model/src/Account.cpp` into `Account.o`.
- Compile `service/src/ThreadPool.cpp` and `service/src/AccountService.cpp` into object files.
- Compile `app/BankingApp/main.cpp`.

### Shared libraries
- Link `lib/libModel.so` from model objects.
- Link `lib/libService.so` from service objects and dependencies.

### Executable linking
- Link `bin/BankingSystem` against `-lService -lModel -lCommon`.
- Ensure runtime can locate `libCommon.so`, `libModel.so`, and `libService.so`.

### Diagrams

Compile stage:
```text
Account.cpp -> Account.o
ThreadPool.cpp -> ThreadPool.o
AccountService.cpp -> AccountService.o
main.cpp -> main.o
```

Link stage:
```text
Account.o -> libModel.so
ThreadPool.o + AccountService.o -> libService.so
main.o + libService.so + libModel.so + libCommon.so -> bin/BankingSystem
```

Runtime stage:
```text
BankingSystem -> loads libService.so -> loads libModel.so -> loads libCommon.so
```

## Expected Console Output

Sample output after running the demo:

```text
Starting EBMS API service with 4 worker threads.
Submitting 8 concurrent requests.
Request 1 completed: deposit 100.00 to account A1001.
Request 2 completed: withdraw 50.00 from account A1001.
Request 3 completed: transfer 200.00 from A1001 to A1002.
Final balance A1001: 850.00
Final balance A1002: 1200.00
All requests completed successfully.
```

## Common Errors

### Compiler Errors
- missing `#include <thread>` or `<mutex>`.
- `std::lock_guard` not matching the mutex type.
- function signature mismatch between header and source.

### Linker Errors
- undefined reference to `pthread_create` or `pthread_join`: forgot `-pthread`.
- `undefined reference` to service symbols: library not linked or wrong order.

### Runtime Errors
- deadlock: two threads wait on each other due to nested lock order.
- data race: inconsistent output or corrupted balance values.
- exception thrown in worker thread and lost: no propagation mechanism.

### Shared Library Errors
- runtime loader cannot find the service or model shared library.
- version mismatch when running old binary with updated libraries.

### Make Errors
- missing separator due to spaces instead of tabs in recipe lines.
- dependency order wrong, causing stale builds.

## Debugging Guide

### `gdb`
- Start with `gdb bin/BankingSystem`.
- Set breakpoints in `Account::deposit` or `AccountService::processDeposit`.
- Use `run` and `bt` when a crash occurs.

### `valgrind` (optional)
- Use `valgrind ./bin/BankingSystem` to detect memory issues.
- Threaded programs may need `--tool=memcheck --trace-children=yes`.

### `nm`
- Inspect `lib/libService.so` for exported symbols.
- Check that service APIs are visible if they are intended to be public.

### `ldd`
- Use `ldd bin/BankingSystem` to verify runtime library resolution.

### `objdump` / `readelf`
- Use `readelf -d bin/BankingSystem` and `nm` to confirm dependencies.

### `make`
- Use `make -n` to preview the build steps.
- Use `make clean` to remove old artifacts.

## Coding Standards

### Threading standards
- Keep locks as narrow as possible.
- Prefer member mutexes inside synchronized objects.
- Avoid calling user-defined callbacks while holding a lock.
- Do not use `using namespace std;` in headers.

### API standards
- Keep request/response structures simple and immutable.
- Prefer value semantics for API parameters.
- Use `std::string` and avoid raw string pointers.
- Document every API method.

### Synchronization standards
- Use `std::lock_guard` when exclusive access is enough.
- Use `std::unique_lock` with condition variables.
- Use `std::shared_mutex` for read-mostly state.
- Avoid global variables for shared data.

## Best Practices

### Enterprise practices
- Separate the concurrency implementation from business logic.
- Keep API contracts stable.
- Build the thread pool as a reusable platform service.
- Design for observability: add logging around request submission and completion.

### Why
- This reduces coupling and makes debugging easier.
- An explicit API boundary makes future networking or microservices integration straightforward.
- Reusable thread pool code will be important for later database and networking sprints.

## Exercises

### Easy
- Update `ServiceAPI.h` to add a request type for `GetBalance`.
- Add a thread-safe `getBalance()` call to `Account`.

### Medium
- Add a `TransferRequest` that locks source and destination accounts in a safe order.
- Implement `std::atomic<int>` for a request counter.

### Hard
- Extend `ThreadPool` to support task priorities.
- Add a `shutdown()` method that waits for queued tasks to finish.

### Challenge
- Design a `RequestContext` struct that carries request metadata and supports cancellation.
- Add a callback mechanism that reports results back to the caller.

## Interview Questions

### Beginner
- What is a thread in C++?
- Why do we need `std::mutex`?
- What is `std::lock_guard`?

### Intermediate
- How does `std::condition_variable` work?
- What is the difference between `std::mutex` and `std::shared_mutex`?
- How do you avoid deadlock?

### Senior
- How do you design a thread-safe service API?
- When would you use `std::async` vs. a thread pool?
- What are the risks of shared mutable state?

### Architect
- How should threading and API layers be separated in an enterprise system?
- How does concurrency influence modular shared library design?
- What monitoring and observability capabilities should accompany a threaded service?

## Sprint Checklist
- [ ] Create `service/` and `model/` directories
- [ ] Implement `model/include/Account.h`
- [ ] Implement `model/src/Account.cpp`
- [ ] Implement `service/include/ServiceAPI.h`
- [ ] Implement `service/include/ThreadPool.h`
- [ ] Implement `service/src/ThreadPool.cpp`
- [ ] Implement `service/src/AccountService.cpp`
- [ ] Update `app/BankingApp/main.cpp`
- [ ] Update `Makefile` with `-pthread` and new targets
- [ ] Build shared libraries and app
- [ ] Run concurrent banking requests successfully

## Preparation For Next Sprint
- This sprint adds concurrency and service APIs, which are essential for transaction processing.
- The next sprint should add networking or database persistence using these thread-safe services.
- The thread pool and service API will become the foundation for asynchronous request handling in later sprints.
