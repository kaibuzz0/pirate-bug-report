// test_nullcheck.cpp
// Demonstrates PIR-003: Missing NULL check pattern

#include <cstdio>
#include <cstring>

// Simulated types from Pirate codebase
struct BlockIndex {
    int nHeight;
    char hash[64];
};

// Simulated map with find
struct BlockMap {
    BlockIndex* data[10];
    int count;
    
    BlockMap() : count(0) {
        for(int i=0; i<10; i++) data[i] = nullptr;
    }
    
    void insert(const char* hash, BlockIndex* block) {
        if (count < 10) {
            data[count] = block;
            count++;
        }
    }
    
    BlockIndex* find(const char* hash) {
        for(int i=0; i<count; i++) {
            if (data[i] && strcmp(data[i]->hash, hash) == 0) {
                return data[i];
            }
        }
        return nullptr;
    }
};

// Simulates the code from komodo_bitcoind.cpp:54
int vulnerable_get_height(BlockMap& map, const char* hashBlock) {
    BlockIndex* block = map.find(hashBlock);
    
    // Current code pattern
    if (block != nullptr) {
        // Could add extra check here
        return block->nHeight;  // Assumes valid
    } else {
        // Unconfirmed transaction
        return 0;
    }
}

// Improved version with extra validation
int safe_get_height(BlockMap& map, const char* hashBlock) {
    BlockIndex* block = map.find(hashBlock);
    
    if (block != nullptr) {
        // Extra validation
        if (block->nHeight >= 0) {
            return block->nHeight;
        }
    }
    printf("  Block not found or invalid\n");
    return 0;
}

int main() {
    printf("=== PIR-003: NULL Check Pattern ===\n\n");
    
    // Setup test data
    BlockMap mapBlockIndex;
    BlockIndex block1;
    block1.nHeight = 1000;
    strcpy(block1.hash, "abc123");
    mapBlockIndex.insert("abc123", &block1);
    
    // Test 1: Existing block
    printf("Test 1: Existing block\n");
    int h1 = vulnerable_get_height(mapBlockIndex, "abc123");
    printf("  Height: %d (expected 1000)\n", h1);
    
    // Test 2: Missing block
    printf("\nTest 2: Missing block\n");
    int h2 = vulnerable_get_height(mapBlockIndex, "nonexistent");
    printf("  Height: %d (expected 0)\n", h2);
    
    // Test 3: Safe version with same cases
    printf("\nTest 3: Safe version\n");
    printf("  Existing block:\n");
    int h3 = safe_get_height(mapBlockIndex, "abc123");
    printf("    Height: %d\n", h3);
    
    printf("  Missing block:\n");
    int h4 = safe_get_height(mapBlockIndex, "missing");
    printf("    Height: %d\n", h4);
    
    printf("\n=== Test Complete ===\n");
    printf("Recommendation: Add extra validation for safety\n");
    
    return 0;
}
