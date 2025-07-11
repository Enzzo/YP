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
        // �������� ��������� ����� � ������� ������� rand.
        // � ������� (rand() % 2) ������� ����� ����� � ��������� 0..1.
        // � C++ ������� ����� ����������� ���������� ��������� �����,
        // �� � ������ ����� �� ����� �� ��������
        v.push_back(rand() % 2);
    }
}

void Operate() {
    std::vector<int> random_bits;

    // �������� << ��� ����� ����� ��� ����� ���� ��� � ��������
    // ������ �����. ������� � � ������� ����� 2 � ������� 17 (131072)
    static const int N = 1 << 17;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // �������� ������ ���������� ������� 0 � 1
    FillRandom(random_bits, N);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cerr << "Fill random: "s << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms"s << std::endl;

    begin = std::chrono::steady_clock::now();
    // ��������� ������ ����� ������
    std::vector<int> reversed_bits = ReverseVector(random_bits);

    end = std::chrono::steady_clock::now();
    std::cerr << "Reverse: "s << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms"s << std::endl;

    begin = std::chrono::steady_clock::now();
    // ��������� ������� ������ �� ��������� �������� �������
    for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
        // ����� ��������� ��������, �� �������� �� ������� 100. ���� double;
        // ������������� �������� ������� CountPops ��� ���� �������������
        // ������������� � double, ��� � i
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