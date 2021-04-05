#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    std::string bus;
    std::string stop;
    std::vector<std::string> stops;
};

std::istream& operator>>(std::istream& is, Query& q) {
    // Реализуйте эту функцию DONE
    int t;
    is >> t;
    q.type = static_cast<QueryType>(t);
    switch (q.type) {
    case QueryType::NewBus: is >> q.bus >> q.stop; break;
    case QueryType::BusesForStop: is >> q.stop; break;
    case QueryType::StopsForBus: is >> q.bus; break;
    }
    return is;
}

struct BusesForStopResponse {
    // Наполните полями эту структуру
    std::string stop;
    std::vector<std::string> buses;
};

std::ostream& operator<<(std::ostream& os, const BusesForStopResponse& r) {
    // Реализуйте эту функцию
    return os;
}

struct StopsForBusResponse {
    // Наполните полями эту структуру
    std::string bus;
    std::vector<std::string> stops;
};

std::ostream& operator<<(std::ostream& os, const StopsForBusResponse& r) {
    // Реализуйте эту функцию
    return os;
}

struct AllBusesResponse {
    // Наполните полями эту структуру
    std::map<std::string, std::vector<std::string>> buses;
};

std::ostream& operator<<(std::ostream& os, const AllBusesResponse& r) {
    // Реализуйте эту функцию
    if (r.buses.size() == 0) os << "No buses";
    else {
        for (auto& [bus, stops] : r.buses) {
            os << "Bus " << bus<<":";
            for (const std::string& stop : stops) {               
                os << " " << stop;
            }
            os << std::endl;
        }        
    }
    return os;
}

class BusManager {
    std::map<std::string, std::vector<std::string>> buses_for_stops;
    std::map<std::string, std::vector<std::string>> stops_for_buses;

public:
    void AddBus(const std::string& bus, const std::vector<std::string>& stops) {
        // Реализуйте этот метод DONE
        stops_for_buses[bus] = stops;

        for (const std::string& stop : stops) {
            buses_for_stops[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const std::string& stop) const {
        // Реализуйте этот метод
        return { stop, buses_for_stops.at(stop) };
    }

    StopsForBusResponse GetStopsForBus(const std::string& bus) const {
        // Реализуйте этот метод
        return { bus, stops_for_buses.at(bus) };
    }

    AllBusesResponse GetAllBuses() const {
        // Реализуйте этот метод
        return{ stops_for_buses };
    }
};

void TestAllBuses() {
    BusManager bm;
    std::ostringstream left;
    std::ostringstream right;

    left << bm.GetAllBuses();
    right << "No buses";
    assert(left.str() == right.str());
    left.str("");
    right.str("");

    bm.AddBus("777", { "one", "two", "three" });

    left << bm.GetAllBuses();
    right << "Bus 777: one two three" << std::endl;
    assert(left.str() == right.str());
    left.str("");
    right.str("");

    bm.AddBus("333", { "four"});
    left << bm.GetAllBuses();
    right << "Bus 333: four" << std::endl << "Bus 777: one two three" << std::endl ;
    
    std::cout << left.str() << std::endl;
    std::cout << right.str();
    assert(left.str() == right.str());
    left.str("");
    right.str("");

    std::cout << "TestAllBuses OK\n";
}

void Testing() {
    TestAllBuses();
}
// Не меняя тела функции main, реализуйте функции и классы выше
int main() {
    Testing();
    /*
    int query_count;
    Query q;

    std::cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        std::cin >> q;
        switch (q.type) {
        case QueryType::NewBus:
            bm.AddBus(q.bus, q.stops);
            break;
        case QueryType::BusesForStop:
            std::cout << bm.GetBusesForStop(q.stop) << std::endl;
            break;
        case QueryType::StopsForBus:
            std::cout << bm.GetStopsForBus(q.bus) << std::endl;
            break;
        case QueryType::AllBuses:
            std::cout << bm.GetAllBuses() << std::endl;
            break;
        }
    }
    */
    return 0;
}