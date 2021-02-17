#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>

std::string ReadLine(std::istream&);
std::set<std::string> ParseStopWords(const std::string&);
std::vector<std::string> SplitIntoWords(const std::string&);
int ReadLineWithNumber(std::istream&);
std::vector<std::string> SplitIntoWordsNoStop(const std::string&, const std::set<std::string>&);
void AddDocument(std::map<std::string, std::set<int>>&, const std::set<std::string>&, int, const std::string&);
std::vector<std::pair<int, int>> FindDocuments(const std::map<std::string, std::set<int>>&, const std::set<std::string>&, const std::string&);

int main() {

	//DEBUG:
	//std::ifstream ifst("input.txt");
	//if (!ifst.is_open()) return -1;

	std::istream& ist = std::cin;

	//Считываем стоп-слова
	const std::set<std::string> stop_words = ParseStopWords(ReadLine(ist));

	//Read documents
	std::map<std::string, std::set<int>> word_to_documents;
	const int document_count = ReadLineWithNumber(ist);
	for (int document_id = 0; document_id < document_count; ++document_id) {
		AddDocument(word_to_documents, stop_words, document_id, ReadLine(ist));
	}

	const std::string query = ReadLine(ist);
	for (const auto& document_id : FindDocuments(word_to_documents, stop_words, query)) {
		std::cout << document_id.first<<" "<<document_id.second << std::endl;
	}
	return 0;
}

std::string ReadLine(std::istream& ist) {
	std::string s;
	//std::getline(std::cin, s);
	std::getline(ist, s);
	return s;
}

std::set<std::string> ParseStopWords(const std::string& text) {
	std::set<std::string> stop_words;

	for (const std::string& word : SplitIntoWords(text))
		stop_words.insert(word);
	return stop_words;
}

int ReadLineWithNumber(std::istream& ist) {
	int result;
	//std::cin >> result;
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

std::vector<std::string>SplitIntoWordsNoStop(const std::string& text, const std::set<std::string>& stop_words) {
	std::vector<std::string> words;
	for (const std::string& word : SplitIntoWords(text)) {
		if (stop_words.count(word) == 0) {
			words.push_back(word);
		}
	}
	return words;
}

//СОЗДАТЬ ДОКУМЕНТ ПО ИДЕНТИФИКАТОРУ, ИСКЛЮЧАЯ СТОП-СЛОВА
//word_to_documents - список, который наполняется словами без стоп-слов
//stop_words		- стоп-слова
//document_id		- идентификатор документа
//document			- список слов в одном из документов
void AddDocument(std::map<std::string, std::set<int>>& word_to_documents,
	const std::set<std::string>& stop_words,
	int document_id,
	const std::string& document) {

	std::vector<std::string>words = SplitIntoWordsNoStop(document, stop_words);

	for (const std::string& s : words)
		word_to_documents[s].insert(document_id);
}


std::vector<std::pair<int, int>> FindDocuments(const std::map<std::string, std::set<int>>& word_to_documents,
	const std::set<std::string>& stop_words,
	const std::string& query) {

	const std::vector<std::string> query_words = SplitIntoWordsNoStop(query, stop_words);

	std::map<int, int> document_to_relevance;
	for (const std::string& word : query_words) {

		if (word_to_documents.count(word) == 0)
			continue;
		for (const int document_id : word_to_documents.at(word))
			document_to_relevance[document_id]++;
	}
	std::vector<std::pair<int, int>> found_documents;
	for (const std::pair<int, int> d : document_to_relevance) {
		found_documents.push_back({ d.first, d.second });
	}
	return found_documents;
}