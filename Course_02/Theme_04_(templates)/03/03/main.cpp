#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Animal {
    std::string first;
    int second;
};

bool operator<(const Animal& l, const Animal& r) {
    if (l.first < r.first) return true;
    return (l.first == r.first && l.second < r.second);
}

template <typename Term>
std::map<Term, int> ComputeTermFreqs(const std::vector<Term>& terms) {
    std::map<Term, int> term_freqs;
    for (const Term& term : terms)
        ++term_freqs[term];
    return term_freqs;
}

template <typename T>
T FindMaxFreqAnimal(const std::vector<T>& animals) {
    
    // верните элемент с максимальной частотой
    std::map<T,int> map_animals = ComputeTermFreqs(animals);

    T res = { "", 0 };
    
    int count = 0;
    for (const auto& [animal, freq] : map_animals) {
        if (freq > count){
            res = animal;
            count = freq;
        }
    }
    return res;
}

pair<string, int> FindMaxFreqAnimal(const vector<pair<string, int>>& animals) {
    // верните животного с максимальной частотой
    int max_frequency = 0;
    auto animal_freq = ComputeTermFreqs(animals);//
    pair<string, int> our_animal;
   
    for (auto& [animal, frequency] : animal_freq) {
        
        if (frequency > max_frequency) {
            max_frequency = animal.second;
            our_animal = animal;
        }
    }
    return our_animal;
}

int main() {
    const std::vector<std::pair<std::string, int>> animals = {
        {"Murka", 5},
        {"Belka", 6},
        {"Murka", 7},
        {"Murka", 5},
        {"Belka", 6},
    };

    const std::vector<Animal> v_animals = {

        {"Murka", 5},
        {"Belka", 6},
        {"Murka", 7},
        {"Murka", 5},
        {"Belka", 6},
    };

    const std::pair<std::string, int> max_freq_animal = FindMaxFreqAnimal(animals);
    std::cout << max_freq_animal.first <<" "<< max_freq_animal.second << std::endl;

    const Animal animal = FindMaxFreqAnimal(v_animals);
    std::cout << animal.first << " " <<animal.second << std::endl;
}