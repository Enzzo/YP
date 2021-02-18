#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

const size_t MAX_RESULT_DOCUMENT_COUNT = 5;

bool SortByRelevancePred(const std::pair<int, int>&, const std::pair<int, int>&);

std::string ReadLine(std::istream&);
int ReadLineWithNumber(std::istream&);
std::vector<std::string> SplitIntoWords(const std::string&);
std::set<std::string> ParseStopWords(const std::string&);
std::vector<std::string> SplitIntoWordsNoStop(const std::string&, const std::set<std::string>&);

void AddDocument(
	std::map<std::string, std::set<int>>&,
	const std::set<std::string>&,
	int document_id,
	const std::string&);

std::vector<std::pair<int, int>> FindAllDocuments(
	const std::map<std::string, std::set<int>>&,
	const std::set<std::string>&,
	const std::string&);

std::vector<std::pair<int, int>> FindTopDocuments(
	const std::vector<std::pair<int, int>>&
);

////////////////////////////////////////////////////////

int main() {

	
	//DEBUG
	//std::ifstream ifs("input.txt");
	//if (!ifs.is_open()) return -1;
	//
	std::istream &ist = std::cin;

	const std::string stop_words_joined = ReadLine(ist);
	const std::set<std::string> stop_words = ParseStopWords(stop_words_joined);
	
	std::map<std::string, std::set<int>> word_to_documents;
	const int document_count = ReadLineWithNumber(ist);

	for (int document_id = 0; document_id < document_count; ++document_id)
		AddDocument(word_to_documents, stop_words, document_id, ReadLine(ist));

	const std::string query = ReadLine(ist);
	for (const std::pair<int, int>& p : FindTopDocuments(FindAllDocuments(word_to_documents, stop_words, query)))
		std::cout << "{ document_id = " << p.first << ", relevance = " << p.second << " }" << std::endl;
	return 0;
}

////////////////////////////////////////////////////////

std::string ReadLine(std::istream& ist) {
	std::string s;
	getline(ist, s);
	return s;
}

int ReadLineWithNumber(std::istream& ist) {
	int result;
	ist >> result;
	ReadLine(ist);
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

std::vector<std::pair<int, int>> FindAllDocuments(
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

	std::sort(found_documents.begin(), found_documents.end(), SortByRelevancePred);
	std::reverse(found_documents.begin(), found_documents.end());

	return found_documents;
}

std::vector<std::pair<int, int>> FindTopDocuments(const std::vector<std::pair<int, int>>& list) {
	std::vector<std::pair<int, int>> top;
	
	for (int i = 0; i < MAX_RESULT_DOCUMENT_COUNT && i < list.size(); ++i)
		top.push_back(list[i]);
	return top;
}

bool SortByRelevancePred(const std::pair<int, int>& lhp, const std::pair<int, int>& rhp) {
	return lhp.second < rhp.second;
}