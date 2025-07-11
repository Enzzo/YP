#include "log_duration.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

std::vector<int> ReverseVector(const std::vector<int>& source_vector) {
    std::vector<int> res;

    for (int i : source_vector) {
        res.insert(res.begin(), i);
    }

    return res;
}

std::vector<int> ReverseVector2(const std::vector<int>& source_vector) {
    std::vector<int> res;

    // будем проходить source_vector задом наперёд
    // с помощью обратного итератора
    for (auto iterator = source_vector.rbegin(); iterator != source_vector.rend(); ++iterator) {
        res.push_back(*iterator);
    }

    return res;
}

std::vector<int> ReverseVector3(const std::vector<int>& source_vector) {
    return { source_vector.rbegin(), source_vector.rend() };
}

std::vector<int> ReverseVector4(const std::vector<int>& source_vector) {
    std::vector<int> res(source_vector.size());

    // реализация вашего собственного оборачивания
    for (size_t i = 0; i < source_vector.size(); ++i) {
        res[source_vector.size() - 1 - i] = source_vector[i];
    }

    return res;
}

void Operate() {
    std::vector<int> rand_vector;
    int n;

    std::cin >> n;
    rand_vector.reserve(n);

    for (int i = 0; i < n; ++i) {
        rand_vector.push_back(rand());
    }

    // код измерения тут
    if (n <= 100000) {
        {
            LOG_DURATION("Naive"s);
            ReverseVector(rand_vector);
        }
        {
            LOG_DURATION("Good"s);
            ReverseVector2(rand_vector);
        }
    }
    else {
        {
            LOG_DURATION("Good"s);
            ReverseVector2(rand_vector);
        }
        {
            LOG_DURATION("Best"s);
            ReverseVector3(rand_vector);
        }
        {
            LOG_DURATION("Your"s);
            ReverseVector4(rand_vector);
        }
    }
}

int main() {
    Operate();
    return 0;
}