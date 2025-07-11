#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

using namespace std;

int main() {
	TransportCatalogue transport_catalogue;
	InputReader input(transport_catalogue);
	StatReader output(transport_catalogue);

	std::ifstream ifs("input.txt");
	std::ofstream ofs("output5.txt");
	//std::istream& ist = ifs;
	//std::istream& ist = std::cin;

	std::cout << std::setprecision(6);
	std::string skip;
	int base_request_count;
	ifs >> base_request_count;

	ifs.get();
	//make base
	for (int i = 0; i < base_request_count; ++i) {
		input.Request(ifs);
	}
	ifs >> base_request_count;
	ifs.get();
	//read base
	for (int i = 0; i < base_request_count; ++i) {
		output.Request(ifs);
		output.Answer(ofs);
	}
	return 0;
}