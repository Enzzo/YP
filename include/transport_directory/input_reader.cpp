#include <iostream>
#include <sstream>
#include <algorithm>

#include "input_reader.h"
void InputReader::TrimString(std::string& str) {
	str.erase(str.find_last_not_of(' ') + 1);
	str.erase(0, str.find_first_not_of(' '));
}

std::pair<int, std::string> InputReader::ParseDescription(std::istream& ist) const {
	int i = 0;
	std::string s;
	ist >> i >> s >> s;
	std::getline(ist, s, ',');
	return std::make_pair(i, s);
}

void InputReader::Request(std::istream& request) {
	std::string mode;
	std::string line;
	request >> mode;
	std::getline(request, line);
	std::istringstream iss(std::move(line));

	//add stop
	if (mode == "Stop") {
		Stop stop;
		std::string stop_name;
		double latitude;
		double longitude;

		std::getline(iss, stop_name, ':');

		TrimString(stop_name);
		stop.name = stop_name;
		iss >> latitude >> line >> longitude >> line;
		stop.coord = { latitude, longitude };
		transport_catalogue_.AddStop(std::move(stop));

		while (!iss.eof()) {
			auto [dist, to_stop] = ParseDescription(iss);
			TrimString(to_stop);
			transport_catalogue_.AddDistanceBetweenStops(stop.name, to_stop, dist);
		}
	}

	//add bus
	else if (mode == "Bus") {
		Bus bus;
		std::string bus_num;

		std::getline(iss, bus_num, ':');
		bus_num.erase(bus_num.find_last_not_of(' ') + 1);
		bus_num.erase(0, bus_num.find_first_not_of(' '));
		bus.number = bus_num;

		std::string description;
		std::getline(iss, description);

		char type = '>';
		int pos = description.find(type);
		if (pos > 0) {
			bus.is_ring = true;
		}
		else {
			type = '-';
		}

		std::vector<std::string>stops;
		std::istringstream ss(description);

		std::string stop;
		while (std::getline(ss, stop, type)) {
			stop.erase(stop.find_last_not_of(' ') + 1);
			stop.erase(0, stop.find_first_not_of(' '));
			stops.push_back(stop);
		}

		for(const std::string& s : stops){
			Stop* found_stop = transport_catalogue_.FindStop(s);
			if (found_stop == nullptr) {
				transport_catalogue_.AddStop({ s, {} });
			}
			bus.stops.push_back(transport_catalogue_.FindStop(s));
		}
		if (!bus.is_ring) {
			auto pred = [&bus, this](const std::string& s) {
				bus.stops.push_back(transport_catalogue_.FindStop(s));
			};
			std::for_each(stops.rbegin() + 1, stops.rend(), pred);
		}
		transport_catalogue_.AddBus(bus);
	}
}