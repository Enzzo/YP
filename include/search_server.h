#pragma once

#include <cmath>
#include <math.h>
#include <set>
#include <map>
#include <algorithm>
#include <execution>

#include "document.h"
#include "string_processing.h"
#include "concurrent_map.h"

using namespace std::literals;

const int MAX_RESULT_DOCUMENT_COUNT = 5;
const int THREAD_COUNT = 4;

class SearchServer {

    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    struct QueryWord {
        std::string data;
        bool is_minus;
        bool is_stop;
    };

    struct Query {
        std::set<std::string> plus_words;
        std::set<std::string> minus_words;
    };

    std::set<std::string> stop_words_;    
    std::map<int, std::map<std::string, double>> doc_to_word_freqs_;
    //ConcurrentMap<int, std::map<std::string, double>> doc_to_word_freqs_ = ConcurrentMap<int, std::map<std::string, double>>(THREAD_COUNT);
    std::map<int, DocumentData> documents_;
    //ConcurrentMap<int, DocumentData> documents_ = ConcurrentMap<int, DocumentData>(THREAD_COUNT);
    std::set<int> document_id_;


public:
    // Defines an invalid document id
    // You can refer this constant as SearchServer::INVALID_DOCUMENT_ID
    inline static constexpr int INVALID_DOCUMENT_ID = -1;

    explicit SearchServer() = default;

    explicit SearchServer(const std::string_view&);

    template <typename StringContainer>
    explicit SearchServer(const StringContainer&);

    explicit SearchServer(const std::string&);

    

    void AddDocument(int, const std::string_view&, DocumentStatus, const std::vector<int>&);

    inline int GetDocumentCount() const noexcept {
        return documents_.size();
    }

    //O(1)
    inline std::set<int>::const_iterator begin() const noexcept {
        return document_id_.begin();
    }

    //O(1)
    inline std::set<int>::const_iterator end() const noexcept {
        return document_id_.end();
    }

    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(const std::string_view& raw_query, DocumentPredicate document_predicate) const;
    std::vector<Document> FindTopDocuments(const std::string_view&, DocumentStatus) const;
    std::vector<Document> FindTopDocuments(const std::string_view&) const;

    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(std::execution::sequenced_policy, const std::string_view& raw_query, DocumentPredicate document_predicate) const;
    std::vector<Document> FindTopDocuments(std::execution::sequenced_policy, const std::string_view&, DocumentStatus) const;
    std::vector<Document> FindTopDocuments(std::execution::sequenced_policy, const std::string_view&) const;

    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(std::execution::parallel_policy, const std::string_view& raw_query, DocumentPredicate document_predicate) const;
    std::vector<Document> FindTopDocuments(std::execution::parallel_policy, const std::string_view&, DocumentStatus) const;
    std::vector<Document> FindTopDocuments(std::execution::parallel_policy, const std::string_view&) const;


    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(const std::string_view&, int) const;
    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(std::execution::sequenced_policy, const std::string_view&, int) const;
    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(std::execution::parallel_policy, const std::string_view&, int) const;


    const std::map<std::string_view, double>& GetWordFrequencies(const int) const noexcept;

    void RemoveDocument(const int document_id);
    void RemoveDocument(std::execution::sequenced_policy p, const int document_id);
    void RemoveDocument(std::execution::parallel_policy p, const int document_id);

private:

    template<typename str>
    static bool IsValidWord(const str&);

    static int ComputeAverageRating(const std::vector<int>&);

    inline bool IsStopWord(const std::string_view& word) const {
        return stop_words_.count(static_cast<std::string>(word)) > 0;
    }

    [[nodiscard]] bool SplitIntoWordsNoStop(const std::string_view&, std::vector<std::string>&) const;

    [[nodiscard]] bool ParseQueryWord(std::string, QueryWord&) const;

    [[nodiscard]] bool ParseQuery(const std::string_view&, Query&) const;

    // Existence required
    double ComputeWordInverseDocumentFreq(const std::string&) const;

