#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>
#include <cassert>
#include <assert.h>

using namespace std::literals;

int EffectiveCount(const std::vector<int>& v, int n, int i) {
    // место для вашего решения

    std::vector<int>::const_iterator it;

    int part = static_cast<int64_t>(v.size()) * i / n;

    int log = std::log2(v.size());

    if (part < log) {
        std::cout << "Using find_if\n"s;
        it = std::find_if(v.begin(), v.end(), [i](const int j) {
            return i <= j;
            });
    }
    else {
        std::cout << "Using upper_bound\n"s;
        it = std::upper_bound(v.begin(), v.end(), i);
    }
    return std::distance(v.begin(), it);
}

int main() {
    static const int NUMBERS = 1'000'000;
    static const int MAX = 1'000'000'000;

    std::mt19937 r;
    std::uniform_int_distribution<int> uniform_dist(0, MAX);

    std::vector<int> nums;
    for (int i = 0; i < NUMBERS; ++i) {
        int random_number = uniform_dist(r);
        nums.push_back(random_number);
    }

    std::sort(nums.begin(), nums.end());

    int i;
    std::cin >> i;
    int result = EffectiveCount(nums, MAX, i);
    std::cout << "Total numbers before "s << i << ": "s << result << std::endl;

    return 0;
}