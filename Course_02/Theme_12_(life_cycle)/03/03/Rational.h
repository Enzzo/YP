#pragma once
#include <numeric>

class Rational{
	int numerator_ = 0;
	int denominator_ = 1;

public:

	Rational() = default;

	Rational(int numerator):Rational(numerator, 1) {}
	
	Rational(int numerator, int denominator):numerator_(numerator), denominator_(denominator){
		Normalize();
	}

	int Numerator()const {
		return numerator_;
	}

	int Denominator()const {
		return denominator_;
	}

private:

	void Normalize() {
		if (denominator_ < 0) {
			numerator_ *= (-1);
			denominator_ *= (-1);
		}

		const int divizor = std::gcd(numerator_, denominator_);

		numerator_ /= divizor;
		denominator_ /= divizor;		
	}
};

