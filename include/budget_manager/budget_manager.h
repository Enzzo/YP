#pragma once
#include "date.h"

#include <vector>
#include <iostream>
#include <optional>

enum class EnumOperationType {
    NONE,
    COMPUTEINCOME,
    PAYTAX,
    EARN    
};

struct Result {
    EnumOperationType operation_type;
    Date from, to;
    std::optional<double> param;
};

//------------------------- Request Interface -------------------------
class Request {
protected:
    Result result_;

public:
    Result GetResult() {
        return result_;
    }
};


//------------------------- Requests -------------------------
class ReadableRequest : public Request {
};

class WriteableRequest : public Request {
};


//------------------------- Writeable Requests -------------------------
class ComputeIncomeRequest final : public WriteableRequest {
public:
    ComputeIncomeRequest(const Date from, const Date to) {
        result_.from = from;
        result_.to = to;
    }
};

class PayTaxRequest final : public WriteableRequest {
public:
    PayTaxRequest(const Date from, const Date to) {
        result_.from = from;
        result_.to = to;
    }
};

//------------------------- Readable Requests -------------------------
class EarnRequest final : public ReadableRequest {
public:
    EarnRequest(const Date from, const Date to, double earn) {
        result_.from = from;
        result_.to = to;
        result_.param = earn;
    }
};

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

    std::optional<double> Manage(const Result result) {
        auto [type, date_from, date_to, param1] = result;
        auto start_date = Date::ComputeDistance(START_DATE, date_from);
        auto to_date = Date::ComputeDistance(START_DATE, date_to);

        switch (type) {
        case EnumOperationType::COMPUTEINCOME: {
            return ComputeIncome(start_date, to_date);
            break;
        }
        case EnumOperationType::PAYTAX: {
            PayTax(start_date, to_date);
            break;
        }
        case EnumOperationType::EARN: {
            Earn(start_date, to_date, param1.value());
            break;
        }
        }
    }
private:
    double ComputeIncome(const int from, const int to) const {
        double result = 0.0;
        for (int i = from; i <= to; ++i) {
            result += general_info_[i].earned_;
        }
        return result;
    }

    void Earn(const int from, const int to, double income) {
        for (int i = from; i <= to; ++i) {
            general_info_[i].earned_ += (income / (from - to + 1));
        }
    }

    void PayTax(const int from, const int to) {
        for (int i = from; i <= to; ++i) {
            general_info_[i].earned_ *= .87;
        }
    }
};