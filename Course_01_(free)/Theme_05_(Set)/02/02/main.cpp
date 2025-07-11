#include <iostream>
#include <set>
#include <string>

int main() {

	int n;
	std::cin >> n;
	std::string word;
	std::set<std::string> dict;

	for (size_t i = 0; i < n; ++i) {
		std::cin >> word;
		dict.insert(word);
	}
	std::cout << dict.size();

	return 0;
}