#include <iostream>
#include <cstdlib>
#include <string>

void Assert(const bool value, const std::string& hint) {
	if (!value) {
		std::cout << "Assertion failed. ";
		if (!hint.empty()) {
			std::cout << "Hint: " << hint << std::endl;
		}
		abort();
	}
}

int main() {
	const std::string greeting = "hello";

	Assert(greeting.empty(), "Greeting must be non-empty");
	std::cout << "This line will not be printed" << std::endl;
	return 0;
}