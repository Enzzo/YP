#include <iostream>

using namespace std::literals;

template <typename F>
int FindFloor(int n, F drop) {
    // упростите этот алгоритм за O(n),
    // сделав из него логарифмический
    
    /*
    for (int i = 1; i < n; ++i) {
        if (drop(i)) {
            return i;
        }
    }
    */

    int a = 1, b = n;
    while (a != b) {
        int m = (a + b) / 2;
        if (drop(m)) {
            b = m;
        }
        else {
            a = m + 1;
        }
    }

    return b;
}

int main() {
    int n, t;
    std::cout << "Enter n and target floor number: "s << std::endl;
    std::cin >> n >> t;

    int count = 0;
    int found = FindFloor(n, [t, &count](int f) {
        ++count;
        return f >= t;
        });

    std::cout << "Found floor "s << found << " after "s << count << " drops"s;
}