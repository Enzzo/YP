#pragma once
#include "date.h"

// напишите в этом классе код, ответственный за чтение запросов

// ¬ этой задаче удобно создать абстрактный класс дл€ запроса.
// “акже можно создать два его наследника : запрос - чтение и запрос - модификаци€.
// ќт них можно наследоватьс€ всем остальным запросам.

//------------------------- Request Interface -------------------------
class Request {
public:
	virtual void ReadRequest(const std::string_view) = 0;
};


//------------------------- Requests -------------------------
class ReadableRequest : public Request {
public:
	virtual void ReadRequest(const std::string_view) override = 0;
};

class WriteableRequest : public Request {
public:
	virtual void ReadRequest(const std::string_view) override = 0;
};


//------------------------- Writeable Requests -------------------------
class ComputeIncomeRequest final : public WriteableRequest {
public:
	void ReadRequest(const std::string_view request) override {

	}
};

class PayTaxRequest final : public WriteableRequest {
public:
	void ReadRequest(const std::string_view request) override {

	}
};

//------------------------- Readable Requests -------------------------
class EarnRequest final : public ReadableRequest {
public:
	void ReadRequest(const std::string_view request) override {/*
		std::istringstream istr(std::move(std::string(request.begin(), request.end())));
		std::string date_from;
		std::string date_to;

		std::getline(istr, date_from, ' ');
		std::getline(istr, date_to, ' ');
		auto start_date = Date::ComputeDistance(START_DATE, from);
		auto to_date = Date::ComputeDistance(START_DATE, to);
		for (int i = start_date; i <= to_date; ++i) {
			general_info_[i].earned_ += (income / (to_date - start_date + 1));
		}*/
	}
};

class Parser {
	Request* request_;

public:
	Parser() = delete;

	Parser(const std::string_view request) {
		std::istringstream iss(std::move(std::string(request.begin(), request.end())));
		std::string mode;

		iss >> mode;

		if (mode == "ComputeIncome") {
			request_ = ComputeIncomeRequest();
		}
		else if (mode == "Earn") {
			request_ = EarnRequest();
		}
		else if (mode == "PayTax") {
			request_ = PayTaxRequest();
		}
	};
};