#include "test.h"
#include "safe_add.h"
#include <cassert>

void TestAll() {
	TestLowerOverflow();
	TestUpperOverflow();
	TestNotOverflow();
	TestFromExample();
};

void TestLowerOverflow() {
	
	
	// SIGNED
	{
		constexpr int8_t MIN = std::numeric_limits<int8_t>::min();

		// 1. l = min, r = min
		assert(SafeAdd(MIN, MIN) == std::nullopt);

		// 2. l = min, r = middle < 0
		assert(SafeAdd(MIN, static_cast<int8_t>(-1)) == std::nullopt);

		// 3. l = middle < 0, r = min
		assert(SafeAdd(static_cast<int8_t>(-1), MIN) == std::nullopt);
	}

	// UNSIGNED
	{
		constexpr uint8_t UMIN = std::numeric_limits<uint8_t>::min();
		
		// 1. l = min, r = min
		assert(SafeAdd(UMIN, UMIN) == UMIN);

		// 2. l = min, r = middle == 0
		assert(SafeAdd(UMIN, static_cast<uint8_t>(0)) == UMIN);

		// 3. l = middle == 0, r = min
		assert(SafeAdd(static_cast<uint8_t>(0), UMIN) == UMIN);
	}
};

void TestUpperOverflow() {
	// SIGNED
	{
		constexpr int8_t MAX = std::numeric_limits<int8_t>::max();

		// 1. l = max, r = max
		assert(SafeAdd(MAX, MAX) == std::nullopt);

		// 2. l = max, r = middle > 0
		assert(SafeAdd(MAX, static_cast<int8_t>(1)) == std::nullopt);

		// 3. l = middle > 0 r = max
		assert(SafeAdd(static_cast<int8_t>(1), MAX) == std::nullopt);
	}

	// UNSIGNED
	{
		constexpr uint8_t UMAX = std::numeric_limits<uint8_t>::max();

		// 1. l = max, r = max
		assert(SafeAdd(UMAX, UMAX) == std::nullopt);

		// 2. l = max, r = middle > 0
		assert(SafeAdd(UMAX, static_cast<uint8_t>(1)) == std::nullopt);

		// 3. l = middle > 0, r = max
		assert(SafeAdd(static_cast<uint8_t>(1), UMAX) == std::nullopt);
	}
};

void TestNotOverflow() {
	assert(SafeAdd(10, 10) == 20);
	assert(SafeAdd(10, -10) == 0);
	assert(SafeAdd(-10, 10) == 0);
	assert(SafeAdd(-10, -10) == -20);
};

void TestFromExample() {
	assert(SafeAdd(4567890123456789012, 5678901234567890123) == std::nullopt);
}