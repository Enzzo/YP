#include <iostream>
#include <string>
#include <vector>

std::vector<std::string>& SplitIntoWords(const std::string&);

int main() {

	std::string query;
	std::getline(std::cin, query);

	std::vector<std::string> &words = SplitIntoWords(query);

	for (std::string word : words) {
		std::cout << '[' << word << ']' << std::endl;
	}

	return 0;
}

std::vector<std::string>& SplitIntoWords(const std::string& line) {
	std::vector<std::string> split;
	std::string word;

	for (const char ch : line) {
		if (isspace(ch)) {
			split.push_back(word);
			word = "";
		}
		else
			word += ch;
	}
	split.push_back(word);

	return split;
}