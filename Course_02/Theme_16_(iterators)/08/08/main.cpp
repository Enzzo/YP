#include <iostream>
#include <numeric>

#include "custom_algorithms.h"

int main() {
    std::vector<int> permutation(3);
    // ��������� �������� ��������������� ������������� ����������
    std::iota(permutation.begin(), permutation.end(), 1);

    PrintPermutations(permutation.begin(), permutation.end());
	return 0;
}