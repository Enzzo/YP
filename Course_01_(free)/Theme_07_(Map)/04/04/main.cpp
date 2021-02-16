#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>

std::string ReadLine();
std::set<std::string> ParseStopWords(const std::string&);
std::vector<std::string> SplitIntoWords(const std::string&);
int ReadLineWithNumber();
std::vector<std::string> SplitIntoWordsNoStop(const std::string&, const std::set<std::string>&);
void AddDocument(std::map<std::string, std::set<int>>&, const std::set<std::string>&, int, const std::string&);
std::vector<int> FindDocuments(const std::map<std::string, std::set<int>>&, const std::set<std::string>&, const std::string&);

int main() {

	const std::string stop_words_joined = ReadLine();
	const std::set<std::string> stop_words = ParseStopWords(stop_words_joined);

	std::map<std::string, std::set<int>> word_to_documents;

	const int document_count = ReadLineWithNumber();
	for (int document_id = 0; document_id < document_count; ++document_id) {
		AddDocument(word_to_documents, stop_words, document_id, ReadLine());
	}

	const std::string query = ReadLine();
	for (const int document_id : FindDocuments(word_to_documents, stop_words, query)) {
		std::cout << document_id << std::endl;
	}
	return 0;
}

std::string ReadLine() {
	std::string s;
	std::getline(std::cin, s);
	return s;
}

std::set<std::string> ParseStopWords(const std::string& text) {
	std::set<std::string> stop_words;

	for (const std::string& word : SplitIntoWords(text)) 
		stop_words.insert(word);
	return stop_words;
}

int ReadLineWithNumber() {
	int result;
	std::cin >> result;
	ReadLine();
	return result;
}

std::vector<std::string> SplitIntoWords(const std::string& text) {
	std::vector<std::string> words;
	std::string word;
	for (const char c : text) {
		if (isspace(c)) {
			words.push_back(word);
			word = "";
		}
		else {
			word += c;
		}
	}
	words.push_back(word);
	return words;
}

std::vector<std::string>SplitIntoWordsNoStop(const std::string& text, const std::set<std::string>& stop_words) {
	std::vector<std::string> words;
	for (const std::string& word : SplitIntoWords(text)) {
		if (stop_words.count(word) == 0) {
			words.push_back(word);
		}
	}
	return words;
}

void AddDocument(std::map<std::string, std::set<int>>& word_to_documents,
	const std::set<std::string>& stop_words,
	int document_id,
	const std::string& document) {
	//TODO:
}

std::vector<int> FindDocuments(const std::map<std::string, std::set<int>>& word_to_documents,
	const std::set<std::string>& stop_words,
	const std::string& query) {
	
	//TODO:
	std::vector<int> v;
	return v;
}