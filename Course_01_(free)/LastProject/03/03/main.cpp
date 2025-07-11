#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <execution>
#include <cmath>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

/// <summary>
/// ��������������� �������, ������� ������� � ����� ���������� ������ ���������� � ������ �� ���������
/// </summary>
/// <param name="ost"></param>
/// <param name="map"></param>
/// <returns></returns>
std::ostream& operator<<(std::ostream& ostream, const std::map<std::string, std::map<int, double>>& map) {
	std::ostream& ost = ostream;
	ost << "DEBUG:\n";
	for (const auto& [word, doc] : map) {
		ost << "[WORD]\n" << word;
		for (const auto& [d, tf] : doc) {
			ost << "\n" << d << "\t[TF] " << tf << std::endl;
		}
	}

	return ost;
}

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
	double relevance;
};

struct Query {
	std::set<std::string> p_words;
	std::set<std::string> m_words;
};

class SearchServer {
public:

	/// <summary>
	/// ��������� ������ word_to_documents �������, ������������ �� ��������� ������� ����������, � ������� ��� ����� ���������
	/// </summary>
	/// <param name="document_id"></param>
	/// <param name="document"></param>
	void AddDocument(
		const int document_id,
		const std::string& document) {
		document_count_++;
		std::vector<std::string> words = SplitIntoWordsNoStop(document);
		//�������� ��������� �������, �������� ����-����� � ������ �������� TF ������� ����� � ��������� ���������
		for (const std::string& word : words) {
			//word_to_documents_[word].insert(document_id);
		
			word_to_documents_freqs_[word][document_id] += 1.0 / static_cast<double>(words.size());
		}
	};

	/// <summary>
	/// ��������� ������ ����-����. ��� ����� ����� �������������� ��� ������ ������� � ����������
	/// </summary>
	/// <param name="stop_words_joined"></param>
	void SetStopWords(const std::string& stop_words_joined) {
		for (const std::string& word : SplitIntoWords(stop_words_joined))
			stop_words_.insert(word);
	}

	/// <summary>
	/// ��������� ��������� �� ������������� � ������� ���������� �� MAX_RESULT_DOCUMENT_COUNT
	/// </summary>
	/// <param name="query"></param>
	/// <returns></returns>
	std::vector<Document> FindTopDocuments(const std::string& query) const {
		auto matched_documents = FindAllDocuments(query);

		std::sort(std::execution::par, matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs) {return lhs.relevance > rhs.relevance; });
		if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
			matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
		}
		return matched_documents;
	}
private:
	/// <summary>
	/// ������ �������� ���������� � ������� �����
	/// ����������� ������ ���� �������, ������� �� �������� ����-�������
	/// �������� TF<double> ��� ������� ����� <string> � ��������� <int>
	/// </summary>
	std::map<std::string, std::map<int, double>> word_to_documents_freqs_;
	
	/// <summary>
	/// ��������� ����-����
	/// </summary>
	std::set<std::string> stop_words_;
	
	/// <summary>
	/// ����� ���������� ����������
	/// </summary>
	int document_count_ = 0;

	/// <summary>
	/// �������� ����-�����
	/// </summary>
	/// <param name="document"></param>
	/// <returns>���������� ������ ����� ��� ����-����</returns>
	std::vector<std::string> SplitIntoWordsNoStop(const std::string& document) const {
		std::vector<std::string> words_;
		for (const std::string& word : SplitIntoWords(document)) {
			if (!stop_words_.count(word)) {
				words_.push_back(word);
			}
		}
		return words_;
	}

	/// <summary>
	/// ������������� ������ ���� � ������ ����
	/// </summary>
	/// <param name="query"></param>
	/// <returns>������ ����</returns>
	std::vector<std::string> SplitIntoWords(const std::string& query) const {

		std::vector<std::string> words;
		std::string word;
		for (const char c : query) {
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
	/// <summary>
	/// ���� ���������� ������� �� ������������
	/// </summary>
	/// <param name="query - ������ ���������� �������"></param>
	/// <returns>���������, ���������� ����-����� � �����-�����</returns>
	const Query ParseQuery(const std::string& query) const {
		Query q;

		std::vector<std::string> line = SplitIntoWords(query);

		for (const std::string& s : line) {
			if (s[0] == '-') {
				q.m_words.insert(s.substr(1));
			}
			else
				q.p_words.insert(s);
		}

		return q;
	};

	/// <summary>
	/// 
	/// </summary>
	/// <param name="query"></param>
	/// <returns></returns>
	const std::vector<Document>FindAllDocuments(const std::string& query) const {

		Query query_ = ParseQuery(query);

		const std::set<std::string> plus_words  = query_.p_words;//SplitIntoWordsNoStop(query);
		const std::set<std::string> minus_words = query_.m_words;



		std::map<int, double> document_to_relevance;
		for (const std::string& word : plus_words) {
			if (word_to_documents_freqs_.count(word) == 0) {
				continue;
			}
			for (const auto& [document_id, tf] : word_to_documents_freqs_.at(word)) {
				document_to_relevance[document_id] += tf * std::log(document_count_ / static_cast<double>(word_to_documents_freqs_.at(word).size()));
			}
		}

		for (const std::string& word : minus_words) {
			if (word_to_documents_freqs_.count(word) == 0) {
				continue;
			}
			for (const std::pair<int, double>& document_id : word_to_documents_freqs_.at(word)) {
				document_to_relevance.erase(document_id.first);
			}
		}

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