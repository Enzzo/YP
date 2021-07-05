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
    
    auto first = container.begin();
    auto last = container.end();
    auto mid = container.begin();
    std::advance(mid, std::distance(container.begin(), container.end()) / 2);

    std::vector<typename Container::value_type> c1(first, mid);
    std::vector<typename Container::value_type> c2(mid, last);

    std::vector<std::mutex> mtx(container.size());
    auto it = mtx.begin();

    std::future<void> f1 = std::async([&it, &container, &result, &predicate] {for (const auto& value : container) {
        
        if (predicate(value)) {            
            result.push_back(value);
        }
    }
    });    
    f1.get();
    //std::copy_if(std::execution::par, container.begin(), container.end(), result.begin(), predicate);
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