#pragma once
#include <iostream>

#include "transport_catalogue.h"

class InputReader{
	TransportCatalogue& transport_catalogue_;

public:
	InputReader() = delete;
	explicit InputReader(TransportCatalogue& transport_catalogue) : transport_catalogue_(transport_catalogue) {};

	void Request(std::istream& request = std::cin);

private:
	void TrimString(std::string&);
	std::pair<int, std::string> ParseDescription(std::istream&)const;
};