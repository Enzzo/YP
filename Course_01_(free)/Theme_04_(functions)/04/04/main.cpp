#include <iostream>

int Factorial(const int);

int main() {
	int x;
	std::cin >> x;
	std::cout << Factorial(x);

	return 0;
}

int Factorial(const int n) {
	int r = 1;
	if (n > 1) {
		for (size_t i = 1; i <= n; ++i) {
			r *= i;
		}
	}
	return r;
}