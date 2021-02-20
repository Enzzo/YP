#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <execution>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

std::string ReadLine() {
	std::string s;
	std::getline(std::cin, s);
	return s;
}

int ReadLineWithNumber() {
	int result;
	std::cin >> result;
	ReadLine();
	return result;
}

struct Document {
	int id;
	int relevance;
};

class SearchServer {
public:
	void AddDocument(
		const int document_id,
		const std::string& document) {

		for (const std::string& word : SplitIntoWordsNoStop(document))
			word_to_documents_[word].insert(document_id);
	};
	void SetStopWords(const std::string& stop_words_joined) {
		for (const std::string& word : SplitIntoWords(stop_words_joined))
			stop_words_.insert(word);
	}
	std::vector<Document> FindTopDocuments(const std::string& query) const{
		auto matched_documents = FindAllDocuments(query);

		std::sort(std::execution::par, matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs) {return lhs.relevance > rhs.relevance; });
		if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
			matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
		}
		return matched_documents;
	}
private:
	std::map<std::string, std::set<int>> word_to_documents_;
	std::set<std::string> stop_words_;
	

	std::vector<std::string> SplitIntoWordsNoStop(const std::string& document) const {
		std::vector<std::string> words_;
		for (const std::string& word : SplitIntoWords(document)) {
			if (stop_words_.count(word) == 0) {
				words_.push_back(word);
			}
		}
		return words_;
	}

	std::vector<std::string> SplitIntoWords(const std::string& text) const{
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

	std::vector<Document>FindAllDocuments(const std::string& query) const {
		const std::vector<std::string> query_words = SplitIntoWordsNoStop(query);
		std::map<int, int> document_to_relevance;
		for (const std::string& word : query_words) {
			if (word_to_documents_.count(word) == 0) {
				continue;
			}
			for (const int document_id : word_to_documents_.at(word)) {
				++document_to_relevance[document_id];
			}
		}

		std::vector<Document> matched_documents;
		for (auto [document_id, relevance] : document_to_relevance) {
			matched_documents.push_back({ document_id, relevance });
		}
		return matched_documents;
	};
};

SearchServer CreateSearchServer() {
	SearchServer search_server;
	search_server.SetStopWords(ReadLine());

	const int document_count = ReadLineWithNumber();
	for (int document_id = 0; document_id < document_count; ++document_id) {
		search_server.AddDocument(document_id, ReadLine());
	}
	return search_server;
}

int main() {

	const SearchServer search_server = CreateSearchServer();	
	for (auto [document_id, relevance] : search_server.FindTopDocuments(ReadLine())) {
		std::cout << "{ document_id = " << document_id << ", relevance = " << relevance << " }" << std::endl;
	}
}