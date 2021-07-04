#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include <future>

using namespace std::literals;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        std::cout << *it << " "s;
    }
    std::cout << std::endl;
}

// Ускорьте с помощью параллельности
template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    // 1. Если диапазон содержит меньше 2 элементов, выходим из функции
    int range_length = range_end - range_begin;
    if (range_length < 2) {
        return;
    }

    // 2. Создаём вектор, содержащий все элементы текущего диапазона
    std::vector elements(range_begin, range_end);
    // Тип элементов — typename iterator_traits<RandomIt>::value_type

    // 3. Разбиваем вектор на две равные части
    auto mid = elements.begin() + range_length / 2;

    // 4. Вызываем функцию MergeSort от каждой половины вектора
    auto x = std::async([&elements, &mid] {MergeSort(elements.begin(), mid); });
    auto y = std::async([&elements, &mid] {MergeSort(mid, elements.end()); });
    x.get();
    y.get();

    // 5. С помощью алгоритма merge сливаем отсортированные половины
    // в исходный диапазон
    // merge -> http://ru.cppreference.com/w/cpp/algorithm/merge
    merge(elements.begin(), mid, mid, elements.end(), range_begin);
}

int main() {
    std::mt19937 generator;

    std::vector<int> test_vector(10);

    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
    // Заполняет диапазон последовательно возрастающими значениями
    std::iota(test_vector.begin(), test_vector.end(), 1);

    // shuffle   -> https://ru.cppreference.com/w/cpp/algorithm/random_shuffle
    // Перемешивает элементы в случайном порядке
    shuffle(test_vector.begin(), test_vector.end(), generator);

    // Выводим вектор до сортировки
    PrintRange(test_vector.begin(), test_vector.end());

    // Сортируем вектор с помощью сортировки слиянием
    MergeSort(test_vector.begin(), test_vector.end());

    // Выводим результат
    PrintRange(test_vector.begin(), test_vector.end());

    // Проверяем, можно ли передать указатели
    MergeSort(test_vector.data(), test_vector.data() + test_vector.size());

    return 0;
}