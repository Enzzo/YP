#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

std::string ReadLine();
int ReadLineWithNumber();
std::vector<std::string> SplitIntoWords(const std::string&);
std::set<std::string> ParseStopWords(const std::string&);
std::vector<std::string> SplitIntoWordsNoStop(const std::string&, const std::set<std::string>&);

void AddDocument(
	std::map<std::string, std::set<int>>&,
	const std::set<std::string>&,
	int document_id,
	const std::string&);

std::vector<std::pair<int, int>> FindDocuments(
	const std::map<std::string, std::set<int>>&,
	const std::set<std::string>&,
	const std::string&);

////////////////////////////////////////////////////////

int main() {
	const std::string stop_words_joined = ReadLine();
	const std::set<std::string> stop_words = ParseStopWords(stop_words_joined);

	std::map<std::string, std::set<int>> word_to_documents;
	const int document_count = ReadLineWithNumber();

	for (int document_id = 0; document_id < document_count; ++document_id)
		AddDocument(word_to_documents, stop_words, document_id, ReadLine());

	const std::string query = ReadLine();
	for (const std::pair<int, int>& p : FindDocuments(word_to_documents, stop_words, query))
		std::cout << "{ document_id = " << p.first << ", relevance = " << p.second << " }" << std::endl;
	return 0;
}

////////////////////////////////////////////////////////

std::string ReadLine() {
	std::string s;
	getline(std::cin, s);
	return s;
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
std::set<std::string> ParseStopWords(const std::string& text) {
	std::set<std::string> stop_words;
	for (const std::string& word : SplitIntoWords(text))
		stop_words.insert(word);
	return stop_words;
}

std::vector<std::string> SplitIntoWordsNoStop(const std::string& text, const std::set<std::string>& stop_words) {
	std::vector<std::string> words;
	for (const std::string& word : SplitIntoWords(text))
		if (stop_words.count(word) == 0)
			words.push_back(word);
	return words;
}

void AddDocument(std::map<std::string, std::set<int>>& word_to_documents,
	const std::set<std::string>& stop_words,
	int document_id,
	const std::string& document) {
	for (const std::string& word : SplitIntoWordsNoStop(document, stop_words))
		word_to_documents[word].insert(document_id);
}

std::vector<std::pair<int, int>> FindDocuments(
	const std::map<std::string, std::set<int>>& word_to_documents,
	const std::set<std::string>& stop_words,
	const std::string& query) {

	const std::vector<std::string> query_words = SplitIntoWordsNoStop(query, stop_words);
	std::map<int, int> document_to_relevance;
	for (const std::string& word : query_words) {
		if (word_to_documents.count(word) == 0)continue;
		for (const int document_id : word_to_documents.at(word))
			++document_to_relevance[document_id];
	}
	std::vector<std::pair<int, int>> found_documents;
	for (const std::pair<int, int> p : document_to_relevance)
		found_documents.push_back(p);
	return found_documents;
}