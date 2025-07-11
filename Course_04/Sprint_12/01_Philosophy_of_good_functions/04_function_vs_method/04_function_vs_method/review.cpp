#include <iostream>
#include <vector>
#include <unordered_map>


using namespace std;

class Library {
public:
    Library() {
        users_.reserve(100000);
        pages_.resize(1000);
    }

    void Read(int user, size_t page) {
        size_t old_page = 0;
        const auto user_it = users_.find(user);
        if (user_it != users_.cend()) {
            old_page = user_it->second;
        }
        users_[user] = page;
        for (size_t i = old_page + 1; i <= page; ++i) {
            ++pages_[i];
        }
    }

    void Cheer(int user) const {
        const auto user_it = users_.find(user);
        double y = 0;
        if (user_it != users_.cend()) {
            const size_t all_users = users_.size();
            const auto users_read_the_same = static_cast<size_t>(pages_[user_it->second]);
            const size_t users_min = all_users - users_read_the_same;
            if (all_users - 1 == 0) {
                y = 1.0;
            }
            else {
                y = static_cast<double>(users_min) / static_cast<double>(all_users - 1);
            }
        }
        else {
            y = 0.0;
        }
        cout << y << endl;
    }
private:
    std::unordered_map<int, size_t> users_;
    vector<size_t> pages_;
};

int main() {
    Library library;
    int queries;
    cin >> queries;
    for (int i = 0; i < queries; ++i) {
        string query_type;
        cin >> query_type;

        if (query_type == "READ") {
            int user;
            cin >> user;
            size_t page;
            cin >> page;
            library.Read(user, page);
        }
        else {
            int user;
            cin >> user;
            library.Cheer(user);
        }
    }
}