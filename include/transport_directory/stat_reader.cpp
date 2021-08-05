#include <algorithm>
#include <set>

#include "stat_reader.h"

void StatReader::Request(std::istream& ist){
	ist >> mode_;

	std::getline(ist, description_);
	description_.erase(description_.find_last_not_of(' ') + 1);
	description_.erase(0, description_.find_first_not_of(' '));
}
void StatReader::Answer(std::ostream& ost)const {

	if (mode_ == "Bus") {
		ost << "Bus " << description_ << ": ";
		const Bus* bus = transport_catalogue_.FindBus(description_);
		if (!bus) {
			ost << "not found" << std::endl;
		}
		else {
			auto b = transport_catalogue_.GetBusInfo(*bus);
			ost << b.stops << " stops on route, " 
				<< b.unique_stops << " unique stops, " 
				<< b.route_length << " route length, " 
				<< b.curvature << " curvature"
				<< std::endl;
		}
	}
	else if (mode_ == "Stop") {
		ost << "Stop " << description_ << ": ";
		const Stop* stop = transport_catalogue_.FindStop(description_);
		if (!stop) {
			ost << "not found" << std::endl;
		}
		else {
			std::unordered_set<Bus*> buses = transport_catalogue_.GetBusesByStop(description_);
			std::set<std::string> bus_numbers;
			std::for_each(buses.begin(), buses.end(), [&bus_numbers](const Bus* b) {
				bus_numbers.emplace(b->number);
				});
			if (buses.size() == 0) {
				ost << "no buses" << std::endl;
			}
			else {
				ost << "buses";
				for (const std::string& bus : bus_numbers){
					ost <<" "<<bus;
				}
				ost << std::endl;
			}
		}
	}
}