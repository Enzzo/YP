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
    words.reserve(std::count(text.begin(), text.end(), ' ') + 1);
    
    int pos = 0;

    while (true) {
        pos = text.find(' ', pos + 1);
        if (pos == std::string_view::npos) break;

        std::string_view temp(new char[pos]);// = text.substr(pos);
        //std::move(text.begin(), text.begin() + pos, temp.begin());
        const std::string s = "fff";
        words.push_back(temp);
        //std::string_view& temp = *words.end();
    }
    int x = 0;
    return words;
}