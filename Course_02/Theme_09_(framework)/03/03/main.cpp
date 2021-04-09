#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>

void Assert(bool expression, 
	const std::string& str,
	const std::string& file,
	const unsigned line,
	const std::string& function,
	const std::string& hint) {

	if (!expression) {
		std::cout << file << "(" << line << "): " << function <<": ASSERT("<< str <<") failed. ";
		if (!hint.empty()) {
			std::cout << "Hint: " << hint;
		}
		std::cout << std::endl;
		abort();
	}
}

#define ASSERT_HINT(expr, hint) Assert((expr), #expr, __FILE__, __LINE__, __FUNCTION__, hint)
#define ASSERT(expr) ASSERT_HINT(expr, "")

int main() {

	std::string hello = "hello";
	ASSERT(hello.empty());
	
	ASSERT_HINT(2 + 2 == 5, "This will fails"); 
	//assert.cpp(48): main: ASSERT(2 + 2 == 5) failed. Hint: This will fail 

	return 0;
}