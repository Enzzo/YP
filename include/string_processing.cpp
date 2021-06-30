#include "string_processing.h"
#include <execution>

/*
std::vector<std::string_view> SplitIntoWords(const std::string_view& text) {
    std::vector<std::string_view> words;
    std::string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back({ word });
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}*/

std::vector<std::string_view> SplitIntoWords(const std::string_view& text) {
    std::vector<std::string_view> words;
    std::string_view temp = text;
    words.reserve(std::count(text.begin(), text.end(), ' ') + 1);
    
    int start = 0;
    int end = 0;

    while (true) {
        end = temp.find(' ', start);
        if (end == std::string_view::npos) break;        
        
        words.push_back(temp.substr(start, end-start));
        start = end + 1;
    }
    words.push_back(temp.substr(start));
    return words;
}