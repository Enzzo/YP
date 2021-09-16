#pragma once
#include "date.h"
#include "parser.h"

#include <vector>
#include <iostream>

class BudgetManager {

    struct DailyInfo {
        double earned_;
        double spend_;
    };

    std::vector<DailyInfo> general_info_;

public:

    BudgetManager() : general_info_(Date::ComputeDistance(START_DATE, END_DATE)) {};

    inline static const Date START_DATE{ 2000, 1, 1 };
    inline static const Date END_DATE{ 2100, 1, 1 };

    double ComputeIncome(const Date& from, const Date& to) const {
        auto start_date = Date::ComputeDistance(START_DATE, from);
        auto to_date = Date::ComputeDistance(START_DATE, to);
        double result = 0.0;
        for (int i = start_date; i <= to_date; ++i) {
            result += general_info_[i].earned_;
        }
        return result;
    }   

    void Earn(const Date& from, const Date& to, double income) {
        auto start_date = Date::ComputeDistance(START_DATE, from);
        auto to_date = Date::ComputeDistance(START_DATE, to);
        for (int i = start_date; i <= to_date; ++i) {
            general_info_[i].earned_ += (income / (to_date - start_date + 1));
        }
    }

    void PayTax(const Date& from, const Date& to) {
        auto start_date = Date::ComputeDistance(START_DATE, from);
        auto to_date = Date::ComputeDistance(START_DATE, to);
        for (int i = start_date; i <= to_date; ++i) {
            general_info_[i].earned_ *= .87;
        }
    }

    void Write(const Request& r) {

    }
};