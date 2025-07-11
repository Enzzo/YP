#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

template<typename It>
void PrintRange(const It& begin, const It& end) {
	It it = begin;
	while (it != end) {
		std::cout << *(it++) << " ";
	}
	std::cout << std::endl;
}

template<typename Container, typename elem>
void FindAndPrint(const Container& container, const elem& e) {
	auto ie = std::find(container.begin(), container.end(), e);
	PrintRange(container.begin(), ie);
	PrintRange(ie, container.end());
}

template<typename It>
auto MakeVector(const It& begin, const It& end) {
	return std::vector(begin, end);
}

template<typename Container, typename It>
void EraseAndPrint(Container& container, const It it) {
	auto e = container.erase(it);
	PrintRange(container.begin(), e);
	PrintRange(e, container.end());
}

template<typename Container>
void EraseAndPrint(Container& container, const int pos) {
	EraseAndPrint(container, container.begin() + pos);
}

template<typename Container>
void EraseAndPrint(Container& container, const int pos, const int b, const int e) {

	auto ib = container.begin();
	container.erase(ib + pos);
	PrintRange(container.begin(), container.end());

	ib = container.begin();
	container.erase(ib + b, ib + e);
	PrintRange(container.begin(), container.end());
}

template<typename InputIt>
void PrintPermutations(InputIt begin, InputIt end) {
	std::reverse(begin, end);
	do {
		PrintRange(begin, end);
	} while (std::prev_permutation(begin, end));
}

template<typename RandomIt>
void MergeSort(RandomIt begin, RandomIt end) {
	if (begin == end)return;
	int distance = std::distance(begin, end);
	RandomIt mid = begin + distance / 2;
	
	if (distance > 1) {
		MergeSort(begin, mid);
		MergeSort(mid, end);
		if (begin != mid && mid != end) {
			std::vector<typename RandomIt::value_type> v;
			std::merge(begin, mid, mid, end, std::back_inserter(v));

			std::copy(v.begin(), v.end(), begin);
		}
	}	
}