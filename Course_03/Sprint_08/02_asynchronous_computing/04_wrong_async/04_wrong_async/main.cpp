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

// �������� � ������� ��������������
template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    // 1. ���� �������� �������� ������ 2 ���������, ������� �� �������
    int range_length = range_end - range_begin;
    if (range_length < 2) {
        return;
    }

    // 2. ������ ������, ���������� ��� �������� �������� ���������
    std::vector elements(range_begin, range_end);
    // ��� ��������� � typename iterator_traits<RandomIt>::value_type

    // 3. ��������� ������ �� ��� ������ �����
    auto mid = elements.begin() + range_length / 2;

    // 4. �������� ������� MergeSort �� ������ �������� �������
    auto x = std::async([&elements, &mid] {MergeSort(elements.begin(), mid); });
    auto y = std::async([&elements, &mid] {MergeSort(mid, elements.end()); });
    x.get();
    y.get();

    // 5. � ������� ��������� merge ������� ��������������� ��������
    // � �������� ��������
    // merge -> http://ru.cppreference.com/w/cpp/algorithm/merge
    merge(elements.begin(), mid, mid, elements.end(), range_begin);
}

int main() {
    std::mt19937 generator;

    std::vector<int> test_vector(10);

    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
    // ��������� �������� ��������������� ������������� ����������
    std::iota(test_vector.begin(), test_vector.end(), 1);

    // shuffle   -> https://ru.cppreference.com/w/cpp/algorithm/random_shuffle
    // ������������ �������� � ��������� �������
    shuffle(test_vector.begin(), test_vector.end(), generator);

    // ������� ������ �� ����������
    PrintRange(test_vector.begin(), test_vector.end());

    // ��������� ������ � ������� ���������� ��������
    MergeSort(test_vector.begin(), test_vector.end());

    // ������� ���������
    PrintRange(test_vector.begin(), test_vector.end());

    // ���������, ����� �� �������� ���������
    MergeSort(test_vector.data(), test_vector.data() + test_vector.size());

    return 0;
}