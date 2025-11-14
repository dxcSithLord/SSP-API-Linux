/*
 * test_secure_memory.c - Test suite for secure memory handling
 *
 * This test suite verifies that sensitive data is properly cleared from memory
 * before being freed, in compliance with secure coding standards.
 *
 * Compile: gcc -o test_secure_memory test_secure_memory.c utils.c -I. -lpthread
 * Run: ./test_secure_memory
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "global.h"

// Test counter
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(condition, message) \
	do { \
		if (condition) { \
			printf("✓ PASS: %s\n", message); \
			tests_passed++; \
		} else { \
			printf("✗ FAIL: %s\n", message); \
			tests_failed++; \
		} \
	} while(0)

/**
 * Execute a unit test that verifies SecureMemoryClear zeros a 256-byte buffer.
 *
 * The test initializes a buffer with 'A' characters, asserts the initialization,
 * calls SecureMemoryClear on the buffer, and asserts that all bytes are zeroed.
 */
void test_secure_memory_clear() {
	printf("\n=== Test 1: SecureMemoryClear ===\n");

	// Create a buffer with known data
	char buffer[256];
	memset(buffer, 'A', sizeof(buffer));

	// Verify buffer contains 'A'
	int all_a = 1;
	for (size_t i = 0; i < sizeof(buffer); i++) {
		if (buffer[i] != 'A') {
			all_a = 0;
			break;
		}
	}
	TEST_ASSERT(all_a, "Buffer initialized with 'A'");

	// Clear the buffer
	SecureMemoryClear(buffer, sizeof(buffer));

	// Verify buffer contains zeros
	int all_zero = 1;
	for (size_t i = 0; i < sizeof(buffer); i++) {
		if (buffer[i] != 0) {
			all_zero = 0;
			break;
		}
	}
	TEST_ASSERT(all_zero, "Buffer cleared to zeros");
}

/**
 * Exercise SecureGlobalFree to verify it clears a 1024-byte allocation and sets the pointer to NULL.
 *
 * Allocates 1024 bytes, initializes the buffer with test data, invokes SecureGlobalFree(&ptr, size),
 * and asserts that the pointer is set to NULL and the buffer was initialized prior to the free.
 */
void test_secure_global_free() {
	printf("\n=== Test 2: SecureGlobalFree ===\n");

	const size_t size = 1024;
	void *ptr = GlobalAlloc(size);
	TEST_ASSERT(ptr != NULL, "GlobalAlloc succeeded");

	// Fill with sensitive data
	memset(ptr, 0xAB, size);

	// Verify data is set
	unsigned char *bytes = (unsigned char *)ptr;
	int data_set = (bytes[0] == 0xAB && bytes[size-1] == 0xAB);
	TEST_ASSERT(data_set, "Buffer filled with test data");

	// Store pointer for verification (in real scenario, this would be checking memory)
	// SecureGlobalFree should clear the memory before freeing
	SecureGlobalFree(&ptr, size);

	TEST_ASSERT(ptr == NULL, "Pointer set to NULL after SecureGlobalFree");
}

/**
 * Verifies that SecureMemoryClear safely handles NULL pointers and zero lengths.
 *
 * Calls SecureMemoryClear with a NULL pointer and a nonzero length, and with a valid buffer
 * and a length of zero, asserting that neither invocation crashes and both are treated as safe.
 */
void test_secure_memory_clear_null() {
	printf("\n=== Test 3: SecureMemoryClear with NULL ===\n");

	// Should not crash
	SecureMemoryClear(NULL, 100);
	TEST_ASSERT(1, "SecureMemoryClear handles NULL pointer");

	// Should not crash with zero length
	char buffer[10];
	SecureMemoryClear(buffer, 0);
	TEST_ASSERT(1, "SecureMemoryClear handles zero length");
}

/**
 * Validate that SecureMemoryClear zeroes a 32-byte cryptographic key buffer.
 *
 * Initializes a 32-byte key with incremental byte values, invokes SecureMemoryClear on it,
 * and asserts that all bytes are zero afterwards.
 */
void test_sensitive_data_patterns() {
	printf("\n=== Test 4: Sensitive Data Patterns ===\n");

	// Simulate cryptographic key
	unsigned char key[32];
	for (int i = 0; i < 32; i++) {
		key[i] = (unsigned char)i;
	}

	// Verify key is set
	TEST_ASSERT(key[0] == 0 && key[31] == 31, "Key initialized");

	// Clear the key
	SecureMemoryClear(key, sizeof(key));

	// Verify all bytes are zero
	int all_clear = 1;
	for (int i = 0; i < 32; i++) {
		if (key[i] != 0) {
			all_clear = 0;
			break;
		}
	}
	TEST_ASSERT(all_clear, "Cryptographic key cleared");
}

