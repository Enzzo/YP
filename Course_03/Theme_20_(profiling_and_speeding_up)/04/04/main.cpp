#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include "log_duration.h"

using namespace std::literals;

std::vector<int> ReverseVector(const std::vector<int>& source_vector) {

    std::vector<int> res;
    for (int i : source_vector) {
        res.insert(res.begin(), i);
    }

    return res;
}

int CountPops(const std::vector<int>& source_vector, int begin, int end) {

    int res = 0;

    for (int i = begin; i < end; ++i) {
        if (source_vector[i]) {
            ++res;
        }
    }

    return res;
}

void FillRandom(std::vector<int>& v, int n) {
    for (int i = 0; i < n; ++i) {
        // получаем случайное число с помощью функции rand.
        // с помощью (rand() % 2) получим целое число в диапазоне 0..1.
        // в C++ имеются более современные генераторы случайных чисел,
        // но в данном уроке не будем их касаться
        v.push_back(rand() % 2);
    }
}

void Operate() {
    LogDuration total("Total");

    std::vector<int> random_bits;
    std::vector<int> reversed_bits;

    // операция << для целых чисел это сдвиг всех бит в двоичной
    // записи числа. Запишем с её помощью число 2 в степени 17 (131072)
    static const int N = 1 << 17;

    {
        LogDuration log("Fill random"s);
        // заполним вектор случайными числами 0 и 1
        FillRandom(random_bits, N);
    }
    
    {
        LogDuration log("Reverse"s);
        // перевернём вектор задом наперёд
        reversed_bits = ReverseVector(random_bits);
    }
    {
    // посчитаем процент единиц на начальных отрезках вектора
        LogDuration log("Counting");
        for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
            double rate = CountPops(reversed_bits, 0, i) * 100. / i;
            std::cout << "After "s << i << " bits we found "s << rate << "% pops"s << std::endl;
        }
    }
}

int main() {
    Operate();
}