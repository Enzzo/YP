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

    std::string_view::iterator start = text.begin();
    std::string_view::iterator end = start;

    std::string temp; 

    while (start != text.end()) {
        end = std::find(start, text.end(), ' ');
        std::string temp(start, end);
        words.push_back(std::move(temp));

        if(end != text.end())
            while (isspace(*end))
                end++;
        start = end;
    }

    return words;
}