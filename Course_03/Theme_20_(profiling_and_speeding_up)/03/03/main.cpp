#include <cstdlib>
#include <iostream>
#include <vector>
#include <chrono>

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
    std::vector<int> random_bits;

    // операция << для целых чисел это сдвиг всех бит в двоичной
    // записи числа. Запишем с её помощью число 2 в степени 17 (131072)
    static const int N = 1 << 17;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // заполним вектор случайными числами 0 и 1
    FillRandom(random_bits, N);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cerr << "Fill random: "s << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms"s << std::endl;

    begin = std::chrono::steady_clock::now();
    // перевернём вектор задом наперёд
    std::vector<int> reversed_bits = ReverseVector(random_bits);

    end = std::chrono::steady_clock::now();
    std::cerr << "Reverse: "s << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms"s << std::endl;

    begin = std::chrono::steady_clock::now();
    // посчитаем процент единиц на начальных отрезках вектора
    for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
        // чтобы вычислить проценты, мы умножаем на литерал 100. типа double;
        // целочисленное значение функции CountPops при этом автоматически
        // преобразуется к double, как и i
        double rate = CountPops(reversed_bits, 0, i) * 100. / i;
        std::cout << "After "s << i << " bits we found "s << rate << "% pops"s
            << std::endl;
    }
    
    end = std::chrono::steady_clock::now();
    std::cerr << "Counting: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms"s << std::endl;
}

int main() {
    Operate();
}