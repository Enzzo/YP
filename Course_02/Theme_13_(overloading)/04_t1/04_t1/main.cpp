#include "Rational.h"

#include <iostream>

int main() {

	Rational r1{ 2, 4 };
	Rational r2{ 3, 6 };
	r1 *= r2;
	std::cout << r1;
	return 0;
}