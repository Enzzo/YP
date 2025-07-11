#include <iostream>
#include <string>
#include <set>

std::set<std::string> SplitIntoWords(const std::string&);
int CalculateSimilarity(const std::set<std::string>&, const std::set<std::string>&);

int main() {
	std::string query, description;

	std::getline(std::cin, query);
	std::getline(std::cin, description);

	std::cout << CalculateSimilarity(SplitIntoWords(query), SplitIntoWords(description));
	return 0;
}

std::set<std::string> SplitIntoWords(const std::string& line) {
	std::set<std::string> s;
	std::string word;

	for (size_t i = 0; i < line.size(); ++i) {
		if (isspace(line[i])) {
			s.insert(word);
			word = "";
		}
		else
			word += line[i];
	}
	s.insert(word);

	return s;
}

int CalculateSimilarity(const std::set<std::string>& first, const std::set<std::string>& second) {
	int result = 0;

	for (const std::string s : first) {
		if (second.count(s) > 0) result++;
	}

	return result;
}