#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

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
    // Реализуйте эту функцию
    std::string command;
    is >> command;

    if (command == "NEW_BUS") q.type = QueryType::NewBus;
    else if (command == "BUSES_FOR_STOP") q.type = QueryType::BusesForStop;
    else if (command == "STOPS_FOR_BUS") q.type = QueryType::StopsForBus;
    else if (command == "ALL_BUSES") q.type = QueryType::AllBuses;

    switch (q.type) {
    case QueryType::NewBus: {
        int stop_count;
        q.stops.clear();
        is >> q.bus >> stop_count;
        while (stop_count-- > 0) {
            std::string stop;
            is >> stop;
            q.stops.push_back(stop);
        }
        break;
    }
    case QueryType::BusesForStop: is >> q.stop; break;
    case QueryType::StopsForBus: is >> q.bus; break;
    case QueryType::AllBuses:break;
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

    if (r.buses.empty()) {
        os << "No stop";
    }
    else {
        for (const std::string& bus : r.buses) {
            os << bus << " ";
        }
    }
    return os;
}

struct StopsForBusResponse {
    // Наполните полями эту структуру
    const std::string& bus;
    const std::map<std::string, std::vector<std::string>>& stops_for_buses;
    const std::vector<std::string> buses;
};

std::ostream& operator<<(std::ostream& os, const StopsForBusResponse& r) {
    // Реализуйте эту функцию

    if (r.stops_for_buses.count(r.bus) == 0) {
        os << "No bus";
    }
    else {
        
        const std::vector<std::string>& stops_for_bus = r.stops_for_buses.at(r.bus);
        const std::vector<std::string>& all_buses = r.buses;

        bool first = true;
        for (const std::string& stop : stops_for_bus) {            
            if (!first) os << std::endl;
            else first = false;
            os << "Stop " << stop << ":";
            int count = 0;

            //TODO:             
            for (const std::string& bus : all_buses){                
                if (bus == r.bus)continue;
                for (const std::string& stops : r.stops_for_buses.at(bus)) {
                    if(stop == stops) {
                        count++;
                        os << " " << bus;
                    }                   
                }             
            }
            
            if (count == 0) {
                os << " no interchange";
            }
        }
    }
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
        bool first = true;
        for (auto& [bus, stops] : r.buses) {
            if (!first) os << std::endl;
            else first = false;
            os << "Bus " << bus << ":";
            for (const std::string& stop : stops) {
                os << " " << stop;
            }
        }
    }
    return os;
}

class BusManager {
    std::map<std::string, std::vector<std::string>> buses_for_stop;
    std::map<std::string, std::vector<std::string>> stops_for_bus;
    std::vector<std::string> all_buses;
public:
    Query query;

    void AddBus(const std::string& bus, const std::vector<std::string>& stops) {
        // Реализуйте этот метод DONE

        if (std::count(all_buses.begin(), all_buses.end(), bus) == 0)
            all_buses.push_back(bus);

        stops_for_bus[bus] = stops;
        std::vector<std::string>& _bus = stops_for_bus.at(bus);

        for (const std::string& stop : stops) {
            if (std::count(_bus.begin(), _bus.end(), stop) == 0) {
                _bus.push_back(stop);
            }
            //Если в списке остановок не существует остановки stop, то помещаем её в список и присваиваем номер автобуса
            if (buses_for_stop.count(stop) == 0) {
                buses_for_stop.emplace(stop, std::vector<std::string>({ bus }));
            }
            //Если к остановке не привязан автобус, то привязываем его к этой остановке
            std::vector<std::string>& _stop = buses_for_stop.at(stop);
            if (std::count(_stop.begin(), _stop.end(), bus) == 0)
                _stop.push_back(bus);

        }
    };

    BusesForStopResponse GetBusesForStop(const std::string& stop) const {
        // Реализуйте этот метод
        if (buses_for_stop.count(stop) == 0) {
            std::vector<std::string>empty;
            return { stop, empty };
        }
        return { stop, buses_for_stop.at(stop) };
    }

    StopsForBusResponse GetStopsForBus(const std::string& bus) const {
        return { bus, stops_for_bus, all_buses };
    }

    AllBusesResponse GetAllBuses() const {
        // Реализуйте этот метод
        return{ stops_for_bus };
    }
};
int main() {
    int query_count;
    Query q;

    std::ifstream ifs("input.txt");
    std::istream& is = ifs;

    is >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {

        is >> q;
        switch (q.type) {
        case QueryType::NewBus:
            bm.AddBus(q.bus, q.stops);
            break;
        case QueryType::BusesForStop:
            cout << bm.GetBusesForStop(q.stop) << std::endl;
            break;
        case QueryType::StopsForBus:
            cout << bm.GetStopsForBus(q.bus) << std::endl;
            break;
        case QueryType::AllBuses:
            cout << bm.GetAllBuses() << std::endl;
            break;
        }
    }
}