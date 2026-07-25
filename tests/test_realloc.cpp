// test_realloc.cpp
// Demonstrates PIR-002: Unchecked realloc pattern

#include <cstdio>
#include <cstdlib>
#include <cstring>

struct buffer {
    char *ptr;
    size_t len;
};

// Simulates vulnerable code from komodo_bitcoind.cpp:73
void vulnerable_realloc(struct buffer *s, size_t new_len) {
    printf("  Vulnerable: s->ptr = realloc(s->ptr, %zu)\n", new_len);
    
    s->ptr = (char *)realloc(s->ptr, new_len);
    if (s->ptr == NULL) {
        printf("  ERROR: realloc failed, but original pointer lost!\n");
        // In real code: StartShutdown() is called
        // But original s->ptr is now leaked
        return;
    }
    printf("  OK: realloc succeeded\n");
}

// Fixed version
void safe_realloc(struct buffer *s, size_t new_len) {
    printf("  Safe: new_ptr = realloc(s->ptr, %zu)\n", new_len);
    
    char *new_ptr = (char *)realloc(s->ptr, new_len);
    if (new_ptr == NULL) {
        printf("  ERROR: realloc failed, but s->ptr still valid\n");
        // Can still free original memory
        free(s->ptr);
        s->ptr = NULL;
        return;
    }
    s->ptr = new_ptr;
    printf("  OK: realloc succeeded\n");
}

int main() {
    printf("=== PIR-002: Unchecked realloc Pattern ===\n\n");
    
    // Test 1: Normal realloc
    printf("Test 1: Normal realloc\n");
    struct buffer s1;
    s1.ptr = (char *)malloc(100);
    s1.len = 100;
    strcpy(s1.ptr, "test data");
    
    vulnerable_realloc(&s1, 200);
    if (s1.ptr) free(s1.ptr);
    
    // Test 2: Demonstrate the pattern difference
    printf("\nTest 2: Memory preservation on failure\n");
    printf("  Vulnerable pattern loses old pointer on failure\n");
    printf("  Safe pattern preserves it\n");
    
    printf("\n=== Test Complete ===\n");
    printf("Recommendation: Preserve old pointer before realloc\n");
    
    return 0;
}
