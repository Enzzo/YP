#include <cassert>
#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <string>

class Synonyms {
	std::map<std::string, std::set<std::string>> synonyms_;

public:
	void Add(const std::string& first_word, const std::string& second_word) {
		synonyms_[first_word].insert(second_word);
		synonyms_[second_word].insert(first_word);
	}
	size_t GetSynonymCount(const std::string& word)const {
		if (synonyms_.count(word) != 0) {
			return synonyms_.at(word).size();
		}
		return 0;
	}
	bool AreSynonyms(const std::string& first_word, const std::string& second_word)const {
		if (synonyms_.count(first_word) > 0) {
			if (synonyms_.at(first_word).count(second_word) > 0) {
				return true;
			}
		}
		return false;
	}
};

void TestAddingSynonymsIncreasesTheirCount() {
	Synonyms synonyms;
	assert(synonyms.GetSynonymCount("music") == 0);
	assert(synonyms.GetSynonymCount("melody") == 0);

	synonyms.Add("music", "melody");

	assert(synonyms.GetSynonymCount("music") == 1);
	assert(synonyms.GetSynonymCount("melody") == 1);

	synonyms.Add("music", "tune");
	assert(synonyms.GetSynonymCount("music") == 2);
	assert(synonyms.GetSynonymCount("tune") == 1);
	assert(synonyms.GetSynonymCount("melody") == 1);
}

void TestAreSynonyms() {
	Synonyms synonyms;
	assert(synonyms.AreSynonyms("music", "melody") == false); //пустой класс
	
	synonyms.Add("music", "melody");

	assert(synonyms.AreSynonyms("one", "two") == false); //слова, не содержащиеся в классе

	assert(synonyms.AreSynonyms("music", "melody") == true); //синонимы
	assert(synonyms.AreSynonyms("melody", "music") == true); // -//-

	synonyms.Add("one", "two");
	assert(synonyms.AreSynonyms("music", "one") == false);
	assert(synonyms.AreSynonyms("two", "melody") == false);
	assert(synonyms.AreSynonyms("one", "two") == true);
	
}

void TestSynonyms() {
	TestAddingSynonymsIncreasesTheirCount();
	TestAreSynonyms();
}
int main() {
	TestSynonyms();
	Synonyms synonyms;

	std::string line;

	while (getline(std::cin, line)) {
		std::istringstream command(line);
		std::string action;
		command >> action;

		if (action == "ADD") {
			std::string first_word, second_word;
			command >> first_word >> second_word;

			synonyms.Add(first_word, second_word);
		}
		else if (action == "COUNT") {
			std::string word;
			command >> word;

			std::cout << synonyms.GetSynonymCount(word) << std::endl;
		}
		else if (action == "CHECK") {
			std::string first_word, second_word;
			command >> first_word >> second_word;

			std::cout << synonyms.AreSynonyms(first_word, second_word) ? "YES" : "NO";
			std::cout << std::endl;
		}
		else if (action == "EXIT") {
			break;
		}
	}

	return 0;
}