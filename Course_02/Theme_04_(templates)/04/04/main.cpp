#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

template<typename Documents, typename Term>
std::vector<double> ComputeTfIdfs(const Documents& doc, const Term& req) {
    
    int freq = 0;
    std::vector<double> res;

    for (const auto& d : doc) {
        int count = std::count(d.begin(), d.end(), req);
        if (count) freq++; 

        res.push_back(count / static_cast<double>(d.size()));
    }
    double idf = std::log(doc.size() / static_cast<double>(freq));

    for (size_t i = 0; i < res.size(); i++) {
        res[i] *= idf;
    }
    
    return res;
}

int main() {
    const std::vector<std::vector<std::string>> documents = {
        {"белый", "кот", "и", "модный", "ошейник"},
        {"пушистый", "кот", "пушистый", "хвост"},
        {"ухоженный", "пёс", "выразительные", "глаза"},
    };
    const auto& tf_idfs = ComputeTfIdfs(documents, "кот");
    for (const double tf_idf : tf_idfs) {
        std::cout << tf_idf << " ";
    }
    std::cout << std::endl;
    return 0;
}