#include <iostream>
#include <numeric>
#include <vector>
#include <future>
#include <execution>

using namespace std::literals;

template <typename Container, typename Predicate>
std::vector<typename Container::value_type> CopyIfUnordered(const Container& container, Predicate predicate) {
    std::atomic_int size = 0;
    std::vector<typename Container::value_type> result;
    result.resize(container.size());

    for_each(
        std::execution::par,
        container.begin(), container.end(),
        [predicate, &size, &result](const auto& value) {
            if (predicate(value)) {
                result[size.fetch_add(1)] = value;
            }
        }
    );
    result.resize(size);
    return result;
}

int main() {
    std::vector<int> numbers(1'000);
    std::iota(numbers.begin(), numbers.end(), 0);

    const std::vector<int> even_numbers =
        CopyIfUnordered(numbers, [](int number) { return number % 2 == 0; });
    for (const int number : even_numbers) {
        std::cout << number << " "s;
    }
    std::cout << std::endl;
    // выведет все чётные числа от 0 до 999
}