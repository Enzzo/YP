// не мен€йте файлы json.h и json.cpp
#include "json.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

using namespace std;

struct Spending {
    string category;
    int amount;
};

int CalculateTotalSpendings(const vector<Spending>& spendings) {
    int result = 0;
    for (const Spending& s : spendings) {
        result += s.amount;
    }
    return result;
}

string FindMostExpensiveCategory(const vector<Spending>& spendings) {
    auto compare_by_amount = [](const Spending& lhs, const Spending& rhs) {
        return lhs.amount < rhs.amount;
    };
    return max_element(begin(spendings), end(spendings), compare_by_amount)->category;
}

vector<Spending> LoadFromJson(istream& input) {
    std::vector<Spending> result;
    auto db = Load(input).GetRoot().AsArray();
    for (const auto& data : db) {
        int amount = data.AsMap().at("amount").AsInt();
        std::string category = data.AsMap().at("category").AsString();

        result.push_back({ category, amount });
    }
    return result;
}

int main() {
    // не мен€йте main
    const vector<Spending> spendings = LoadFromJson(cin);
    cout << "Total "sv << CalculateTotalSpendings(spendings) << '\n';
    cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings) << '\n';
}