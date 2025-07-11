#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

// не мен€йте файлы xml.h и xml.cpp
#include "xml.h"

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

vector<Spending> LoadFromXml(istream& input) {
    std::vector<Spending> result;
    auto db = Load(input).GetRoot().Children();
    for (const auto& data : db) {
        int amount = data.AttributeValue<int>("amount");
        std::string category = data.AttributeValue<std::string>("category");
        result.push_back({ category, amount });
    }
    return result;
}

int main() {
    std::ifstream ifs("input.txt");
    const vector<Spending> spendings = LoadFromXml(ifs);
    cout << "Total "sv << CalculateTotalSpendings(spendings) << '\n';
    cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings) << '\n';
}