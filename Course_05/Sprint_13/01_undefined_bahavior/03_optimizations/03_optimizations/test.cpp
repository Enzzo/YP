#include "test.h"
#include "safe_add.h"
#include <cassert>

void TestAll() {
	TestLowerOverflow();
	TestUpperOverflow();
	TestNotOverflow();
};

void TestLowerOverflow() {
	// SIGNED
	// 1. l = min, r = min
	// 2. l = min, r = middle < 0
	// 3. l = middle < 0, r = min
	// 
	// UNSIGNED
	// 1. l = min, r = min
	// 2. l = min, r = middle < 0
	// 3. l = middle < 0, r = min
};

void TestUpperOverflow() {
	// SIGNED
	// 1. l = max, r = max
	// 2. l = max, r = middle > 0
	// 3. l = middle > 0 r = max
	// 
	// UNSIGNED
	// 1. l = max, r = max
	// 2. l = max, r = middle > 0
	// 3. l = middle > 0, r = max
};

void TestNotOverflow() {
	
	assert(SafeAdd(10, 10) == 20);
};