#include <iostream>
#include <string>
#include <vector>
#include <set>

std::vector<std::string> SplitIntoWords(const std::string& text) {
    std::vector<std::string> words;

    std::string word;

    for (int i = 0; i < text.size(); ++i) {
        if (text[i] == ' ') {
            words.push_back(word);
            word = "";
        }
        else {
            word += text[i];
        }
    }
    words.push_back(word);

    return words;
}

int main() {
    std::string stop_word;
    std::string query;

    std::getline(std::cin, stop_word);
    std::getline(std::cin, query);
    
    std::vector<std::string> v_forbidden = SplitIntoWords(stop_word);
    std::set<std::string> s_forbidden(v_forbidden.begin(), v_forbidden.end());
    
    for (std::string word : SplitIntoWords(query)) {
        if(s_forbidden.count(word) == 0)
            std::cout << '[' << word << ']' << std::endl;
    }
}