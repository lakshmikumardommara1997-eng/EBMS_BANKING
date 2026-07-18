# Sprint 01 - Common Library

## Sprint Title
Sprint 01 - Common Library

## Objectives
- Build the foundation module for the Enterprise Banking Management System (EBMS).
- Define shared constants, enums, macros, and utility facilities used by all other modules.
- Establish project conventions and build integration for a shared library.

Why we are building this sprint
- A common library is the financial system equivalent of a strong foundation slab. It prevents duplicated definitions, inconsistent values, and subsystem drift.
- It solves the problem of shared definitions and application-wide policies being scattered across modules.
- It fits in the architecture as the base layer used by domain entities, services, logging, persistence, and applications.

## Prerequisites
- Basic knowledge of Linux commands.
- Familiarity with VS Code and editing files.
- Understanding of what a compiler does.
- Simple awareness of files and folders.

## C++ Concepts Covered

### Namespaces
- Why: avoid name collisions in a large codebase.
- How: wrap shared declarations in a namespace such as `ebms` or `ebms::common`.
- Example: `namespace ebms { namespace common { ... } }`

### Header Guards and `#pragma once`
- Why: prevent multiple inclusion of the same header.
- How: use `#ifndef`, `#define`, `#endif` or `#pragma once` at the top of header files.

### Separate Compilation
- Why: keep declarations in headers and implementations in `.cpp` files so build is scalable.
- How: header files declare interfaces, source files provide definitions.

### `constexpr`
- Why: enable compile-time constants and safer constant expressions.
- How: use `constexpr` for values that never change and should be evaluated at compile time.
- Example: `constexpr int kMaxCustomerNameLength = 128;`

### `enum class`
- Why: provide strongly typed enumerations with scoped names.
- How: define enums for account and transaction states, then use them explicitly.
- Example: `enum class AccountType { Savings, Checking, Loan, Credit };`

### `const` correctness
- Why: protect data from unintended modification and express intent.
- How: mark method parameters and member functions as `const` when they do not modify state.
- Example: `const std::string& getName() const;`

### References
- Why: pass large objects efficiently without copying.
- How: prefer `const Type&` for read-only function parameters.

### Pointers
- Why: represent ownership and optional data.
- How: use raw pointers only for non-owning references or legacy APIs.

### Templates
- Why: create reusable, type-safe code.
- How: implement small generic helpers in the common library.
- Example: `template <typename T> std::string toString(const T& value);`

### Smart Pointers
- Why: manage dynamic memory safely.
- How: use `std::unique_ptr` for exclusive ownership and `std::shared_ptr` for shared ownership.
- Note: this sprint will favor simple static data; smart pointers are introduced in principle.

### Move Semantics
- Why: allow efficient transfer of resources.
- How: use `std::move` and implement move constructors when necessary.
- Note: this sprint explains the concept; implementation examples remain lightweight.

### Exception Handling
- Why: report and manage runtime errors in a controlled way.
- How: use C++ exceptions for domain errors and unexpected states.
- Example: `throw std::runtime_error("Invalid account state");`

## Enterprise Concepts

### Why enterprise projects use this approach
- Shared libraries enable multiple products or modules to reuse the same core code.
- A common library promotes consistency, reduces bugs, and speeds development.
- In enterprise systems, a stable `common` module is typically maintained by an architecture or platform team.

### Common mistakes
- Putting business logic in the common library.
- Using macros for everything instead of `constexpr`, `enum class`, and typed constants.
- Creating too many global variables.
- Not documenting the common module contracts.

### Industry best practices
- Keep the `common` module minimal and stable.
- Export only what other modules need.
- Use clear naming conventions and avoid hidden dependencies.
- Document every shared type and constant.

### How companies implement this
- Define explicit `common` interfaces for enums, error codes, and shared utilities.
- Build the common library as a shared object (`.so`) consumed by application binaries.
- Version the library and avoid breaking changes lightly.

## Project Structure

Root tree for Sprint 01:

