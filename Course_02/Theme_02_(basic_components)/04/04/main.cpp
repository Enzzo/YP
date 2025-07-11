#include <iostream>
#include <cstdint>
#include <limits>

int main() {

	const auto min = std::numeric_limits<int64_t>::min();
	const auto max = std::numeric_limits<uint64_t>::max();

	std::cout << min << std::endl <<
		max << std::endl <<
		min + max << std::endl <<
		min * 2 << std::endl <<
		max * 2 << std::endl;

	return 0;
}