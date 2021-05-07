#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <optional>
#include <deque>

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
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}

template<typename It>
class IteratorRange {
    It _begin;
    It _end;

public:
    IteratorRange(const It begin, const It end) : _begin(begin), _end(end){};
    It begin() const { return _begin; }
    It end() const { return _end; }    
 };

template<typename It>
std::ostream& operator<<(std::ostream& os, const IteratorRange<It>& ir) {
    for (It it = ir.begin(); it != ir.end(); ++it) {
        os << *it;
    }
    return os;
}

template <typename It>
class Paginator {
    std::vector<IteratorRange<It>> _range;
    It _begin;
    It _end;

public:
    Paginator(const It begin, const It end, const size_t size):_begin(begin), _end(end){
        while (_begin != _end) {
            int distance = std::distance(_begin, _end);
            
            if (distance > static_cast<int>(size)) {
                distance = size;
            }
            _range.push_back({ _begin, _begin + distance});
            std::advance(_begin, distance);
        }
    };

    auto begin()const{
        return _range.begin();
    }
    auto end()const{
        return _range.end();
    }
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

struct Document {
    Document() = default;

    Document(int id, double relevance, int rating)
        :id(id)
        ,relevance(relevance)
        ,rating(rating) {
    }

    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

std::ostream& operator<<(std::ostream& os, const Document& d) {
    os << "{ document_id = " << d.id << ", relevance = " << d.relevance << ", rating = " << d.rating << " }";
    return os;
}

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
    
    std::map<std::string, std::map<int, double>> word_to_document_freqs_;
    
    std::map<int, DocumentData> documents_;
    
    std::vector<int> document_ids_;

public:
    // Defines an invalid document id
    // You can refer this constant as SearchServer::INVALID_DOCUMENT_ID
    inline static constexpr int INVALID_DOCUMENT_ID = -1;

    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words){
        CheckValidity(stop_words);
        stop_words_ = MakeUniqueNonEmptyStrings(stop_words);
    }

    explicit SearchServer(const std::string& stop_words_text)
        : SearchServer(SplitIntoWords(stop_words_text)) {}

    void AddDocument(int document_id, const std::string& document, DocumentStatus status,
        const std::vector<int>& ratings) {
        if ((document_id < 0)){
            throw std::invalid_argument("ID can't be less than zero");
        }
        if (documents_.count(document_id) > 0) {
            throw std::invalid_argument("ID already exists");
        }
        std::vector<std::string> words;
        if (!SplitIntoWordsNoStop(document, words)) {
            throw std::invalid_argument("invalid character(s) in word");
        }

        const double inv_word_count = 1.0 / words.size();
        for (const std::string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status });
        document_ids_.push_back(document_id);
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    int GetDocumentId(int index)const {

        if (GetDocumentCount() < index || index < 0) {
            throw std::out_of_range("non-existend ID");
        }
        return document_ids_.at(index);
    }

    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentPredicate document_predicate) const {
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

        // Exchange matched_documents and result instead of deep copying
        result.swap(matched_documents);
        return result;
    }

    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentStatus status) const {

        return FindTopDocuments(
            raw_query,
            [status](int document_id, DocumentStatus document_status, int rating) {
                return document_status == status;
            });
    }

    std::vector<Document> FindTopDocuments(const std::string& raw_query) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }    

    std::tuple<std::vector<std::string>, DocumentStatus> MatchDocument(const std::string& raw_query, int document_id) const {

        Query query;
        if (!ParseQuery(raw_query, query)) {
            throw std::invalid_argument("invalid request");
        }
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

        return { matched_words, documents_.at(document_id).status };
    }

private:

    static bool IsValidWord(const std::string& word) {
        // A valid word must not contain special characters
        return none_of(word.begin(), word.end(), [](char c) {
            return c >= '\0' && c < ' ';
            });
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

    bool IsStopWord(const std::string& word) const {
        return stop_words_.count(word) > 0;
    }

    [[nodiscard]] bool SplitIntoWordsNoStop(const std::string& text, std::vector<std::string>& result) const {
        result.clear();
        std::vector<std::string> words;
        for (const std::string& word : SplitIntoWords(text)) {
            if (!IsValidWord(word)) {
                return false;
            }
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        result.swap(words);
        return true;
    }

    [[nodiscard]] bool ParseQueryWord(std::string text, QueryWord& result) const {
        // Empty result by initializing it with default constructed QueryWord
        result = {};

        if (text.empty()) {
            return false;
        }
        bool is_minus = false;
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }

        if (text.empty() || text[0] == '-' || !IsValidWord(text)) {
            return false;
        }

        result = QueryWord{ text, is_minus, IsStopWord(text) };
        return true;
    }

    [[nodiscard]] bool ParseQuery(const std::string& text, Query& result) const {
        // Empty result by initializing it with default constructed Query
        result = {};
        for (const std::string& word : SplitIntoWords(text)) {
            QueryWord query_word;
            if (!ParseQueryWord(word, query_word)) {
                return false;
            }
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    result.minus_words.insert(query_word.data);
                }
                else {
                    result.plus_words.insert(query_word.data);
                }
            }
        }
        return true;
    }              

    // Existence required
    double ComputeWordInverseDocumentFreq(const std::string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    template <typename DocumentPredicate>
    std::vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const {
        std::map<int, double> document_to_relevance;
        for (const std::string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
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
            matched_documents.push_back({ document_id, relevance, documents_.at(document_id).rating });
        }
        return matched_documents;
    }

    template <typename StringContainer>
    void CheckValidity(const StringContainer& strings) {
        for (const std::string& str : strings) {
            if (!IsValidWord(str)) {
                throw std::invalid_argument("invalid stop-words in constructor");
            }
        }
    }

    template <typename StringContainer>
    std::set<std::string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
        std::set<std::string> non_empty_strings;
        for (const std::string& str : strings) {
            if (!str.empty()) {
                non_empty_strings.insert(str);
            }
        }
        return non_empty_strings;
    }
};

class RequestQueue {

    struct QueryResult {
        std::vector<Document> request;
    };

    const SearchServer& _search_server;

    std::deque<QueryResult> requests_;

    const static int sec_in_day_ = 1440;

public:
    explicit RequestQueue(const SearchServer& search_server):_search_server(search_server){}

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        if (requests_.size() >= sec_in_day_) {
            requests_.pop_front();
        }

        //fill queue by all requests
        requests_.push_back({ _search_server.FindTopDocuments(raw_query, document_predicate) });
        return requests_.back().request;
    }


    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status) {
        return AddFindRequest(raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
            return document_status == status;
            });
    }

    std::vector<Document> AddFindRequest(const std::string& raw_query) {
        return AddFindRequest(raw_query, DocumentStatus::ACTUAL);
    }

    int GetNoResultRequests() const {
        int empty_result = 0;
        for (const QueryResult& r : requests_) {
            if (r.request.size() == 0) {
                empty_result++;
            }
        }
        return empty_result;
    }
};