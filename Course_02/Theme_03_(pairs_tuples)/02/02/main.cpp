#include <algorithm>
#include <iostream>
#include <vector>

enum class Status {
    ACTUAL,
    EXPIRED,
    DELETED
};

struct Document {
    int id;
    Status status;
    double relevance;
    int rating;
};

void SortDocuments(std::vector<Document>& matched_documents) {
    std::sort(matched_documents.begin(), matched_documents.end(),
        [](const Document& lhs, const Document& rhs) {
            return std::pair(lhs.rating, lhs.relevance)
                  > std::pair(rhs.rating, rhs.relevance);
        });
}

int main() {
    std::vector<Document> documents = {
        {100, Status::ACTUAL,  0.5, 4},
        {101, Status::EXPIRED, 0.5, 4},
        {102, Status::ACTUAL,  1.2, 4},
        {103, Status::DELETED, 1.2, 4},
        {104, Status::ACTUAL,  0.3, 5},
    };
    SortDocuments(documents);
    for (const Document& document : documents) {
        std::cout << document.id << ' '
            << static_cast<int>(document.status) << ' '
            << document.relevance << ' '
            << document.rating << std::endl;
    }
}