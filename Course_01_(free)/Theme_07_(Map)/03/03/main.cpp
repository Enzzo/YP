#include <iostream>
#include <string>
#include <map>

bool IsAnagram(const std::string&, const std::string&);
std::map<char, int> BuildCharCounter(const std::string&);

int main() {
    int n;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        std::string first_word, second_word;
        std::cin >> first_word >> second_word;

        if (BuildCharCounter(first_word) == BuildCharCounter(second_word)) {
            std::cout << "YES" << std::endl;
        }
        else {
            std::cout << "NO" << std::endl;
        }
    }

    return 0;
}

std::map<char, int> BuildCharCounter(const std::string& str) {
    std::map<char, int> m;
    for (const char ch : str)
        m[ch]++;
    return m;
}