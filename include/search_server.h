#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <optional>

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
            if (!word.empty()) {
                words.push_back(word);                
            }
            word = "";
        }
        else {
            word += c;
        }
    }
    if (!word.empty())
        words.push_back(word);
    return words;
}

struct Document {
    int id = 0;
    double relevance = .0;
    int rating = 0;

    Document() = default;
    Document(int _id, double _relevance, int _rating) : id(_id), relevance(_relevance), rating(_rating) {}
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    std::set<std::string> stop_words_;
    std::map<std::string, std::map<int, double>> word_to_document_freqs_;
    std::map<int, DocumentData> documents_;

public:

    inline static constexpr int INVALID_DOCUMENT_ID = -1;

    SearchServer() = default;

    explicit SearchServer(const std::string& inp_stop_words):SearchServer(SplitIntoWords(inp_stop_words)) {}

    template<typename StringCollection>
    explicit SearchServer(const StringCollection& collection) {
        for(const std::string& word : collection)
            if (stop_words_.count(word) == 0)
                stop_words_.insert(word);
    }

    void SetStopWords(const std::string& text) {
        for (const std::string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    [[nodiscard]] bool AddDocument(int document_id, const std::string& document, DocumentStatus status, const std::vector<int>& ratings) {
        
        if (documents_.count(document_id) > 0 || document_id < 0) return false;
        
        const std::vector<std::string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();
        
        for (const std::string& word : words) {
            if (!IsValidWord(word)) return false;
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id,
            DocumentData{
                ComputeAverageRating(ratings),
                status
            });
        return true;
    }

    std::optional<std::vector<Document>> FindTopDocuments(const std::string& raw_query, std::vector<Document>& result) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL, result);
    }

    [[nodiscard]] bool FindTopDocuments(const std::string& raw_query, DocumentStatus status, std::vector<Document>& result) const {
        return FindTopDocuments(raw_query, [status](const int document_id, const DocumentStatus ds, const int rating) { return status == ds; }, result);
    }

    //TODO +
    template<typename DocumentPredicate>
    [[nodiscard]] bool FindTopDocuments(const std::string& raw_query, DocumentPredicate document_predicate, std::vector<Document>& result) const {
        
        Query query;
        if (!ParseQuery(raw_query, query)) return false;

        result = FindAllDocuments(query, document_predicate);

        std::sort(result.begin(), result.end(),
            [](const Document& lhs, const Document& rhs) {
                if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                    return lhs.rating > rhs.rating;
                }
                else {
                    return lhs.relevance > rhs.relevance;
                }
            });

        if (result.size() > MAX_RESULT_DOCUMENT_COUNT) {
            result.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        //return matched_documents;
        return true;
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    //TODO
    int GetDocumentId(int index) const {
        if (documents_.count(index)) {
            return index;
        }
        return INVALID_DOCUMENT_ID;
    }

     [[nodiscard]] bool MatchDocument(const std::string& raw_query, int document_id, std::tuple<std::vector<std::string>, DocumentStatus>& result) const {
        Query query;
        if (!ParseQuery(raw_query, query))
             return false;
        std::vector<std::string> matched_words;
        for (const std::string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const std::string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        result = { matched_words, documents_.at(document_id).status };
        //return { matched_words, documents_.at(document_id).status };
        return true;
    }

private:

    static bool IsValidWord(const std::string& word) {
        //A valid word must not contain special characters
        return std::none_of(word.begin(), word.end(), [](const char c) {
            return c >= '\0' && c < ' ';
            });
    }

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

    [[nodiscard]]bool ParseQueryWord(std::string text, QueryWord& query_word) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            text = text.substr(1);
            is_minus = true;
        }
        if (text.empty()) return false;
        if (text[0] == '-' || !IsValidWord(text)) return false;
        query_word = {
            text,
            is_minus,
            IsStopWord(text)
        };
        return true;
    }

    struct Query {
        std::set<std::string> plus_words;
        std::set<std::string> minus_words;
    };

    [[nodiscard]] bool ParseQuery(const std::string& text, Query& query) const {

        //TODO: Первое условие: Слово не должно содержать двух минусов.
        //1 вариант: обрабатывать два минуса в разделителе слов
        //2 вариант: обрабатывать два минуса в парсере слов
        for (const std::string& word : SplitIntoWords(text)) {
            QueryWord query_word;
            if (!ParseQueryWord(word, query_word))
                return false;
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                }
                else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return true;
    }

    // Existence required
    double ComputeWordInverseDocumentFreq(const std::string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    template<typename Filter>
    std::vector<Document> FindAllDocuments(const Query& query, const Filter& filter) const {
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
        for (const auto& [document_id, relevance] : document_to_relevance) {
            if (filter(document_id, documents_.at(document_id).status, documents_.at(document_id).rating))
                matched_documents.push_back({
                    document_id,
                    relevance,
                    documents_.at(document_id).rating
                    });
        }
        return matched_documents;
    }
};