    template <typename DocumentPredicate>
    std::vector<Document> FindAllDocuments(const Query&, DocumentPredicate) const;

    template <typename StringContainer>
    void CheckValidity(const StringContainer&);

    template <typename StringContainer>
    std::set<std::string> MakeUniqueNonEmptyStrings(const StringContainer&);
};

template <typename StringContainer>
SearchServer::SearchServer(const StringContainer& stop_words) {
    CheckValidity(stop_words);
    stop_words_ = MakeUniqueNonEmptyStrings(stop_words);
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindTopDocuments(const std::string_view& raw_query, DocumentPredicate document_predicate) const {
    
    std::vector<Document> result;
    Query query;
    if (!ParseQuery(raw_query, query)) {
        throw std::invalid_argument("invalid request");
    }
    auto matched_documents = FindAllDocuments(query, document_predicate);

    sort(matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs) {
        if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
            return lhs.rating > rhs.rating;
        }
        else {
            return lhs.relevance > rhs.relevance;
        }
        });
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }

    result.swap(matched_documents);
    return result;
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindTopDocuments(std::execution::sequenced_policy, const std::string_view& raw_query, DocumentPredicate document_predicate) const {

    std::vector<Document> result;
    Query query;
    if (!ParseQuery(raw_query, query)) {
        throw std::invalid_argument("invalid request");
    }
    auto matched_documents = FindAllDocuments(query, document_predicate);

    sort(matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs) {
        if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
            return lhs.rating > rhs.rating;
        }
        else {
            return lhs.relevance > rhs.relevance;
        }
        });
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }

    result.swap(matched_documents);
    return result;
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindTopDocuments(std::execution::parallel_policy, const std::string_view& raw_query, DocumentPredicate document_predicate) const {

    std::vector<Document> result;
    Query query;
    if (!ParseQuery(raw_query, query)) {
        throw std::invalid_argument("invalid request");
    }
    auto matched_documents = FindAllDocuments(query, document_predicate);

    sort(std::execution::par, matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs) {
        if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
            return lhs.rating > rhs.rating;
        }
        else {
            return lhs.relevance > rhs.relevance;
        }
        });
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }

    result.swap(matched_documents);
    return result;
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const {
    std::map<int, double> document_to_relevance;

    for (const std::string& word : query.plus_words) {
        const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);

        for (const auto [document_id, word_freq] : doc_to_word_freqs_) {

            const auto& document_data = documents_.at(document_id);

            for (const auto [w, fr] : word_freq) {
                if (w == word) {
                    if (document_predicate(document_id, document_data.status, document_data.rating)) {
                        document_to_relevance[document_id] += fr * inverse_document_freq;
                    }
                    continue;
                }
            }
        }
    }

    for (const std::string& word : query.minus_words) {
        for (const auto& [id, doc] : doc_to_word_freqs_) {
            if (document_to_relevance.count(id)) {
                for (const auto& [w, fr] : doc) {
                    if (w == word) {
                        document_to_relevance.erase(id);
                        continue;
                    }
                }
            }
        }
    }

    std::vector<Document> matched_documents;
    for (const auto [document_id, relevance] : document_to_relevance) {
        matched_documents.push_back({ document_id, relevance, documents_.at(document_id).rating });
    }
    return matched_documents;
}

template <typename StringContainer>
void SearchServer::CheckValidity(const StringContainer& strings) {
    for (const auto& str : strings) {
        if (!IsValidWord(str)) {
            throw std::invalid_argument("invalid stop-words in constructor"s);
        }
    }
}

template <typename StringContainer>
std::set<std::string> SearchServer::MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    std::set<std::string> non_empty_strings;
    for (const auto& str : strings) {
        non_empty_strings.emplace(str);
    }
    return non_empty_strings;
}

template<typename str>
bool SearchServer::IsValidWord(const str& word) {
    // A valid word must not contain special characters
    return std::none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
        });
}

void RemoveDuplicates(SearchServer&);
