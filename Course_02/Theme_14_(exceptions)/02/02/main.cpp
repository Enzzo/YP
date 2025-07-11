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
    SearchServer search_server("� � ��"s);

    // ���� ���������� ��������� ������ AddDocument, ����� �������� ��������������
    // � �������������� ���������� ��� ������
    (void)search_server.AddDocument(1, "�������� ��� �������� �����", DocumentStatus::ACTUAL, { 7, 2, 7 });

    if (!search_server.AddDocument(1, "�������� �� � ������ �������", DocumentStatus::ACTUAL, { 1, 2 })) {
        std::cout << "�������� �� ��� ��������, ��� ��� ��� id ��������� � ��� ���������" << std::endl;
    }

    if (!search_server.AddDocument(-1, "�������� �� � ������ �������", DocumentStatus::ACTUAL, { 1, 2 })) {
        std::cout << "�������� �� ��� ��������, ��� ��� ��� id �������������" << std::endl;
    }

    if (!search_server.AddDocument(3, "������� �� ����\x12���", DocumentStatus::ACTUAL, { 1, 3, 2 })) {
        std::cout << "�������� �� ��� ��������, ��� ��� �������� �����������" << std::endl;
    }

    std::vector<Document> documents;
    if (search_server.FindTopDocuments("\x12������", documents)) {
        for (const Document& document : documents) {
            PrintDocument(document);
        }
    }
    else {
        std::cout << "������ � ��������� �������" << std::endl;
    }
}