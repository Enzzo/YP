#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <algorithm>

using namespace std::literals;

template <typename Hash>
int FindCollisions(Hash& hasher, std::istream& text) {
    std::string word;
    int collisions = 0;
    std::unordered_map<size_t, std::unordered_set<std::string, Hash>> collisions_map;
    
    while (text >> word) {
        size_t h = hasher(word);
        collisions_map[h].emplace(std::move(word));
    }

    std::for_each(collisions_map.begin(), collisions_map.end(), [&collisions](const auto& p) {
        const auto& [_, colls] = p;
        collisions+=colls.size()-1;
        });
    return collisions;
}

// это плохой хешер. Его можно использовать для тестирования.
// Подумайте, в чём его недостаток
struct HasherDummy {
    size_t operator() (const std::string& str) const {
        size_t res = 0;
        for (char c : str) {
            res += static_cast<size_t>(c);
        }
        return res;
    }
};
struct DummyHash {
    size_t operator()(const std::string&) const {
        return 42;
    }
};

int main() {
    DummyHash dummy_hash;
    std::hash<std::string> good_hash;

    {
        std::istringstream stream("I love C++"s);
        std::cout << FindCollisions(dummy_hash, stream) << std::endl;
    }
    {
        std::istringstream stream("I love C++"s);
        std::cout << FindCollisions(good_hash, stream) << std::endl;
    }
}