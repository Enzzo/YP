#include "parser.h"
#include "budget_manager.h"

#include <iostream>
#include <sstream>
#include <string_view>

using namespace std;

void ParseAndProcessQuery(BudgetManager& manager, string_view line) {
    // разработайте функцию чтения и обработки запроса
    Parser p(manager, line);
}

int ReadNumberOnLine(istream& input) {
    std::string line;
    std::getline(input, line);
    return std::stoi(line);
}

int main() {
    BudgetManager manager;

    const int query_count = ReadNumberOnLine(cin);

    for (int i = 0; i < query_count; ++i) {
        std::string line;
        std::getline(cin, line);
        ParseAndProcessQuery(manager, line);
    }
}