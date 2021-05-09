#pragma once

#include <deque>

#include "search_server.h"

class RequestQueue {

    struct QueryResult {
        std::vector<Document> request;
    };

    const SearchServer& search_server_;

    std::deque<QueryResult> requests_;

    const static int sec_in_day_ = 1440;

public:
    explicit RequestQueue(const SearchServer& search_server) :search_server_(search_server) {}
    
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);

    std::vector<Document> AddFindRequest(const std::string&, DocumentStatus);

    std::vector<Document> AddFindRequest(const std::string&);

    int GetNoResultRequests() const;
};