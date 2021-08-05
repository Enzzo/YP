// файл wordstat.cpp

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <unordered_map>
#include <string>
#include <tuple>
#include <vector>
#include <fstream>

#include "log_duration.h"

using namespace std;

vector<pair<string, int>> GetSortedWordCounts(vector<string> words) {
    unordered_map<string, int> counts_map;
    {
        LOG_DURATION("filling");
        for (auto& word : words) {
            ++counts_map[move(word)];
        }
    }
    {
        vector<pair<string, int>> counts_vector(move_iterator(counts_map.begin()), move_iterator(counts_map.end()));
        LOG_DURATION("sorgint");
        sort(counts_vector.begin(), counts_vector.end(), [](const auto& l, const auto& r) {
            return l.second > r.second && l.first.size() == 5;
            });
        return counts_vector;
    }
}

int main() {
    setlocale(LC_ALL, "russian");
    vector<string> words;
    string word;

    std::ifstream input("canterbury.txt");

    while (input >> word) {
        words.push_back(word);
    }

    auto counts_vector = GetSortedWordCounts(move(words));

    cout << "Слово - Количество упоминаний в тексте"s << endl;
    // выводим первые 10 слов
    for (auto [iter, i] = tuple(counts_vector.begin(), 0); i < 10 && iter != counts_vector.end(); ++i, ++iter) {
        cout << iter->first << " - "s << iter->second << endl;
    }
}