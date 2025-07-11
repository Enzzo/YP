#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

std::string ReadLine() {
    std::string s;
    std::getline(std::cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    std::cin >> result;
    ReadLine();
    return result;
}

int main() {
    int queryCount = ReadLineWithNumber();

    std::vector<std::string> queries(queryCount);
    for (std::string& query : queries) {
        query = ReadLine();
    }
    std::string buzzword = ReadLine();

    std::cout << count_if(queries.begin(), queries.end(), [buzzword](const std::string& query) {
        return query.find(buzzword) != std::string::npos;
        });
    std::cout << std::endl;
}