#include <iostream>
#include <cassert>

class SomeClass {
	int* p_ = nullptr;

public:
	SomeClass(const int x) :p_(new int(x)) {};
	SomeClass(const SomeClass& other) {
		p_ = new int(other.GetValue());
	}

	int GetValue()const {
		return *p_;
	}
	int& GetRef()const {
		return *p_;
	}
	~SomeClass() {
		delete p_;
	}
};

int main() {
	SomeClass s1(1);
	assert(s1.GetValue() == 1);
	{		
		SomeClass s2 = s1;
	}
	assert(s1.GetValue() == 1);
	return 0;
}