#include <iostream>
#include <map>
#include <string>
#include <vector>

std::map<std::string, int> ComputeTermFreqs(const std::vector<std::string>& terms) {
	std::map<std::string, int> result;
	// реализуйте функцию
	for (const std::string word : terms)
		result[word]++;
	return result;
}

int main() {
	const std::vector<std::string> terms = { "first", "time", "first", "class" };
	for (const auto& [term, freq] : ComputeTermFreqs(terms)) {
		std::cout << term << " x " << freq << std::endl;
	}
	// вывод:
	// class x 1
	// first x 2
	// time x 1

	return 0;
}