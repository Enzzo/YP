#include <random>
#include <iostream>

int main() {

	std::mt19937 generator;

	std::uniform_int_distribution value{ 0, 99 };

	for (int i = 0; i < 10; ++i) {
		std::cout << value(generator) << std::endl;
	}

	return 0;
}