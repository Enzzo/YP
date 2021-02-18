#include <iostream>
#include <numeric>
#include <string>
#include <set>

std::string AddStopWords(const std::string&, const std::set<std::string>&);

std::set<std::string> SplitIntoWords(const std::string&);

std::string MarkStops(const std::string& lhs, const std::string& rhs) {
	return lhs + " -" + rhs;
};

int main() {
	std::string query;
	std::getline(std::cin, query);
	std::string stop_words;
	std::getline(std::cin, stop_words);

	std::cout << AddStopWords(query, SplitIntoWords(stop_words));

	return 0;
}

std::string AddStopWords(const std::string& query, const std::set<std::string>& stop_words) {

	std::string s = std::accumulate(stop_words.begin(), stop_words.end(), static_cast<std::string>(""), MarkStops);
	return query + s;
}

std::set<std::string> SplitIntoWords(const std::string& line) {
	std::set<std::string> result;
	std::string word = "";

	for(const char ch : line)
		if (isspace(ch)) {
			result.insert(word);
			word = "";
		}
		else {
			word += ch;
		}

	if(!word.empty())
		result.insert(word);

	return result;
}