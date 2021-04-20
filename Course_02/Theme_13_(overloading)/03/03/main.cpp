#include "Rational.h"

#include <iostream>

int main() {

	Rational r1{ 2, 4 };
	Rational r2{ 3, 4 };
	std::cout << r1 - r2;

	return 0;
}