#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std::literals;

// ������� ������� �������� ���������� ����� �������
template <typename It>
void PrintRangeComma(It range_begin, It range_end) {
    if (range_begin == range_end) {
        return;
    }
    for (auto it = range_begin;;) {
        std::cout << *it;
        if (++it == range_end) {
            break;
        }
        std::cout << ", "s;
    }
    std::cout << std::endl;
}

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    // 1. ���� �������� �������� ������ 2 ���������, ������� �� �������
    int range_length = range_end - range_begin;
    if (range_length < 2) {
        return;
    }

    // 2. ������ ������, ���������� ��� �������� �������� ���������
    std::vector<typename RandomIt::value_type> elements(range_begin, range_end);

    // 3. ��������� ������ �� ��� ������ �����
    auto mid = elements.begin() + range_length / 2;

    // 4. �������� ������� MergeSort �� ������ �������� �������
    MergeSort(elements.begin(), mid);
    MergeSort(mid, elements.end());

    // 5. � ������� ��������� merge ������� ��������������� ��������
    // � �������� ��������
    // merge -> http://ru.cppreference.com/w/cpp/algorithm/merge
    //merge(elements.begin(), elements.end(), mid, mid, range_begin);
    merge(elements.begin(), mid, mid, elements.end(), range_begin);
}

int main() {
    std::vector<int> test_vector(10);

    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
    // ��������� �������� ��������������� ������������� ����������
    std::iota(test_vector.begin(), test_vector.end(), 1);

    // random_shuffle   -> https://ru.cppreference.com/w/cpp/algorithm/random_shuffle
    // ������������ �������� � ��������� �������
    std::random_shuffle(test_vector.begin(), test_vector.end());

    // ������� ������ �� ����������
    PrintRangeComma(test_vector.begin(), test_vector.end());

    // ��������� ������ � ������� ���������� ��������
    MergeSort(test_vector.begin(), test_vector.end());

    // ������� ���������
    PrintRangeComma(test_vector.begin(), test_vector.end());

    return 0;
}