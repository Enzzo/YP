#include <set>
#include <iostream>

std::set<int>::const_iterator FindNearestElement(const std::set<int>& numbers, int border) {
    if (numbers.begin() == numbers.end()) return numbers.end();

    std::set<int>::const_iterator it = numbers.lower_bound(border);

    if (*it > border || it == numbers.end()) {
        it = numbers.begin();
        while (*it < border) {
            if (*next(it) > border || next(it) == numbers.end())
                return it;
            it++;
        }
    }
    return it;
}

int main() {
    std::set<int> numbers = { 1, 4, 6 };
    std::cout << *FindNearestElement(numbers, 0) << " " << *FindNearestElement(numbers, 3) << " "
        << *FindNearestElement(numbers, 5) << " " << *FindNearestElement(numbers, 6) << " ";
        //<< *FindNearestElement(numbers, 100) << std::endl;

    std::set<int> empty_set;

    std::cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << std::endl;
    return 0;
}