#include <iostream>
#include <string>
#include <vector>

bool IsPalindrome(const std::string&);
std::vector<std::string> PalindromeFilter(const std::vector<std::string>&, const int);
std::vector<std::string> SplitIntoWords(const std::string&);
void ShowVector(const std::vector<std::string>&);

int main() {

	std::string line = "weew bro code";
	int min = 4;

	//std::getline(std::cin, line);
	//std::cin >> min;
	std::vector<std::string> words = SplitIntoWords(line);
	ShowVector(PalindromeFilter(words, min));

	return 0;
}

bool IsPalindrome(const std::string& s) {

	for (size_t i = 0; i < s.size() / 2; ++i)
		if (s[i] != s[s.size() - i - 1]) 
			return false;

	return true;
}

std::vector<std::string> PalindromeFilter(const std::vector<std::string>& words, const int min_length) {
	std::vector<std::string> result;

	for (const std::string& s : words) {
		if (IsPalindrome(s) && s.size() >= min_length)
			result.push_back(s);		
	}

	return result;
}

std::vector<std::string> SplitIntoWords(const std::string& line) {
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

void ShowVector(const std::vector<std::string>& v) {
	for (const std::string& s : v)
		std::cout << s << std::endl;
}