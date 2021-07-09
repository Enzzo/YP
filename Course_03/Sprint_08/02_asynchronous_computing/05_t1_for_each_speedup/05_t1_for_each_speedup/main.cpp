#include <algorithm>
#include <execution>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <future>

template <typename ForwardRange, typename Function>
void ForEach(ForwardRange& range, Function function) {
    static constexpr int PART_COUNT = 8;
    const auto part_length = range.size() / PART_COUNT;
    auto part_begin = range.begin();
    auto part_end = next(part_begin, part_length);

    std::vector<std::future<void>> futures;
    for (int i = 0; i < PART_COUNT; ++i, part_begin = part_end, part_end = (i == PART_COUNT - 1
            ? range.end()
            : next(part_begin, part_length))
        ) {
        futures.push_back(std::async([function, part_begin, part_end] { for_each(part_begin, part_end, function); }));
    }
}

int main() {
    // для итераторов с произвольным доступом тоже должно работать
    std::vector<std::string> strings = { "cat" , "dog", "code"};

    ForEach(strings, [](std::string& s) { reverse(s.begin(), s.end()); });

    for (std::string_view s : strings) { 
        std::cout << s << " ";
    }
    std::cout << std::endl;

    // вывод: tac god edoc

    return 0;
}