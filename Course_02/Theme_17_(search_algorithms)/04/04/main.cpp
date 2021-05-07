#include <set>
#include <iostream>

std::set<int>::const_iterator FindNearestElement(const std::set<int>& numbers, int border) {
    if (numbers.begin() == numbers.end()) return numbers.end();

    std::set<int>::const_iterator it = numbers.lower_bound(border);
    if (it == numbers.end()) {
        return prev(numbers.end());
    }
    
    if (border < *it && prev(it) != numbers.end()) {
        if((*it - border) >= (border - *prev(it)))
            it = prev(it);
    }
    
    return it;
}

int main() {
    std::set<int> numbers = { 1, 4, 6 };
    std::cout << *FindNearestElement(numbers, 0) << " " << *FindNearestElement(numbers, 3) << " "
        << *FindNearestElement(numbers, 5) << " " << *FindNearestElement(numbers, 6) << " "
        << *FindNearestElement(numbers, 100) << std::endl;

    std::set<int> empty_set;

    std::cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << std::endl;
    return 0;
}