```text
EBMS_BANKING/
  common/
    include/
      Common.h
    src/
      Common.cpp
  lib/
  apps/
    BankingApp/
      main.cpp
  Makefile
  Sprint-01-Common-Library.md
```

### Folder explanation
- `common/`: contains the common library module.
- `common/include/`: public headers exported by the common library.
- `common/src/`: implementation of common utilities and definitions.
- `lib/`: output folder for built shared libraries.
- `apps/BankingApp/`: placeholder application that will link against the common library.
- `Makefile`: top-level build script that builds the common shared library and links a demo app.

### File explanation
- `common/include/Common.h`: public shared declarations, constants, enums, macros, and utility prototypes.
- `common/src/Common.cpp`: source file that defines non-inline common utilities.
- `apps/BankingApp/main.cpp`: small demo stub used to verify the common library link path.
- `Makefile`: build rules for compiling and linking the common library and demo.
- `Sprint-01-Common-Library.md`: this sprint guide.

## Class Diagram

Sprint 01 uses only one core module, so the class diagram is small.

```text
+-------------------+
| ebms::common      |
+-------------------+
| CommonConstants   |
| CommonEnums       |
| CommonUtilities   |
+-------------------+
        ^
        |
+-------------------+
| apps::BankingApp  |
+-------------------+
| main()            |
+-------------------+
```

### Explanation
- `ebms::common` is not a concrete class, but a namespace containing shared definitions.
- `CommonConstants`: holds compile-time constants like limits and default values.
- `CommonEnums`: defines strongly typed enums shared across the system.
- `CommonUtilities`: small helper functions, such as `toString` or validation helpers.
- `apps::BankingApp` depends on the common module to compile and link.

## Module Dependency Diagram

```text
apps/BankingApp --> common
```

### Coupling and cohesion
- Coupling: the app depends on the common module, but the common module should not depend on the app.
- Cohesion: common should only contain shared definitions and utilities, making it highly cohesive.
- This keeps the dependency graph simple and maintainable.

## Implementation Plan

### Task 1: Create include directory
- Create `common/include/`.
- Expected output: directory exists and is ready for header files.

### Task 2: Create src directory
- Create `common/src/`.
- Expected output: directory exists for implementation files.

### Task 3: Create `Common.h`
- Write header guard or `#pragma once`.
- Declare namespace `ebms::common`.
- Define at least two `enum class` types.
- Define at least two `constexpr` values.
- Declare one utility function prototype.

Expected output:
- `common/include/Common.h` contains a clean interface with enums and constants.

### Task 4: Create `Common.cpp`
- Include `Common.h`.
- Define the utility function declared in the header.

Expected output:
- `common/src/Common.cpp` compiles without errors.

### Task 5: Create `apps/BankingApp/main.cpp`
- Include `Common.h`.
- Use one enum and one constant.
- Print a small message.

Expected output:
- `apps/BankingApp/main.cpp` demonstrates linking against common.

### Task 6: Update or create `Makefile`
- Add rules for compiling `common` as shared library.
- Add rules for building `apps/BankingApp` executable.
- Set correct compiler and linker options.

Expected output:
- `make` produces `lib/libCommon.so` and `bin/BankingSystem`.

### Task 7: Build and run
- Run `make clean` then `make`.
- Execute the app to verify.

Expected output:
- Program prints a simple confirmation message from `main()`.

## Makefile Changes

### Important variables
- `CXX`: the compiler, e.g. `g++`.
- `CXXFLAGS`: compiler flags such as `-std=c++20`, `-Wall`, `-Wextra`, `-fPIC`.
- `LDFLAGS`: linker flags, e.g. `-shared` for a shared library.
- `LDLIBS`: libraries to link.
- `INCLUDE_DIRS`: include search paths for headers.

### Compiler options
- `-std=c++20`: enable modern C++20 features.
- `-Wall -Wextra`: enable warnings.
- `-Werror`: optional; treat warnings as errors for discipline.
- `-fPIC`: required for shared library position-independent code.
- `-Icommon/include`: add public header search path.

