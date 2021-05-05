#include "custom_algorithms.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <random>

int main() {

	std::vector<int> v = {9, 2, 10, 3, 1, 6, 8, 4, 5, 7, 9};
	PrintRange(v.begin(), v.end());
	MergeSort(v.begin(), v.end());
	PrintRange(v.begin(), v.end());
	
 	return 0;
}