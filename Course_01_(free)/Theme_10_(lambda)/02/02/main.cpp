#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

void ShowDoc(const std::vector<std::string>&);
void LexSort(std::vector<std::string>&);

int main() {
	int n;
	std::string query;
	std::vector<std::string> document;
	
	std::cin >> n;

	for (size_t i = 0; i < n; ++i) {
		std::cin >> query;
		document.push_back(query);
	}
	LexSort(document);
	ShowDoc(document);

	return 0;
}

void ShowDoc(const std::vector<std::string>& doc) {
		for (const std::string& s : doc)
			std::cout << s << " ";
}

void LexSort(std::vector<std::string>& doc) {
	std::sort(doc.begin(), doc.end(), [](std::string& lhs, std::string& rhs) {
		return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), [](const char lc, const char rc) {
			return std::tolower(lc) < std::tolower(rc);
			});
		});
	
}