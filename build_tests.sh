#!/bin/bash
# build_tests.sh
# Builds all test executables

set -e

echo "Building Pirate Chain Security Test Suite"
echo "=========================================="
echo ""

cd tests

echo "[*] Building test_strcpy..."
g++ -o test_strcpy test_strcpy.cpp
if [ $? -eq 0 ]; then
    echo "[+] test_strcpy built successfully"
else
    echo "[!] test_strcpy failed to build"
fi

echo ""
echo "[*] Building test_realloc..."
g++ -o test_realloc test_realloc.cpp
if [ $? -eq 0 ]; then
    echo "[+] test_realloc built successfully"
else
    echo "[!] test_realloc failed to build"
fi

echo ""
echo "[*] Building test_nullcheck..."
g++ -o test_nullcheck test_nullcheck.cpp
if [ $? -eq 0 ]; then
    echo "[+] test_nullcheck built successfully"
else
    echo "[!] test_nullcheck failed to build"
fi

echo ""
echo "=========================================="
echo "Build complete"
echo ""
echo "Run individual tests:"
echo "  ./tests/test_strcpy"
echo "  ./tests/test_realloc"
echo "  ./tests/test_nullcheck"
echo ""
echo "Or run all: ./run_tests.sh"
