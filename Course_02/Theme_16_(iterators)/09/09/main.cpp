#include "search_server.h"

using namespace std;

int main() {
    SearchServer search_server("and with"s);
    
    search_server.AddDocument(1, "funny pet and nasty rat", DocumentStatus::ACTUAL, { 7, 2, 7 });
    search_server.AddDocument(2, "funny pet with curly hair", DocumentStatus::ACTUAL, { 1, 2, 3 });
    search_server.AddDocument(3, "big cat nasty hair", DocumentStatus::ACTUAL, { 1, 2, 8 });
    search_server.AddDocument(4, "big dog cat Vladislav", DocumentStatus::ACTUAL, { 1, 3, 2 });
    search_server.AddDocument(5, "big dog hamster Borya", DocumentStatus::ACTUAL, { 1, 1, 1 });

    //auto = std::vector<Document>
    const auto search_results = search_server.FindTopDocuments("curly dog");
    int page_size = 2;

    //auto = Paginator
    const auto pages = Paginate(search_results, page_size);

    //TODO:
    // ¬ыводим найденные документы по страницам
    for (auto page = pages.begin(); page != pages.end(); ++page) {
        std::cout << *page << std::endl;
        std::cout << "Page break" << std::endl;
        int x = 2;
    }
    return 0;
}