#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

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

/* Подставьте вашу реализацию класса SearchServer сюда */

class SearchServer {
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    std::set<std::string> stop_words_;
    std::map<std::string, std::map<int, double>> word_to_document_freqs_;
    std::map<int, DocumentData> documents_;

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
    }
    std::vector<Document> FindTopDocuments(const std::string& raw_query) const{
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }

    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentStatus status) const {
        return FindTopDocuments(raw_query, [status](const int document_id, const DocumentStatus ds, const int rating) { return status == ds; });
    }
    template<typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentPredicate document_predicate) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, document_predicate);

        std::sort(matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) {
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
        return matched_documents;
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    std::tuple<std::vector<std::string>, DocumentStatus> MatchDocument(const std::string& raw_query, int document_id) const {
        const Query query = ParseQuery(raw_query);
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

// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }

    // Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
    // возвращает пустой результат
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("in"s).empty());
    }
}

void AddingDocuments() {
    const int doc_id1 = 0;
    const int doc_id2 = 1;
    const int doc_id3 = 2;
    const int doc_id4 = 3;

    const std::string content1 = "1word1 1word2 1word3 1word4";
    const std::string content2 = "2word1 2word2 2word3 2word4";
    const std::string content3 = "3word1 3word2 3word3 3word4 3word3 3word4";
    const std::string content4 = "4word1 4word2 4word3 4word4";

    const std::vector<int> ratings = { 1, 2, 3 };
    //ПУСТЫЕ ДОКУМЕНТЫ
    {
        SearchServer server;
        const std::vector<Document>& found_documents = server.FindTopDocuments("word1");
        assert(found_documents.empty());
    }

    //ЗАДАНЫ СТОП-СЛОВА. ДОКУМЕНТЫ ПУСТЫЕ
    {
        SearchServer server;
        server.SetStopWords("stop1 stop2 stop3");
        const std::vector<Document>& fd = server.FindTopDocuments("word2");
        const std::vector<Document>& fds = server.FindTopDocuments("word2", DocumentStatus::BANNED);
        const std::vector<Document>& fdsp = server.FindTopDocuments("word2", [](const int id, const DocumentStatus ds, int rating) {return rating > 1; });
        assert(fd.empty());
        assert(fds.empty());
        assert(fdsp.empty());
        assert(server.GetDocumentCount() == 0);
    }

    //ЗАДАНЫ СТОП-СЛОВА. ДОКУМЕНТЫ ЗАПОЛНЕНЫ
    {
        SearchServer server;
        server.SetStopWords("1word1 2word2 2word3");
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, { 1, 2, 3 });
        server.AddDocument(doc_id2, content2, DocumentStatus::BANNED, { 4, 5, 6, 7, 8 });
        server.AddDocument(doc_id3, content3, DocumentStatus::IRRELEVANT, { 1, 3, 4, 5, 6, 7, 8 });
        server.AddDocument(doc_id4, content4, DocumentStatus::REMOVED, { 4, 5, 6, 7, 8, 20, 9 });
        
        //ПОИСК НЕ СТОП-СЛОВА:
        {
            const std::vector<Document>& fd = server.FindTopDocuments("1word2");
            assert(fd[0].id == doc_id1);
        }

        //ПОПИСК СТОП-СЛОВА:
        {
            const std::vector<Document>& fd = server.FindTopDocuments("2word2");
            assert(fd.empty());
        }
        
        //ПОИСК МИНУС-СЛОВА:
        {
            const std::vector<Document>& fd1 = server.FindTopDocuments("1word2 -1word3");
            assert(fd1.empty());
            const std::vector<Document>& fd2 = server.FindTopDocuments("2word1 -1word3", DocumentStatus::BANNED);
            assert(fd2[0].id == doc_id2);
        }

        //МАТЧИНГ ДОКУМЕНТОВ:
        {
            const auto& [w1, ds1] = server.MatchDocument("1word1 1word2 2word1 2word2", doc_id1);
            const auto& [w2, ds2] = server.MatchDocument("1word1 1word2 2word1 2word2", doc_id2);
            const auto& [w3, ds3] = server.MatchDocument("1word1 1word2 -2word1 2word2", doc_id1);
            const auto& [w4, ds4] = server.MatchDocument("-1word2 -2word1 2word2", doc_id1);
            assert(w1[0] == "1word2"); assert(ds1 == DocumentStatus::ACTUAL);
            assert(w2[0] == "2word1"); assert(ds2 == DocumentStatus::BANNED);
            assert(w3[0] == "1word2"); assert(ds3 == DocumentStatus::ACTUAL);
            assert(w4.empty());
        }      
        
    }

    //ПРОВЕРКА РЕЛЕВАНТНОСТИ (ОДИНАКОВЫЕ СТАТУСЫ):
    {
        SearchServer server;
        server.SetStopWords("1word1 2word2 2word3");
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, { 1, 2, 3 });
        server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, { 4, 5, 6, 7, 8 });
        server.AddDocument(doc_id3, content3, DocumentStatus::ACTUAL, { 1, 3, 4, 5, 6, 7, 8 });
        server.AddDocument(doc_id4, content4, DocumentStatus::ACTUAL, { 4, 5, 6, 7, 8, 20, 9 });

        const std::vector<Document>& fd = server.FindTopDocuments("1word2 -2word1 3word1 3word2 3word3 4word1");
        assert(fd[1].relevance < fd[0].relevance); assert(fd[0].rating < fd[2].rating);
        assert(fd[2].relevance < fd[1].relevance); assert(fd[1].rating < fd[0].rating);
        assert(fd[0].relevance == 0.92419624074659368); assert(fd[0].rating == 4);

    }
}

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    TestExcludeStopWordsFromAddedDocumentContent();
    AddingDocuments();

    //void SetStopWords(const std::string & text) 

    //void AddDocument(int document_id, const std::string & document, DocumentStatus status, const std::vector<int> & ratings)

    //std::vector<Document> FindTopDocuments(const std::string & raw_query)

    //std::vector<Document> FindTopDocuments(const std::string & raw_query, DocumentStatus status)

    //template<typename DocumentPredicate>
    //std::vector<Document> FindTopDocuments(const std::string & raw_query, DocumentPredicate document_predicate) 

    //int GetDocumentCount()

    //std::tuple<std::vector<std::string>, DocumentStatus> MatchDocument(const std::string & raw_query, int document_id)
    
    
    
    // Не забудьте вызывать остальные тесты здесь
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
    return 0;
}