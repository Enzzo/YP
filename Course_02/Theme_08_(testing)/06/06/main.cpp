#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

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
    else q.type = QueryType::AllBuses;

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

    if (r.buses.size() == 0) {
        os << "No stop";
    }
    else {
        os << "Stop " << r.stop << ":";
        for (const std::string bus : r.buses) {
            os << " " << bus;
        }
    }
    return os;
}

struct StopsForBusResponse {
    // Наполните полями эту структуру
    std::vector<std::string> buses;
    std::vector<std::string> stops;
};

std::ostream& operator<<(std::ostream& os, const StopsForBusResponse& r) {
    // Реализуйте эту функцию

    if (r.stops.size() == 0) {
        os << "No bus";
    }
    else {

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
    std::map<std::string, std::vector<std::string>> buses_for_stop;
    std::map<std::string, std::vector<std::string>> stops_for_bus;

public:
    void AddBus(const std::string& bus, const std::vector<std::string>& stops) {
        // Реализуйте этот метод DONE

        //Если автобуса не существует, то создаём его, поместив в список
        if (stops_for_bus.count(bus) == 0) {
            stops_for_bus.emplace(bus, stops);
        }

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
            return { stop, empty};
        }
        return { stop, buses_for_stop.at(stop) };
    }

    StopsForBusResponse GetStopsForBus(const std::string& bus) const {
        /*
        На запрос STOPS_FOR_BUS bus выведите описания остановок маршрута bus в отдельных строках в том порядке, 
        в котором они были заданы в соответствующей команде NEW_BUS.Описание каждой остановки stop должно иметь вид 
        Stop stop : bus1 bus2 ..., где bus1 bus2 ... — список автобусов, проезжающих через остановку stop.
        Список должен быть в том порядке, в каком автобусы создавались командами NEW_BUS, за исключением исходного маршрута bus.
        Если через остановку stop не проезжает ни один автобус кроме bus, вместо списка автобусов для неё выведите "no interchange".
        Если маршрут bus не существует, выведите No bus.
        */
        // Реализуйте этот метод



        if (stops_for_bus.count(bus) == 0) {
            return { std::vector<std::string>(), std::vector<std::string>()};
        }
        return { bus, stops_for_bus.at(bus) };
    }

    AllBusesResponse GetAllBuses() const {
        // Реализуйте этот метод
        return{ stops_for_bus };
    }
};

void TestAllBuses() {
    BusManager bm;
    std::ostringstream left;
    std::ostringstream right;
    std::istringstream input;
    
    left << bm.GetAllBuses();
    right << "No buses";
    assert(left.str() == right.str());

    left.str("");
    right.str("");

    Query q;
    input.str("NEW_BUS 777 4 one two three ten");
    input >> q;
    bm.AddBus(q.bus, q.stops);

    left << bm.GetAllBuses();
    right << "Bus 777: one two three ten" << std::endl;
    assert(left.str() == right.str());

    left.str("");
    right.str("");

    input.clear();
    input.str("NEW_BUS 777 3 one two three");
    input >> q;
    bm.AddBus(q.bus, q.stops);
    left << bm.GetAllBuses();
    right << "Bus 777: one two three ten" << std::endl;
    assert(left.str() == right.str());
    
    input.clear();
    input.str("NEW_BUS 333 1 four");
    input >> q;
    bm.AddBus(q.bus, q.stops);

    left.str("");
    right.str("");
    left << bm.GetAllBuses();
    right << "Bus 333: four" << std::endl << "Bus 777: one two three ten" << std::endl;
    assert(left.str() == right.str());

    left.str("");
    right.str("");

    std::cout << "TestAllBuses OK\n";
}

void TestBusesForStop() {
    BusManager bm;
    std::ostringstream left;
    std::ostringstream right;
    std::istringstream input;
    left << bm.GetBusesForStop("STOP");
    right << "No stop";

    assert(left.str() == right.str());
    left.str("");
    right.str("");

    Query q;
    input.str("NEW_BUS bus1 3 stop1 stop2 stop3");
    input >> q;
    bm.AddBus(q.bus, q.stops);
    input.clear();

    input.str("NEW_BUS bus2 3 stop4 stop2 stop5");
    input >> q;
    bm.AddBus(q.bus, q.stops);
    input.clear();

    //несуществующая остановка
    input.str("BUSES_FOR_STOP no_stop");
    input >> q;
    left << bm.GetBusesForStop(q.stop);
    right << "No stop";
    assert(left.str() == right.str());
    left.str("");
    right.str("");
    input.clear();

    input.str("BUSES_FOR_STOP stop1");
    input >> q;
    //существующая остановка для одного автобуса
    left << bm.GetBusesForStop(q.stop);
    right << "Stop stop1: bus1";
    assert(left.str() == right.str());
    left.str("");
    right.str("");
    input.clear();

    //существующая остановка для двух автобусов
    input.str("BUSES_FOR_STOP stop2");
    input >> q;
    left << bm.GetBusesForStop(q.stop);
    right << "Stop stop2: bus1 bus2";
    assert(left.str() == right.str());
    left.str("");
    right.str("");

    std::cout << "TestBusesForStops OK\n";
}

void TestStopsForBus() {
    BusManager bm;
    Query q;
    std::ostringstream left;
    std::ostringstream right;
    std::istringstream input;

    //ПУСТОЙ КЛАСС
    input.str("STOPS_FOR_BUS no_bus");
    input >> q;
    left << bm.GetStopsForBus(q.bus);
    right.str("No bus");
    assert(left.str() == right.str());
    left.str("");
    right.str("");
    input.clear();

    //НЕ ПУСТОЙ КЛАСС
    
    input.str("NEW_BUS bus1 3 stop1 stop2 stop3");
    input >> q;
    bm.AddBus(q.bus, q.stops);
    input.clear();

    input.str("NEW_BUS bus2 3 stop4 stop2 stop5");
    input >> q;
    bm.AddBus(q.bus, q.stops);
    input.clear();
    
    input.str("NEW_BUS bus3 5 stop6 stop2 stop7 stop8 stop1");
    input >> q;
    bm.AddBus(q.bus, q.stops);
    input.clear();

        //НЕ ИМЕЕТСЯ СОВПАДЕНИЙ
    input.str("STOPS_FOR_BUS no_bus");
    input >> q;
    left << bm.GetStopsForBus(q.bus);
    right.str("No bus");
    assert(left.str() == right.str());
    left.str("");
    right.str("");
    input.clear();

        //ИМЕЕТСЯ ОДНО СОВПАДЕНИЕ (ОДНА ОСТАНОВКА У ОДНОГО АВТОБУСА)
    input.str("STOPS_FOR_BUS bus1");
    input >> q;
    left << bm.GetStopsForBus(q.bus);
    right.str("No bus");
    assert(left.str() == right.str());
    left.str("");
    right.str("");
    input.clear();
        //ИМЕЕТСЯ У НЕСКОЛЬКИХ АВТОБУСОВ
    std::cout << "TestsStopForBus OK\n";
};

void Testing() {
    TestAllBuses();
    TestBusesForStop();
    TestStopsForBus();
}
// Не меняя тела функции main, реализуйте функции и классы выше
int main() {
    Testing();
    
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
    return 0;
}