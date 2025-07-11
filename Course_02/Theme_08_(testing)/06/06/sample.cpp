#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
 
using namespace std;
 
enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};
 
struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};
 
istream& operator>>(istream& is, Query& q) {
    // Реализуйте эту функцию
    string command;
    is >> command;
    if (command == "NEW_BUS"s) {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int stop_count = 0;
        is >> stop_count;
        string stop;
        for (int i = 0; i < stop_count; ++i) {
            is >> stop;
            q.stops.push_back(stop);
        }
    } else if (command == "BUSES_FOR_STOP"s) {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    } else if (command == "STOPS_FOR_BUS"s) {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    } else if (command == "ALL_BUSES"s) {
        q.type = QueryType::AllBuses;        
    }
    return is;
}
 
struct BusesForStopResponse {
    // Наполните полями эту структуру
    string stop;
    vector<string> buses;
};
 
ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    // Реализуйте эту функцию
    if (r.buses.empty()) {
        os << "No stop"s;
    } else {
        os << "Stop "s << r.stop << ": "s;
        for (const string &bus : r.buses) {
            os << bus << " "s;
        }
    }
    return os;
}
 
struct StopToInterBuses {
    string stop;
    vector<string> inter_buses;
};
 
struct StopsForBusResponse {
    // Наполните полями эту структуру
    string bus;
    vector<StopToInterBuses> stops_to_buses;
};
 
ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    // Реализуйте эту функцию
    if (r.stops_to_buses.empty()) {
        os << "No bus"s;
    } else {
        int i = 0;
        for (const auto &[stop, buses] : r.stops_to_buses) {
            ++i;
            os << "Stop "s << stop << ": "s;
            if (buses.size() == 1) {
                os << "No interchange"s;
            } else {
                for (const string &bus : buses) {
                    if (r.bus != bus) {
                        os << bus << " "s;
                    }
                }
            }
            if (static_cast<int>(r.stops_to_buses.size()) > i) {
                os << endl;
            }
        }
    }
    return os;
}
 
struct AllBusesResponse {
    // Наполните полями эту структуру
    map<string, vector<string>> buses_to_stops;
};
 
ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    // Реализуйте эту функцию
    if (r.buses_to_stops.empty()) {
        os << "No buses"s;
    } else {
        int i = 0;
        for (const auto &[bus, stops] : r.buses_to_stops) {
            ++i;
            os << "Bus "s << bus << ": "s;
            for (const string &stop : stops) {
                os << stop << " "s;
            }
            if (i < r.buses_to_stops.size()) {
                os << endl;
            }
        }
    }
    return os;
}
 
class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        // Реализуйте этот метод
        buses_to_stops_[bus] = stops;
        for (const string &stop : stops) {
            vector<string> &buses = stops_to_buses_[stop];
            buses.push_back(bus);
        }
    }
 
    BusesForStopResponse GetBusesForStop(const string& stop) const {
        // Реализуйте этот метод
        BusesForStopResponse r;
        r.stop = stop;
        if (stops_to_buses_.count(stop)) {
            r.buses = stops_to_buses_.at(stop);
        }
        return r;
    }
 
    StopsForBusResponse GetStopsForBus(const string& bus) const {
        // Реализуйте этот метод
        StopsForBusResponse r;
        r.bus = bus;
        if (buses_to_stops_.count(bus)) {
            for (const string &stop : buses_to_stops_.at(bus)) {
                r.stops_to_buses.push_back({stop, stops_to_buses_.at(stop)});
            }
        }
        return r;
    }
 
    AllBusesResponse GetAllBuses() const {
        // Реализуйте этот метод
        AllBusesResponse r;
        r.buses_to_stops = buses_to_stops_;
        return r;
    }
private:
    map<string, vector<string>> buses_to_stops_, stops_to_buses_;
};
 
// Не меняя тела функции main, реализуйте функции и классы выше
 
int main() {
    int query_count;
 
    cin >> query_count;
 
    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        Query q;
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }
}