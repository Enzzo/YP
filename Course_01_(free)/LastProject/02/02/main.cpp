#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <execution>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

std::string ReadLine(istream& ist) {
	std::string s;
	std::getline(ist, s);
	return s;
}

int ReadLineWithNumber(istream& ist) {
	int result;
	ist >> result;
	ReadLine(ist);
	return result;
}

struct Document {
	int id;
	int relevance;
};

struct Query {
	std::vector<std::string> p_words;
	std::vector<std::string> m_words;
};

class SearchServer {
public:
	void AddDocument(
		const int document_id,
		const std::string& document) {

		for (const std::string& word : SplitIntoWordsNoStop(document)) {
			word_to_documents_[word].insert(document_id);
		}
	};
	void SetStopWords(const std::string& stop_words_joined) {
		for (const std::string& word : SplitIntoWords(stop_words_joined))
			stop_words_.insert(word);
	}
	std::vector<Document> FindTopDocuments(const std::string& query) const {
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
			if (!stop_words_.count(word)) {
				words_.push_back(word);
			}
		}
		return words_;
	}

	std::vector<std::string> SplitIntoWords(const std::string& text) const {

		std::vector<std::string> words;
		std::string word;
		for (const char c : text) {
			if (c == ' ') {
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

	const Query ParseQuery(const std::string& query) const {
		Query q;

		std::vector<std::string> line = SplitIntoWords(query);

		for (const std::string& s : line) {
			if (s[0] == '-') {
				q.m_words.push_back(s.substr(1));
			}
			else
				q.p_words.push_back(s);
		}

		return q;
	};

	const std::vector<Document>FindAllDocuments(const std::string& query) const {

		Query query_ = ParseQuery(query);

		const std::vector<std::string> plus_words = query_.p_words;//SplitIntoWordsNoStop(query);
		const std::vector<std::string> minus_words = query_.m_words;

		std::map<int, int> document_to_relevance;
		for (const std::string& word : plus_words) {
			if (word_to_documents_.count(word) == 0) {
				continue;
			}
			for (const int document_id : word_to_documents_.at(word)) {
					++document_to_relevance[document_id];
			}
		}

		for (const std::string& word : minus_words) {
			if (word_to_documents_.count(word) == 0) {
				continue;
			}
			for (const int document_id : word_to_documents_.at(word)) {
				document_to_relevance.erase(document_id);
			}
		}

		//Если document_to_relevance содержит стоп-слово, то тупо удаляем документ (идентификатор) по стоп-слову (без минуса) содержащемуся в word_to_documents_

		std::vector<Document> matched_documents;
		for (auto [document_id, relevance] : document_to_relevance) {
			matched_documents.push_back({ document_id, relevance });
		}
		return matched_documents;
	};
};

SearchServer CreateSearchServer(std::istream& ist) {
	SearchServer search_server;
	search_server.SetStopWords(ReadLine(ist));

	const int document_count = ReadLineWithNumber(ist);
	for (int document_id = 0; document_id < document_count; ++document_id) {
		search_server.AddDocument(document_id, ReadLine(ist));
	}
	return search_server;
}

int main() {

	//std::ifstream ifs("input.txt");
	
	//if (!ifs.is_open()) return -1;
	std::istream& ist = std::cin;

	const SearchServer search_server = CreateSearchServer(ist);
	for (auto [document_id, relevance] : search_server.FindTopDocuments(ReadLine(ist))) {
		std::cout << "{ document_id = " << document_id << ", relevance = " << relevance << " }" << std::endl;
	}
}