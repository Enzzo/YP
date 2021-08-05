#pragma once

#include <string_view>
#include <string>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>

#include "geo.h"

struct Stop {
	std::string name;
	Coordinates coord;
};

struct Bus {
	std::string number;
	bool is_ring = false;
	std::vector<Stop*> stops;
};

struct BusInfo {
	int stops = 0;    
	int unique_stops = 0;    
	double route_length = 0.0;
	double curvature = 0.0;
};

class DistanceHasher {
public:
	size_t operator()(const std::pair<Stop*, Stop*>& p) const{
		std::hash<const void*> sh1;
		std::hash<const void*> sh2;
		size_t h1 = sh1(p.first);
		size_t h2 = sh2(p.second) * 37^3;
		return h1 + h2;
	}
};

class TransportCatalogue {
	std::deque<Bus>														buses_;
	std::deque<Stop>													stops_;
	std::unordered_map<std::string, Bus*>								index_buses_;
	std::unordered_map<std::string, Stop*>								index_stops_;
	std::unordered_set<Bus*>											ptr_buses_;
	std::unordered_set<Stop*>											ptr_stops_;
	std::unordered_map<Stop*, std::unordered_set<Bus*>>					buses_for_stops_;
	std::unordered_map<std::pair<Stop*, Stop*>, int, DistanceHasher>	distance_between_stops_;

public:
	//добавление маршрута в базу
	void AddBus(const Bus&);
	//добавление остановки в базу
	void AddStop(const Stop&);
	//поиск маршрута по имени
	Bus* FindBus(const std::string& bus_number)const;
	//поиск остановки по имени
	Stop* FindStop(const std::string& stop_name)const;
	//получение информации о маршруте
	const BusInfo GetBusInfo(const Bus& bus)const;

	const std::unordered_set<Bus*> GetBusesByStop(const std::string&) const;

	void AddDistanceBetweenStops(const std::string&, const std::string&, const int);
	[[nodiscard]]inline int GetDistanceBetweenStops(const std::string&, const std::string&) const ;
};