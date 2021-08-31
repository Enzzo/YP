#include <iostream>

using namespace std;

class HotelManager {
public:
    void Book(const int64_t /*time*/, const std::string& /*hotel name*/, const int /*client id*/, const int /*rooms count*/);
    int ComputeClientCount(const std::string& /*hotel name*/);
    int ComputeRoomCount(const std::string& /*hotel name*/);

private:
    
};

int main() {
    HotelManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            int64_t time;
            cin >> time;
            string hotel_name;
            cin >> hotel_name;
            int client_id, room_count;
            cin >> client_id >> room_count;
            manager.Book(time, hotel_name, client_id, room_count);
        }
        else {
            string hotel_name;
            cin >> hotel_name;
            if (query_type == "CLIENTS") {
                cout << manager.ComputeClientCount(hotel_name) << "\n";
            }
            else if (query_type == "ROOMS") {
                cout << manager.ComputeRoomCount(hotel_name) << "\n";
            }
        }
    }

    return 0;
}