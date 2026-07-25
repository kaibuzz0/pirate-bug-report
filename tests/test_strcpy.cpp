// test_strcpy.cpp
// Demonstrates PIR-001: strcpy buffer overflow

#include <cstring>
#include <cstdio>
#include <cstdlib>

// Simulates the vulnerable code pattern
void vulnerable_copy(char* dest, const char* src) {
    // This is the pattern from komodo_cJSON.c:98
    strcpy(dest, src);  // No bounds checking
}

// Fixed version
void safe_copy(char* dest, const char* src, size_t dest_size) {
    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0';
}

int main() {
    printf("=== PIR-001: strcpy Buffer Overflow Test ===\n\n");
    
    // Test 1: Normal copy (should work)
    char buffer[256];
    const char* normal_input = "Hello, World!";
    
    printf("Test 1: Normal input\n");
    vulnerable_copy(buffer, normal_input);
    printf("  Result: OK - copied '%s'\n", buffer);
    
    // Test 2: Exact size (boundary)
    char exact_buffer[11];
    const char* exact_input = "0123456789";  // 10 chars + null
    
    printf("\nTest 2: Exact boundary\n");
    vulnerable_copy(exact_buffer, exact_input);
    printf("  Result: OK - boundary case handled\n");
    
    // Test 3: Overflow case (this would crash with long strings)
    // Demonstrated conceptually - don't actually overflow in safe test
    printf("\nTest 3: Overflow demonstration\n");
    printf("  If src > dest buffer, strcpy overflows\n");
    printf("  This is the bug reported in PIR-001\n");
    
    printf("\n=== Test Complete ===\n");
    printf("Recommendation: Replace strcpy with strncpy\n");
    
    return 0;
}
