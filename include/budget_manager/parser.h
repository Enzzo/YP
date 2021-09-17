#pragma once
#include "date.h"
#include "budget_manager.h"

#include <iostream>
#include <sstream>

// напишите в этом классе код, ответственный за чтение запросов

// ¬ этой задаче удобно создать абстрактный класс дл€ запроса.
// “акже можно создать два его наследника : запрос - чтение и запрос - модификаци€.
// ќт них можно наследоватьс€ всем остальным запросам.

class Parser {
	BudgetManager& budget_manager_;
	std::unique_ptr<Request> request_;

public:
	Parser(BudgetManager& budget_manager, std::string_view request) : budget_manager_(budget_manager) {
		std::istringstream istr(std::string(request.begin(), request.end()));
		std::string mode;
		std::string date_from;
		std::string date_to;

		istr >> mode >> date_from >> date_to;

		if (mode == "ComputeIncome") {
			request_ = std::make_unique<ComputeIncomeRequest>(Date::FromString(date_from), Date::FromString(date_to));
		}
		else if (mode == "Earn") {
			int earned;
			istr >> earned;
			request_ = std::make_unique<EarnRequest>(Date::FromString(date_from), Date::FromString(date_to), earned);
		}
		else if (mode == "PayTax") {
			request_ = std::make_unique<PayTaxRequest>(Date::FromString(date_from), Date::FromString(date_to));
		}
		auto x = budget_manager_.Manage(request_->GetResult());
		if (*x) {
			std::cout << *x << std::endl;
		}
	}
};