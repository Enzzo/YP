#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& ost, const std::pair<T1, T2>& p) {
    ost << '(' << p.first << ", " << p.second << ')';
    return ost;
}

template<typename Container>
void Print(std::ostream& ost, const Container& container) {
    bool first = true;
    for (const auto& e : container) {
        if (!first)ost << ", ";
        else first = false;
        ost << e;
    }
}

template<typename Elem>
std::ostream& operator<<(std::ostream& ost, const std::vector<Elem>& v) {
    ost << '[';
    Print(ost, v);
    ost << ']';
    return ost;
}

template<typename Elem>
std::ostream& operator<<(std::ostream& ost, const std::set<Elem>& v) {
    ost << '{';
    Print(ost, v);
    ost << '}';
    return ost;
}

template<typename Key, typename Value>
std::ostream& operator<<(std::ostream& ost, const std::map<Key, Value>& v) {
    ost << '<';
    Print(ost, v);
    ost << '>';
    return ost;
}

int main() {
    std::pair<int, int>p = { 1, 2 };
    std::cout << p << std::endl;
    const map<string, int> cat_ages = {
        {"Мурка", 10},
        {"Белка", 5},
        {"Георгий", 2},
        {"Рюрик", 12}
    };
    cout << cat_ages << endl;
    return 0;
}