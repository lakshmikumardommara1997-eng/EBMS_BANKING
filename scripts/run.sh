#!/bin/bash

# Get project root directory
PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"

# Add project's lib directory
export LD_LIBRARY_PATH="$PROJECT_ROOT/lib:$LD_LIBRARY_PATH"

echo "========================================"
echo " Enterprise Banking Management System"
echo "========================================"
echo "Project : $PROJECT_ROOT"
echo "Library : $PROJECT_ROOT/lib"
echo

# Run the application
"$PROJECT_ROOT/bin/BankingSystem"
