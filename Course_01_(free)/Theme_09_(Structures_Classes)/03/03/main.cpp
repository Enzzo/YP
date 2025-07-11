#include <map>
#include <set>
#include <string>
#include <vector>

struct Document {
	int id;
	int relevance;
};

std::vector<std::string> SplitIntoWords(const std::string& text) {
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

std::vector<std::string> SplitIntoWordsNoStop(const std::string& text, const std::set<std::string>& stop_words) {
	std::vector<std::string> words;
	for (const std::string& word : SplitIntoWords(text)) {
		if (stop_words.count(word) == 0) {
			words.push_back(word);
		}
	}
	return words;
}

// For each document returns its relevance and id
std::vector<Document> FindAllDocuments(
	const std::map<std::string, std::set<int>>& word_to_documents,
	const std::set<std::string>& stop_words,
	const std::string& query) {

	const std::vector<std::string> query_words = SplitIntoWordsNoStop(query, stop_words);
	std::map<int, int> document_to_relevance;
	for (const std::string& word : query_words) {
		if (word_to_documents.count(word) == 0) {
			continue;
		}
		for (const int document_id : word_to_documents.at(word)) {
			++document_to_relevance[document_id];
		}
	}

	std::vector<Document> matched_documents;
	for (auto [document_id, relevance] : document_to_relevance) {
		matched_documents.push_back({ relevance, document_id });
	}

	return matched_documents;
}

int main() {
	return 0;
}