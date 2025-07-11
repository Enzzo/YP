#include <iostream>
#include <algorithm>
#include <string>

void PrintSpacesPositions(const std::string& str) {
	for (auto it = std::find(str.begin(), str.end(), ' '); it != str.end(); it = std::find(next(it), str.end(), ' ')) {
		std::cout << std::distance(str.begin(), it) << std::endl;
	}
}

int main() {
	std::string str = "He said: one and one and one is three";
	PrintSpacesPositions(str);
	return 0;
}