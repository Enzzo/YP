#include "budget_manager.h"

#include <iostream>
#include <sstream>
#include <string_view>

using namespace std;

void ParseAndProcessQuery(BudgetManager& manager, string_view line) {
    // разработайте функцию чтения и обработки запроса
    Parser parser(line);

    std::istringstream istr(std::string(line.begin(), line.end()));
    std::string mode;
    std::string date_from;
    std::string date_to;

    std::getline(istr, mode, ' ');
    std::getline(istr, date_from, ' ');
    std::getline(istr, date_to, ' ');

    if (mode == "ComputeIncome") {
        // readable
        // ComputeIncome <дата начала> <дата конца> — вычислить чистую прибыль за данный диапазон дат, и вывести результат в cout.
        std::cout << manager.ComputeIncome(Date::FromString(date_from), Date::FromString(date_to)) << std::endl;
    }
    else if(mode == "Earn") {

        double income;
        istr >> income;
        // writeable
        // Earn <дата начала> <дата конца> <прибыль> — учесть, что в этот период равномерно по дням была заработана указанная сумма.
        // Прибыль — произвольное положительное число double.
        manager.Earn(Date::FromString(date_from), Date::FromString(date_to), income);
    }
    else if (mode == "PayTax") {
        // writeable:
        // PayTax <дата начала> <дата конца> — заплатить налог 13 % в каждый день указанного диапазона.
        // Это означает простое умножение всей прибыли в диапазоне на 0,87, независимо от того, отдавался ли уже налог за какой - либо из указанных дней.
        // Прибыль за эти дни, которая обнаружится позже, налогами из прошлого не облагается.
        manager.PayTax(Date::FromString(date_from), Date::FromString(date_to));
    }
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
