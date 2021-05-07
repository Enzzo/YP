#pragma once

#include <deque>

#include "search_server.h"

class RequestQueue {

    struct QueryResult {
        std::vector<Document> request;
    };

    const SearchServer& _search_server;

    std::deque<QueryResult> requests_;

    const static int sec_in_day_ = 1440;

public:
    explicit RequestQueue(const SearchServer& search_server);

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        if (requests_.size() >= sec_in_day_) {
            requests_.pop_front();
        }

        //fill queue by all requests
        requests_.push_back({ _search_server.FindTopDocuments(raw_query, document_predicate) });
        return requests_.back().request;
    }


    std::vector<Document> AddFindRequest(const std::string&, DocumentStatus);

    std::vector<Document> AddFindRequest(const std::string&);

    int GetNoResultRequests() const;
};