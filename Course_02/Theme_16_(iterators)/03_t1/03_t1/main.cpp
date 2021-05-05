#include "custom_algorithms.h"
#include <iostream>
#include <vector>
#include <string>
#include <set>

int main() {
	std::vector<int> v = { 1, 2, 3, 3, 1, 54};
	std::string s = "hello world";
	FindAndPrint(s, 'o');
	FindAndPrint(v, 3);
	return 0;
}