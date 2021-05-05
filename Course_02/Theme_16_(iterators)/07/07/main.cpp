#include <iostream>

int Fibonacci(const int);
bool IsPowOfTwo(const int);

int main() {
	int result = IsPowOfTwo(-4);
	std::cout << result << std::endl;
	return 0;
}

int Fibonacci(const int n) {
	if (0 <= n && n <= 1) {
		return n;
	}	
	return Fibonacci(n-1) + Fibonacci(n - 2);
}

bool IsPowOfTwo(const int p) {
	if (p == 1)return true;
	if (p % 2 != 0 || p == 0)return false;
	return IsPowOfTwo(p / 2);
}