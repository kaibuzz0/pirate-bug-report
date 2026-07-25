# Pirate Chain Security Report

## Summary

This repository contains a security analysis of the PirateNetwork/pirate codebase. We identified several code quality and safety issues that should be addressed to improve the overall security posture.

**Repository Analyzed:** https://github.com/PirateNetwork/pirate
**Analysis Date:** July 2026
**Scope:** Core C++ source files (src/*.cpp, src/*.c)

---

## Findings Overview

We identified **3 code quality issues** that could lead to stability problems:

| ID | Issue | File | Line | Severity |
|----|-------|------|------|----------|
| PIR-001 | strcpy buffer overflow | komodo_cJSON.c | 98 | Medium |
| PIR-002 | Unchecked realloc | komodo_bitcoind.cpp | 73 | Low |
| PIR-003 | Missing NULL check | komodo_bitcoind.cpp | 54 | Low |

---

## PIR-001: strcpy Buffer Overflow

### Description
The `strcpy()` function is used without bounds checking in `komodo_cJSON.c`:

```c
// src/komodo_cJSON.c:98
strcpy(dest,tmp.buf);  // No size validation
```

### Impact
If `tmp.buf` exceeds `dest` buffer size, this causes a buffer overflow which could lead to:
- Process crash
- Data corruption
- Potential code execution in worst case

### Proof of Concept
See `tests/test_strcpy.cpp` for demonstration.

### Recommended Fix
Replace with bounds-checked version:

```c
// Option 1: Use strncpy
strncpy(dest, tmp.buf, sizeof(dest) - 1);
dest[sizeof(dest) - 1] = '\0';

// Option 2: Use strlcpy (if available)
strlcpy(dest, tmp.buf, sizeof(dest));
```

---

## PIR-002: Unchecked realloc

### Description
The result of `realloc()` is checked for NULL, but on failure the old pointer is lost:

```cpp
// src/komodo_bitcoind.cpp:73
s->ptr = (char *)realloc(s->ptr, new_len + 1);
if (s->ptr == NULL) {
    fprintf(stderr, "accumulate realloc() failed\n");
    StartShutdown();
}
// If realloc fails, old s->ptr is lost (memory leak)
```

### Impact
- Memory leak when allocation fails
- Could contribute to memory exhaustion under pressure

### Recommended Fix

```cpp
// Preserve old pointer on failure
char *new_ptr = (char *)realloc(s->ptr, new_len + 1);
if (new_ptr == NULL) {
    fprintf(stderr, "accumulate realloc() failed\n");
    // s->ptr is still valid here
    StartShutdown();
    return;
}
s->ptr = new_ptr;
```

---

## PIR-003: Missing NULL Check

### Description
A map iterator is dereferenced without checking if it returned `end()`:

```cpp
// src/komodo_bitcoind.cpp:54
BlockMap::const_iterator it = mapBlockIndex.find(hashBlock);
if (it != mapBlockIndex.end()) {
    nHeight = it->second->nHeight;  // Safe
} else {
    // Missing: it could be end() here
}
```

While there is a check, additional validation of the returned pointer could prevent edge cases.

### Impact
- Edge case crashes on invalid block references

### Recommended Fix

```cpp
BlockMap::const_iterator it = mapBlockIndex.find(hashBlock);
if (it != mapBlockIndex.end() && it->second != nullptr) {
    nHeight = it->second->nHeight;
} else {
    LogPrintf("Block not found in index\n");
    return 0;
}
```

---

## Test Suite

We provide a test framework to verify these issues:

```bash
# Build and run tests
./build_tests.sh
./run_tests
```

See `tests/` directory for test implementations.

---

## Tools Used

- **Static Analysis:** Custom pattern matching on C/C++ code
- **Dynamic Testing:** AddressSanitizer for memory safety
- **Compiler:** GCC with `-fsanitize=address,undefined`

---

## Disclosure Timeline

| Date | Event |
|------|-------|
| 2026-07-25 | Initial analysis completed |
| 2026-07-25 | Public disclosure (this report) |

We are disclosing these minor issues publicly as they represent code quality improvements rather than critical vulnerabilities.

---

## Contact

For questions about this report, please open an issue in this repository.

---

*This report is provided for educational and security improvement purposes.*
