#!/bin/bash

set -e

echo "=== Building project for Valgrind ==="
mkdir -p build_release
cd build_release
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

echo ""
echo "=== Running Valgrind Memory Check ==="

valgrind --tool=memcheck \
         --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --error-exitcode=1 \
         ./run_tests

echo ""
echo "Valgrind check completed successfully! No leaks found."