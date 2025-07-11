#include <algorithm>
#include <execution>
#include <mutex>
#include <vector>
#include <iostream>
#include <numeric>

using namespace std;


template <typename Container, typename Predicate>
vector<typename Container::value_type> CopyIfUnordered(const Container& container, Predicate predicate) {
    vector<typename Container::value_type> result;
    result.reserve(container.size());
    mutex result_mutex;
    for_each(
        execution::par,
        container.begin(), container.end(),
        [predicate, &result_mutex, &result](const auto& value) {
            if (predicate(value)) {
                typename Container::value_type* destination;
                {
                    lock_guard guard(result_mutex);
                    destination = &result.emplace_back();
                }
                *destination = value;
            }
        }
    );
    return result;
}


int main() {
    vector<int> numbers(1'000);
    iota(numbers.begin(), numbers.end(), 0);

    const vector<int> even_numbers =
        CopyIfUnordered(numbers, [](int number) { return number % 2 == 0; });
    for (const int number : even_numbers) {
        cout << number << " "s;
    }
    cout << endl;
    // ������� ��� ������ ����� �� 0 �� 999
}
