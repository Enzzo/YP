#include <iostream>
#include <chrono>
#include <set>
#include <vector>
#include <algorithm>
#include <string>

const int SAMPLE_COUNT = 5;

std::vector<int> CountNamesLong(const std::set<std::string>&, const std::vector<std::string>&);
std::vector<int> CountNames(const std::set <std::string>&, const std::vector<std::string>&);
int main() {

	std::set<std::string> s;
	std::vector<std::string> v;
	std::string stra = "";
	std::string strb = "";

	for (int j = 0; j < 10000; ++j) {
		s.insert(stra);
		stra += "a";
		if (j % 2 == 0) {
			v.push_back(strb);
			strb += "b";
		}
		else {
			v.push_back(stra);
		}
	}

	std::cout << "Testing slow version" << std::endl;
	for (int i = 0; i < SAMPLE_COUNT; ++i) {
		auto begin = std::chrono::steady_clock::now();
		CountNames(s, v);
		auto end = std::chrono::steady_clock::now();
		std::cout << "Time difference Long = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
	}

	return 0;
}

std::vector<int> CountNamesLong(const std::set<std::string>& storage, const std::vector<std::string>& candidates) {
	std::vector<int> output;
	for (const std::string& name : candidates) {
		output.push_back(std::count(storage.begin(), storage.end(), name));
	}
	return output;
}

std::vector<int> CountNames(const std::set <std::string > & storage, const std::vector<std::string>& candidates) {
	std::vector<int> output;
	for (const std::string& name : storage) {
		output.push_back(storage.count(name));
	}
	return output;
}