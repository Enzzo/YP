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
        // �������� ��������� ����� � ������� ������� rand.
        // � C++ ������� ����� ����������� ���������� ��������� �����,
        // �� � ������ ����� �� ����� �� ��������
        v.push_back(rand() % 2);
    }
}

void Operate() {
    LOG_DURATION("Total"s);

    std::vector<int> random_bits;

    // �������� << ��� ����� ����� ��� ����� ���� ��� � ��������
    // ������ �����. ������� � � ������� ����� 2 � ������� 17 (131072)
    static const int N = 1 << 17;

    // �������� ������ ���������� ������� 0 � 1

    {
        LOG_DURATION("Fill random"s);
        FillRandom(random_bits, N);
    }

    // ��������� ������ ����� ������
    std::vector<int> reversed_bits;
    {
        LOG_DURATION("Reverse"s);

        reversed_bits = ReverseVector(random_bits);
    }

    {
        LOG_DURATION("Counting"s);
        // ��������� ������� ������ �� ��������� �������� �������
        for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
            double rate = CountPops(reversed_bits, 0, i) * 100. / i;
            std::cout << "After "s << i << " bits we found "s << rate << "% pops"s << std::endl;
        }
    }
}

int main() {
    Operate();
}