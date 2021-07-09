#include "search_server.h"
#include "test_example_functions.h"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

int main() {
    TestSearchServer();
    SearchServer search_server("and with"sv);

    int id = 0;
    for (
        const string_view& text : {
            "funny pet and nasty rat"sv,
            "funny pet with curly hair"sv,
            "funny pet and not very nasty rat"sv,
            "pet with rat and rat and rat"sv,
            "nasty rat with curly hair"sv,
        }
        ) {
        search_server.AddDocument(++id, text, DocumentStatus::ACTUAL, { 1, 2 });
    }

    const string query = "curly and funny -not"s;

    {
        const auto [words, status] = search_server.MatchDocument(query, 1);
        cout << words.size() << " words for document 1"s << endl;
        // 1 words for document 1
    }

    {
        const auto [words, status] = search_server.MatchDocument(execution::seq, query, 2);
        cout << words.size() << " words for document 2"s << endl;
        // 2 words for document 2
    }

    {
        const auto [words, status] = search_server.MatchDocument(execution::par, query, 3);
        cout << words.size() << " words for document 3"s << endl;
        // 0 words for document 3
    }
    return 0;
}