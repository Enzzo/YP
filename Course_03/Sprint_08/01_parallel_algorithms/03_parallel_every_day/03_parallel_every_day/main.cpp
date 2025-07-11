#include <algorithm>
#include <iostream>
#include <string_view>
#include <execution>

using namespace std::literals;

int CountWords(std::string_view str) {
    return std::count(std::execution::par, str.begin(), str.end(), ' ') + 1;
}

int main() {
    std::cout << CountWords("pretty little octopus"s) << std::endl;
    // должно вывести 3

    return 0;
}