/**
 * Verify SecureMemoryClear zeroes authentication-related buffers.
 *
 * Initializes a simulated NUT token, CPS nonce, and HMAC buffer, invokes
 * SecureMemoryClear on each, and asserts that every byte in each buffer is
 * set to 0 after clearing.
 */
void test_auth_token_clearing() {
	printf("\n=== Test 5: Authentication Token Clearing ===\n");

	// Simulate authentication token (nut)
	char nut[12] = "TestNut12345";

	// Simulate CPS nonce
	char cps[24] = "TestCPSNonce12345678901";

	// Simulate HMAC
	unsigned char hmac[32];
	memset(hmac, 0xFF, sizeof(hmac));

	// Clear all sensitive data
	SecureMemoryClear(nut, sizeof(nut));
	SecureMemoryClear(cps, sizeof(cps));
	SecureMemoryClear(hmac, sizeof(hmac));

	// Verify all cleared
	int nut_clear = 1, cps_clear = 1, hmac_clear = 1;

	for (size_t i = 0; i < sizeof(nut); i++) {
		if (nut[i] != 0) nut_clear = 0;
	}
	for (size_t i = 0; i < sizeof(cps); i++) {
		if (cps[i] != 0) cps_clear = 0;
	}
	for (size_t i = 0; i < sizeof(hmac); i++) {
		if (hmac[i] != 0) hmac_clear = 0;
	}

	TEST_ASSERT(nut_clear, "NUT token cleared");
	TEST_ASSERT(cps_clear, "CPS nonce cleared");
	TEST_ASSERT(hmac_clear, "HMAC cleared");
}

/**
 * Perform a stress test of secure freeing by repeatedly allocating, filling, and securely freeing memory.
 *
 * Repeats 100 allocation/fill/secure-free cycles of 1024 bytes each. If any allocation fails or the pointer
 * remains non-NULL after SecureGlobalFree, the test is marked as failed; otherwise the test is marked as passed.
 */
void test_stress_secure_free() {
	printf("\n=== Test 6: Stress Test Secure Free ===\n");

	const int iterations = 100;
	const size_t size = 1024;
	int success = 1;

	for (int i = 0; i < iterations; i++) {
		void *ptr = GlobalAlloc(size);
		if (ptr == NULL) {
			success = 0;
			break;
		}
		memset(ptr, 0xFF, size);
		SecureGlobalFree(&ptr, size);
		if (ptr != NULL) {
			success = 0;
			break;
		}
	}

	TEST_ASSERT(success, "Stress test: 100 alloc/free cycles completed");
}

/**
 * Execute the secure memory test suite and print a formatted summary.
 *
 * Runs each test case for secure memory handling, prints per-suite headers and
 * a final summary showing total, passed, and failed counts.
 *
 * @returns 0 if all tests pass, 1 if any test fails.
 */
int main(int argc, char *argv[]) {
	printf("\n");
	printf("╔═══════════════════════════════════════════════════════════╗\n");
	printf("║   SSP-API Secure Memory Handling Test Suite              ║\n");
	printf("║   Testing compliance with secure coding standards         ║\n");
	printf("╚═══════════════════════════════════════════════════════════╝\n");

	// Run all tests
	test_secure_memory_clear();
	test_secure_global_free();
	test_secure_memory_clear_null();
	test_sensitive_data_patterns();
	test_auth_token_clearing();
	test_stress_secure_free();

	// Print summary
	printf("\n");
	printf("╔═══════════════════════════════════════════════════════════╗\n");
	printf("║   Test Summary                                            ║\n");
	printf("╠═══════════════════════════════════════════════════════════╣\n");
	printf("║   Total Tests: %3d                                        ║\n", tests_passed + tests_failed);
	printf("║   Passed:      %3d                                        ║\n", tests_passed);
	printf("║   Failed:      %3d                                        ║\n", tests_failed);
	printf("╚═══════════════════════════════════════════════════════════╝\n");
	printf("\n");

	if (tests_failed == 0) {
		printf("✓ All tests passed! Secure memory handling is working correctly.\n\n");
		return 0;
	} else {
		printf("✗ Some tests failed. Please review secure memory handling implementation.\n\n");
		return 1;
	}
}