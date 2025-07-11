#include <iostream>
#include "search_server.h"

using namespace std;

void PrintDocument(const Document& document) {
    std::cout << "{ "
        << "document_id = " << document.id << ", "
        << "relevance = " << document.relevance << ", "
        << "rating = " << document.rating << " }" << std::endl;
}

int main() {
    setlocale(LC_ALL, "rus");
    SearchServer search_server("и в на"s);

    // Явно игнорируем результат метода AddDocument, чтобы избежать предупреждения
    // о неиспользуемом результате его вызова
    (void)search_server.AddDocument(1, "пушистый кот пушистый хвост", DocumentStatus::ACTUAL, { 7, 2, 7 });

    if (!search_server.AddDocument(1, "пушистый пёс и модный ошейник", DocumentStatus::ACTUAL, { 1, 2 })) {
        std::cout << "Документ не был добавлен, так как его id совпадает с уже имеющимся" << std::endl;
    }

    if (!search_server.AddDocument(-1, "пушистый пёс и модный ошейник", DocumentStatus::ACTUAL, { 1, 2 })) {
        std::cout << "Документ не был добавлен, так как его id отрицательный" << std::endl;
    }

    if (!search_server.AddDocument(3, "большой пёс скво\x12рец", DocumentStatus::ACTUAL, { 1, 3, 2 })) {
        std::cout << "Документ не был добавлен, так как содержит спецсимволы" << std::endl;
    }

    std::vector<Document> documents;
    if (search_server.FindTopDocuments("\x12шистый", documents)) {
        for (const Document& document : documents) {
            PrintDocument(document);
        }
    }
    else {
        std::cout << "Ошибка в поисковом запросе" << std::endl;
    }
}