#include "transport_catalogue.h"
using namespace transport_catalogue;
void TransportCatalogue::AddStop(std::string_view stop, const double lat, const double longt)
{
	Stop s = { std::string(stop), {lat, longt}, {} };
	stops_.emplace(stop, std::move(s));
	auto correct_name = stops_.extract(std::string(stop));
	correct_name.key() = correct_name.mapped().name;
	stops_.insert(std::move(correct_name)).position->second;
	stop_to_buses_[stops_.at(stop).name];
}

void TransportCatalogue::AddBus(std::string_view bus, std::vector<std::string_view> stops, std::vector<std::string> end_points)
{
	Bus b = { std::string(bus), {}, 0, 0.0, {} };
	for (auto& s : stops) {
		b.b_stops.push_back(FindStop(s));
	}
	for (auto& end_point : end_points) {
		b.end_points_.push_back(FindStop(end_point));
	}
	buses_.emplace(bus, std::move(b));
	auto correct_name = buses_.extract(std::string(bus));
	correct_name.key() = correct_name.mapped().B_name;
	buses_.insert(std::move(correct_name)).position->second;
	for (auto s : stops) {
		stop_to_buses_[(*FindStop(s)).name].insert((*FindBus(bus)).B_name);
		bus_by_stop_[FindStop(s)].insert(&buses_.at(bus));
	}
	deq_buses_.push_back(&buses_.at(bus));
}

void TransportCatalogue::AddLenth(const std::string_view bus) {
	double geographical_length = 0.0;
	auto it1 = buses_.at(bus).b_stops.begin();
	auto it1_end = buses_.at(bus).b_stops.end();
	auto it2 = it1++;
	for (auto it = it1; it != it1_end; ++it, ++it2) {
		geographical_length += distance::ComputeDistance((*it2)->coordinates, (*it)->coordinates);
	}
	it1 = buses_.at(bus).b_stops.begin();
	it1_end = buses_.at(bus).b_stops.end();
	it2 = it1++;
	buses_[bus].length_ = 0;
	for (auto it = it1; it != it1_end; ++it, ++it2) {
		buses_[bus].length_ += GetDistances((*it2)->name, (*it)->name);
	}
	buses_[bus].curvature_ = buses_[bus].length_ / geographical_length;
}

void TransportCatalogue::AddDistances(const std::string_view stop1, const std::string_view stop2, const int distance)
{
	if (FindStop(stop1)) {
		(*FindStop(stop1)).distances.emplace(FindStop(stop2), distance);
	}
}

[[nodiscard]] inline int TransportCatalogue::GetDistances(const std::string_view stop1, const std::string_view stop2) const
{
	auto stop1_S = FindStop(stop1);
	auto stop2_S = FindStop(stop2);

	if ((*stop1_S).distances.count(stop2_S)) {
		return (*stop1_S).distances.at(stop2_S);
	}
	return (*stop2_S).distances.at(stop1_S);
}

[[nodiscard]] Bus* TransportCatalogue::FindBus(const std::string_view bus) const
{
	if (buses_.count(bus)) {
		return const_cast<Bus*>(&buses_.at(bus));
	}
	else {
		return nullptr;
	}
}

const std::unordered_set<Bus*>& TransportCatalogue::GetBusesByStop(Stop* stop) const {
	static const std::unordered_set<Bus*> dummy;

	auto iter = bus_by_stop_.find(stop);

	return iter == bus_by_stop_.end() ? dummy : iter->second;
}

[[nodiscard]] Stop* TransportCatalogue::FindStop(const std::string_view stop) const
{
	if (stops_.count(stop)) {
		return const_cast<Stop*>(&stops_.at(stop));
	}
	else {
		return nullptr;
	}
}

std::vector<Stop> transport_catalogue::TransportCatalogue::GetStops() const
{
	std::vector<Stop> answer;
	for (auto f : stops_) {
		answer.push_back(f.second);
	}
	return answer;
}
[[nodiscard]] BusInfo TransportCatalogue::GetBusInfo(const std::string_view bus) const noexcept
{
	if (buses_.count(bus) == 0) {
		return BusInfo();
	}
	BusInfo i = { true, static_cast<int>(buses_.at(bus).b_stops.size()), 0, 0, 0.0 };
	std::unordered_set<std::string_view> uniq;
	for (auto& st : buses_.at(bus).b_stops) {
		uniq.emplace(st->name);
	}
	i.uniq_stops = static_cast<int>(uniq.size());
	const Bus& b = buses_.at(bus);
	i.curvature = b.curvature_;
	i.length = b.length_;
	return i;
}


StopInfo TransportCatalogue::GetStopInfo(const std::string_view stop) const noexcept
{
	if (stop_to_buses_.count(stop) == 0) {
		return StopInfo();
	}
	StopInfo si;
	if (stop_to_buses_.at(stop).size() == 0) {
		si.about = "no buses";
	}
	else {
		si.about = "Ok";
		si.stop_buses_ = stop_to_buses_.at(stop);
	}
	return StopInfo(si);
}
