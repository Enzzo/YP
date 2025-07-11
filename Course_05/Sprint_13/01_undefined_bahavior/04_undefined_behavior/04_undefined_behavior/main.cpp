#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>

using namespace std;


// clang-format off

const int NUM_PLANETS = 9;
const string_view PLANETS[] = {
    "Mercury"sv, "Venus"sv, "Earth"sv,
    "Mars"sv, "Jupiter"sv, "Saturn"sv,
    "Uranus"sv, "Neptune"sv,
};

// clang-format on

//bool IsPlanet(string_view name) {
//    for (int i = 0; i < NUM_PLANETS; ++i) {
//        if (PLANETS[i] == name) {
//            return true;
//        }
//    }
//    return false;
//}

bool IsPlanet(std::string_view name) {
    
    for (auto i : PLANETS) {
        if (i == name)
            return true;
    }
    return false;
}

void Test(string_view name) {
    cout << name << " is " << (IsPlanet(name) ? ""sv : "NOT "sv)
        << "a planet"sv << endl;
}

int main() {
    //Test("Earth"sv);
    //Test("Jupiter"sv);
    //Test("Pluto"sv);
    //Test("Moon"sv);
    std::string planet_name;
    getline(std::cin, planet_name);
    Test(planet_name);
    return 0;
}