### Linker options
- `-shared`: create a shared object.
- `-Wl,-rpath,\$$ORIGIN/../lib`: optional runtime library search path.
- `-Llib`: search for libraries in `lib`.

### Targets
- `all`: default target that builds library and app.
- `common`: builds `lib/libCommon.so`.
- `banking_app`: builds `bin/BankingSystem`.
- `clean`: removes generated object files, libraries, and binaries.

### Dependencies
- `common` target depends on `common/src/Common.cpp` and `common/include/Common.h`.
- `banking_app` depends on `apps/BankingApp/main.cpp` and `lib/libCommon.so`.

### Why explain every item
- Variables keep the build portable and maintainable.
- Compiler flags enforce code quality and standard compliance.
- Shared library options are essential for producing `.so` files.
- Explicit dependencies prevent incorrect incremental builds.

## Build Process

### Compilation
- Each `.cpp` file is compiled into an `.o` object file.
- Example: `g++ -std=c++20 -Wall -Wextra -fPIC -Icommon/include -c common/src/Common.cpp -o common/src/Common.o`

### Object files
- `Common.o` stores compiled machine code and unresolved references.
- `main.o` stores app code referencing common symbols.

### Shared libraries
- `libCommon.so` combines object code and exports symbols.
- Built with `-shared` and `-fPIC`.

### Linking
- The executable links against `libCommon.so`.
- Dynamic loader resolves shared library references at runtime.

### Executable creation
- `g++ apps/BankingApp/main.o -Llib -lCommon -o bin/BankingSystem`

### Diagrams

Compile stage:
```text
common/src/Common.cpp -> Common.o
apps/BankingApp/main.cpp -> main.o
```

Link stage:
```text
Common.o + main.o -> libCommon.so + bin/BankingSystem
```

Runtime stage:
```text
bin/BankingSystem -> loads libCommon.so
```

## Expected Console Output

After Task 3 and Task 5, the expected output from running the app should be:

```text
EBMS Common Library initialized.
AccountType: Savings
Max customer name length: 128
```

If you build only the library and run `make`, you should see compilation messages and no error.

## Common Errors

### Compiler Errors
- `undefined reference`: function declared but not defined or object file missing from link.
- `multiple definition`: same symbol implemented in more than one source file.
- `no matching function`: function signature mismatch between declaration and call.

### Linker Errors
- `cannot find -lCommon`: library search path wrong or library name mismatch.
- `undefined reference to [0m...`: object file not linked or symbol missing from shared library.

### Runtime Errors
- `./bin/BankingSystem: error while loading shared libraries`: runtime search path missing or library not found.
- `Segmentation fault`: invalid pointer or memory corruption.

### Shared Library Errors
- `relocation R_X86_64_32S against [0m...`: missing `-fPIC` when building shared library.
- version mismatch: app links against older library ABI.

### GNU Make Errors
- `No rule to make target`: build rule or path incorrect.
- `*** missing separator`: tabs required in Makefile recipe lines.
- `Nothing to be done for 'all'`: dependencies are already up to date.

### How to debug them
- Re-run `make` with `VERBOSE=1` or inspect compiler command lines.
- Confirm header include paths with `-I` flags.
- Check object files exist in expected folders.
- Use `ldd bin/BankingSystem` to verify shared library load path.

## Debugging Guide

### `gdb`
- Use `gdb bin/BankingSystem` to start the debugger.
- Useful commands:
  - `run`
  - `break main`
  - `next`
  - `print variable`
  - `backtrace`

### `nm`
- Use `nm -C lib/libCommon.so | grep symbol` to inspect exported symbols.
- It helps confirm that the shared library exports the names you expect.

### `ldd`
- Use `ldd bin/BankingSystem` to list shared library dependencies.
- Verify that `libCommon.so` is resolved and points to the correct path.

### `objdump`
- Use `objdump -x lib/libCommon.so | less` to inspect headers and symbol tables.
- Useful for advanced diagnostics.

