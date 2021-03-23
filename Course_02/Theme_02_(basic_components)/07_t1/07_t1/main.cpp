#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <execution>

const int MAX_RESULT_DOCUMENT_COUNT = 5;

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

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED
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

struct Document {
    int id;
    double relevance;
    int rating;
    DocumentStatus status = DocumentStatus::ACTUAL;
};

class SearchServer {
    std::set<std::string> stop_words_;
    std::map<std::string, std::map<int, double>> word_to_document_freqs_;
    std::map<int, int> document_ratings_;
    std::map<int, DocumentStatus> document_status_;

public:
    void SetStopWords(const std::string& text) {
        for (const std::string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const std::string& document, const DocumentStatus& ds, const std::vector<int>& ratings) {
        const std::vector<std::string> words = SplitIntoWordsNoStop(document);
        
        const double inv_word_count = 1.0 / words.size();
        for (const std::string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        document_ratings_.emplace(document_id, ComputeAverageRating(ratings));
        document_status_.emplace(document_id, ds);
    }

    std::vector<Document> FindTopDocuments(const std::string& raw_query, const DocumentStatus& ds = DocumentStatus::ACTUAL) const {
        const Query query = ParseQuery(raw_query);
        
        std::vector<Document> all_documents = FindAllDocuments(query);

        std::vector<Document> matched_documents;
        

        for (const Document& d : all_documents) {
            if (document_status_.at(d.id) == ds)
                matched_documents.push_back(d);
        }
            
        std::sort(std::execution::par, matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) {
                return lhs.relevance > rhs.relevance;
            });

        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:    

    bool IsStopWord(const std::string& word) const {
        return stop_words_.count(word) > 0;
    }

    std::vector<std::string> SplitIntoWordsNoStop(const std::string& text) const {
        std::vector<std::string> words;
        for (const std::string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    static int ComputeAverageRating(const std::vector<int>& ratings) {
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }

    struct QueryWord {
        std::string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(std::string text) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {
            text,
            is_minus,
            IsStopWord(text)
        };
    }

    struct Query {
        std::set<std::string> plus_words;
        std::set<std::string> minus_words;
    };

    Query ParseQuery(const std::string& text) const {
        Query query;
        for (const std::string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                }
                else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }

    // Existence required
    double ComputeWordInverseDocumentFreq(const std::string& word) const {
        return log(document_ratings_.size() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    std::vector<Document> FindAllDocuments(const Query& query) const {
        std::map<int, double> document_to_relevance;
        for (const std::string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                document_to_relevance[document_id] += term_freq * inverse_document_freq;
            }
        }

        for (const std::string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        std::vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({
                document_id,
                relevance,
                document_ratings_.at(document_id),
                });
        }
        return matched_documents;
    }
    
};

/*
SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        const std::string document = ReadLine();
        int ratings_size;
        std::cin >> ratings_size;

        // создали вектор размера ratings_size из нулей
        std::vector<int> ratings(ratings_size, 0);

        // считали каждый элемент с помощью ссылки
        for (int& rating : ratings) {
            std::cin >> rating;
        }

        search_server.AddDocument(document_id, document, ratings);
        ReadLine();
    }

    return search_server;
}
*/
void PrintDocument(const Document& document) {
    std::cout << "{ "
        << "document_id = " << document.id << ", "
        << "relevance = " << document.relevance << ", "
        << "rating = " << document.rating
        << " }" << std::endl;
}

int main() {
    SearchServer search_server;
    search_server.SetStopWords("и в на");

    search_server.AddDocument(0, "белый кот и модный ошейник", DocumentStatus::ACTUAL, { 8, -3 });
    search_server.AddDocument(1, "пушистый кот пушистый хвост", DocumentStatus::ACTUAL, { 7, 2, 7 });
    search_server.AddDocument(2, "ухоженный пёс выразительные глаза", DocumentStatus::ACTUAL, { 5, -12, 2, 1 });
    search_server.AddDocument(3, "ухоженный скворец евгений", DocumentStatus::BANNED, { 9 });

    std::cout << "ACTUAL:" << std::endl;
    for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный кот", DocumentStatus::ACTUAL)) {
        PrintDocument(document);
    }

    std::cout << "BANNED:" << std::endl;
    for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный кот", DocumentStatus::BANNED)) {
        PrintDocument(document);
    }
}