#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <numeric>
#include <execution>

#include "log_duration.h"

using namespace std::literals;

std::string GenerateQuery(std::mt19937& generator, int max_length, int space_rate) {
    const int length = std::uniform_int_distribution(0, max_length)(generator);
    std::string query(length, ' ');
    for (char& c : query) {
        const int rnd = std::uniform_int_distribution(0, space_rate - 1)(generator);
        if (rnd > 0) {
            c = 'a' + (rnd - 1);
        }
    }
    return query;
}

template <typename Solver>
void Test(std::string_view mark, std::string_view s, Solver solver) {
    int result;
    {
        LOG_DURATION(mark);
        result = solver(s);
    }
    std::cout << result << std::endl;
}

#define TEST(solver) Test(#solver, s, solver)

int CountWords(std::string_view str) {
    // подсчитайте количество слов,
    // игнорируя начальные, конечные
    // и подряд идущие пробелы
    
    if (str.empty()) {
        return 0;
    }

    std::string_view::iterator first = str.begin()+1;
    std::vector<int> v(str.size());

    return std::transform_reduce(std::execution::par, str.begin(), str.end() - 1, first, 0, std::plus<>(), [](const char l, const char r) {
        return isspace(l) && !isspace(r);
        }) +(*str.begin() != ' ');
}

int main() {
    
    std::mt19937 generator;

    //const std::string s = GenerateQuery(generator, 500'000'00, 4);

    //TEST(CountWords);
    
    std::cout << CountWords("  ss"sv) << std::endl;
}