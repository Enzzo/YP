#include <iostream>
#include <string>
#include <map>

bool IsAnagram(const std::string&, const std::string&);

int main() {
    int n;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        std::string first_word, second_word;
        std::cin >> first_word >> second_word;

        if (IsAnagram(first_word, second_word)) {
            std::cout << "YES" << std::endl;
        }
        else {
            std::cout << "NO" << std::endl;
        }
    }

    return 0;
}

bool IsAnagram(const std::string& lhs, const std::string& rhs) {
    return false;
}