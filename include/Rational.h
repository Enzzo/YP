#pragma once
#include <numeric>
#include <iostream>

class Rational{
	int numerator_ = 0;
	int denominator_ = 1;

public:

	Rational() = default;

	Rational(int numerator):Rational(numerator, 1) {}
	
	Rational(int numerator, int denominator){
		Update(numerator, denominator);
	}

	int Numerator()const {
		return numerator_;
	}

	int Denominator()const {
		return denominator_;
	}

	void Update(const int numerator, const int denominator){
		numerator_ = numerator;
		denominator_ = denominator;
		Normalize();
	}

	Rational operator-() {
		return { -numerator_, denominator_ };
	}

	Rational operator+() {
		return { numerator_, denominator_ };
	}

	Rational& operator +=(const Rational r){

		numerator_ *= r.Denominator();
		numerator_ += r.Numerator() * denominator_;
		denominator_ *= r.Denominator();

		Normalize();
		return *this;
	}
	
	Rational& operator -=(Rational r) {
		*this += (-r);
		return *this;
	}
	
	Rational& operator *=(const Rational r) {
		numerator_ *= r.Numerator();
		denominator_ *= r.Denominator();
		Normalize();
		return *this;
	}

	Rational& operator /=(const Rational r) {
		numerator_ *= r.Denominator();
		denominator_ *= r.Numerator();
		Normalize();
		return *this;
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

std::ostream& operator<<(std::ostream& os, const Rational r) {
	return os << r.Numerator() << "/" << r.Denominator();
}

std::istream& operator>>(std::istream& is, Rational& r) {
	int n = 0, d = 1;
	char op;
	if (is >> n) {
		if (is >> op) {
			is >> d;
		}
		r.Update(n, d);
	}
	return is;
}

Rational operator+(Rational l, Rational r) {
	return l+=r;
}

Rational operator-(Rational l, Rational r) {
	return l-=r;
}

Rational operator*(Rational l, Rational r) {
	return l *= r;
}

Rational operator/(Rational l, Rational r) {
	return l /= r;
}

bool operator==(const Rational l, const Rational r) {
	return (l.Numerator() == r.Numerator() && l.Denominator() == r.Denominator());
}

bool operator!=(const Rational l, const Rational r) {
	return !(l == r);
}

bool operator<(const Rational l, const Rational r) {
	int nl = l.Numerator() * r.Denominator();
	int nr = r.Numerator() * l.Denominator();
	return nl < nr;
}

bool operator>(const Rational l, const Rational r) {
	int nl = l.Numerator() * r.Denominator();
	int nr = r.Numerator() * l.Denominator();
	return nl > nr;
}

bool operator<=(const Rational l, const Rational r) {
	return (l < r || l == r);
}

bool operator>=(const Rational l, const Rational r) {
	return (l > r || l == r);
}