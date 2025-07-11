#pragma once
#include <string>
#include <vector>
#include <memory>
#include <set>

#include "geo.h"

struct Stop {
	std::string name;
	geo::Coordinates coord;
	bool free = true;
};

struct Bus {
	std::string number;
	bool is_ring = false;
	std::vector<std::shared_ptr<Stop>> stops;
};

struct BusInfo {
	uint64_t stops = 0;
	uint64_t unique_stops = 0;
	float route_length = 0;
	double curvature = 1.0;
};

struct BusComparator {
	bool operator()(std::shared_ptr<Bus> lhs,
		std::shared_ptr<Bus> rhs) const {
		return lexicographical_compare(
			lhs->number.begin(), lhs->number.end(),
			rhs->number.begin(), rhs->number.end());
	}
};

struct StopComparator {
	bool operator()(std::shared_ptr<Stop> lhs,
		std::shared_ptr<Stop> rhs) const {
		return lexicographical_compare(
			lhs->name.begin(), lhs->name.end(),
			rhs->name.begin(), rhs->name.end());
	}
};

using Buses = std::set<std::shared_ptr<Bus>, BusComparator>;
using Stops = std::set<std::shared_ptr<Stop>, StopComparator>;

namespace detail {
	class DistanceHasher {
	public:
		size_t operator()(const std::pair<std::shared_ptr<Stop>, std::shared_ptr<Stop>>& p) const {
			std::hash<void*> hasher;
			size_t h1 = hasher(p.first.get());
			size_t h2 = hasher(p.second.get()) * 37 ^ 3;
			return h1 + h2;
		}
	};
}