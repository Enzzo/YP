#include <iostream>
#include <numeric>
#include <vector>
#include <mutex>
#include <future>
#include <execution>

using namespace std::literals;

template <typename Container, typename Predicate>
std::vector<typename Container::value_type> CopyIfUnordered(const Container& container, Predicate predicate) {
    std::vector<typename Container::value_type> result;
    result.reserve(container.size());
    std::atomic_int size = 0;

    std::for_each(
        std::execution::par,
        container.begin(), container.end(),
        [predicate, &size, &result](const auto& value) {
            if (predicate(value)) {
                if (size.fetch_add(1) == size) {
                    auto& r = result[size - 1];
                    result.resize(size);
                    r = value;
                }
                int x = 2;
            }
        }
    );
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