### `readelf`
- Use `readelf -d bin/BankingSystem` to examine dynamic dependencies.
- Use `readelf -s lib/libCommon.so | grep symbol` for symbol exports.

### `make`
- Use `make clean` to remove stale artifacts.
- Use `make -n` to preview build commands without executing them.
- Use `make VERBOSE=1` if you define verbose output in Makefile.

## Coding Standards

### Naming conventions
- Namespace names: `ebms`, `ebms::common`.
- Headers: `Common.h`.
- Classes: `PascalCase`.
- Functions: `camelCase`.
- Constants: `kCamelCase` for `constexpr`, `kUPPER_CASE` only for macros.
- Member variables: trailing underscore, e.g. `limit_`.

### Header organization
- Public headers in `common/include/`.
- Keep declarations, no implementation of non-inline functions.
- Use include guards and forward declarations where appropriate.

### Source organization
- Implementation in `common/src/`.
- One `.cpp` per logical component.
- Keep functions short and focused.

### Comments
- Document why a constant exists, not what it is.
- Use block comments for module purpose and inline comments sparingly.

### Namespaces
- Encapsulate shared code in `ebms::common`.
- Avoid `using namespace` in headers.

### Formatting
- Use consistent indentation (spaces or tabs per project rule, but Makefile recipes require tabs).
- Align similar declarations.
- Keep line length readable.

### Error handling
- Validate inputs in public interfaces.
- Throw exceptions for invalid operations in the common module.
- Do not swallow errors silently.

## Best Practices

### Enterprise practices
- Keep the common library stable and backwards compatible.
- Avoid business logic in the common module.
- Build the library as shared to support multiple consumers.
- Use versioning and consistent naming.

### Why
- Large enterprise systems grow quickly; a well-defined common base avoids duplicated work.
- Shared libraries reduce binary sizes and simplify patching.
- Clear boundaries prevent module coupling from turning into monolithic spaghetti.

## Exercises

### Easy
- Add a new `enum class ErrorCode` in `Common.h`.
- Add a helper `std::string toString(ErrorCode)`.

### Medium
- Add a `struct AppConfig` with compile-time constants and a runtime validation helper.
- Add a template function `safeDivide<T>(T a, T b)` that throws on division by zero.

### Hard
- Create a `Result<T>` type in the common module to represent success/failure without exceptions.
- Use it to wrap a simple utility function.

### Challenge
- Design a `Version` struct in the common module with major/minor/patch semantics and a `toString()` helper.
- Add compile-time version constants and a runtime comparison operator.

## Interview Questions

### Beginner
- What is the difference between `#include "..."` and `#include <...>`?
- Why do we use header guards?
- What is an `enum class`?

### Intermediate
- Why is `constexpr` preferred over `#define` for constants?
- What is the purpose of `-fPIC` when building a shared library?
- How does separating header and source files help large projects?

### Senior
- When would you use a shared library instead of a static library?
- How do namespaces help reduce coupling in enterprise software?
- What are the risks of exposing too much in a common module?

### Architect
- How should a versioned shared library be designed to avoid breaking existing consumers?
- What is the role of a platform or common services team in a large enterprise architecture?
- How do you ensure the `common` module remains stable while the product evolves?

## Sprint Checklist
- [ ] Create `common/include/` directory
- [ ] Create `common/src/` directory
- [ ] Implement `common/include/Common.h`
- [ ] Implement `common/src/Common.cpp`
- [ ] Add `apps/BankingApp/main.cpp`
- [ ] Update top-level `Makefile` for shared library build
- [ ] Build `lib/libCommon.so`
- [ ] Build `bin/BankingSystem`
- [ ] Run the demo app successfully
- [ ] Verify no compiler or linker warnings

## Preparation For Next Sprint
- This sprint establishes the shared common library and build mechanics.
- The next sprint can safely add domain models such as accounts and customers because they will reuse these shared constants and enums.
- The next sprint depends on this sprint for stable module boundaries, consistent naming, and build integration.
