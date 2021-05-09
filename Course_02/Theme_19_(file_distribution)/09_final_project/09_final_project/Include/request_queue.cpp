#include "request_queue.h"

template <typename DocumentPredicate>
std::vector<Document>RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
    if (requests_.size() >= sec_in_day_) {
        requests_.pop_front();
    }

    //fill queue by all requests
    requests_.push_back({ search_server_.FindTopDocuments(raw_query, document_predicate) });
    return requests_.back().request;
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    return AddFindRequest(raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
        return document_status == status;
        });
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    return AddFindRequest(raw_query, DocumentStatus::ACTUAL);
}

int RequestQueue::GetNoResultRequests() const {
    int empty_result = 0;
    for (const QueryResult& r : requests_) {
        if (r.request.size() == 0) {
            empty_result++;
        }
    }
    return empty_result;
}