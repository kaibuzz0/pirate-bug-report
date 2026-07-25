#!/bin/bash
# run_tests.sh
# Runs all security tests

echo "Pirate Chain Security Test Suite"
echo "================================="
echo ""

PASS=0
FAIL=0

cd tests

# Test 1: strcpy
echo "[1/3] Running test_strcpy..."
if ./test_strcpy > /dev/null 2>&1; then
    echo "  [+] PIR-001: strcpy test PASSED"
    PASS=$((PASS + 1))
else
    echo "  [-] PIR-001: strcpy test FAILED"
    FAIL=$((FAIL + 1))
fi

# Test 2: realloc
echo ""
echo "[2/3] Running test_realloc..."
if ./test_realloc > /dev/null 2>&1; then
    echo "  [+] PIR-002: realloc test PASSED"
    PASS=$((PASS + 1))
else
    echo "  [-] PIR-002: realloc test FAILED"
    FAIL=$((FAIL + 1))
fi

# Test 3: nullcheck
echo ""
echo "[3/3] Running test_nullcheck..."
if ./test_nullcheck > /dev/null 2>&1; then
    echo "  [+] PIR-003: nullcheck test PASSED"
    PASS=$((PASS + 1))
else
    echo "  [-] PIR-003: nullcheck test FAILED"
    FAIL=$((FAIL + 1))
fi

echo ""
echo "================================="
echo "Results: $PASS passed, $FAIL failed"
echo ""

if [ $FAIL -eq 0 ]; then
    echo "All tests completed successfully"
    exit 0
else
    echo "Some tests failed"
    exit 1
fi
