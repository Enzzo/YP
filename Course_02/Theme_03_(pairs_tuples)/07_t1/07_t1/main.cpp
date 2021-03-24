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
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
public:
    void SetStopWords(const std::string& text) {
        for (const std::string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const std::string& document, DocumentStatus status, const std::vector<int>& ratings) {
        const std::vector<std::string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();
        for (const std::string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id,
            DocumentData{
                ComputeAverageRating(ratings),
                status
            });
    };

    int GetDocumentCount()const {
        return documents_.size();
    }

    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentStatus status = DocumentStatus::ACTUAL) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, status);

        std::sort(matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) {
                
                return (abs(lhs.relevance - rhs.relevance) > 1e-6) ? lhs.relevance > rhs.relevance : lhs.rating > rhs.rating;
            });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

    //raw_query - слова запроса.
    //если слов в документе нет, то нужно вернуть пустой вектор слов
    //возвращает плюс-слова
    std::tuple<std::vector<std::string>, DocumentStatus> MatchDocument(const std::string& raw_query, const int id) {
        const Query query = ParseQuery(raw_query);
        const DocumentStatus ds = documents_.at(id).status;
        std::vector<std::string> s;

        //сначала ищем минус-слова. Если нашли, то вылетаем с пустым вектором
        for (const std::string& m_word : query.minus_words) {
            if (word_to_document_freqs_.count(m_word) > 0)
                if (word_to_document_freqs_.at(m_word).count(id) > 0)
                    return std::tuple(s, ds);
        }

        for (const std::string& p_word : query.plus_words) {
            if (word_to_document_freqs_.count(p_word) > 0 && std::count(std::execution::par, s.begin(), s.end(), p_word) == 0) {
                if (word_to_document_freqs_.at(p_word).count(id) > 0)
                    s.push_back(p_word);
            }
        }
        std::sort(std::execution::par, s.begin(), s.end());
        return std::tuple(s, ds);
    }

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    std::set<std::string> stop_words_;
    std::map<std::string, std::map<int, double>> word_to_document_freqs_;
    std::map<int, DocumentData> documents_;

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
        if (ratings.empty()) {
            return 0;
        }
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
        return log(documents_.size() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    std::vector<Document> FindAllDocuments(const Query& query, DocumentStatus status) const {
        std::map<int, double> document_to_relevance;
        for (const std::string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                if (documents_.at(document_id).status == status) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
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
                documents_.at(document_id).rating
                });
        }
        return matched_documents;
    }
};

void PrintDocument(const Document& document) {
    std::cout << "{ "
        << "document_id = "<< document.id << ", "
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

    for (const Document& document : search_server.FindTopDocuments("ухоженный кот")) {
        PrintDocument(document);
    }
}
/*
{ document_id = 2, relevance = 0.274653, rating = -1 }
{ document_id = 1, relevance = 0.101366, rating = 5 }
{ document_id = 0, relevance = 0.101366, rating = 2 }
*/