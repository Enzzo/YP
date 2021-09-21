#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <sstream>

using namespace std;

class RouteManager {
public:
    void AddRoute(int start, int finish) {
        reachable_lists_[start].insert(finish);
        reachable_lists_[finish].insert(start);
    }

    int FindNearestFinish(int start, int finish) const {
        int result = abs(start - finish);

        if (reachable_lists_.count(start) < 1) {
            return result;
        }

        const set<int>& reachable_stations = reachable_lists_.at(start);

        if (!reachable_stations.empty()) {
            auto it = reachable_stations.lower_bound(finish);
            if (*it == finish) {
                return 0;
            }
            if (it == reachable_stations.end()) {
                result = std::min(result, std::abs(*(--it) - finish));
            }
            else if (it == reachable_stations.begin()) {
                result = std::min(result, std::abs(*it - finish));
            }
            else {
                result = std::min({ result,
                    std::abs(*it - finish),
                    std::abs(*(--it) - finish)
                    });
            }
        }

        return result;
    }

private:
    map<int, set<int>> reachable_lists_;
};

int main() {
    RouteManager routes;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int start, finish;
        cin >> start >> finish;
        if (query_type == "ADD"s) {
            routes.AddRoute(start, finish);
        }
        else if (query_type == "GO"s) {
            cout << routes.FindNearestFinish(start, finish) << "\n"s;
        }
    }
}