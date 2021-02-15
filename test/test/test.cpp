#include <iostream>
#include <string>

int main(){

    std::string query;
    std::string word;

    std::getline(std::cin, query);

    for (size_t i = 0; i < query.size(); i++) {
        if (isspace(query[i])){
            std::cout << "["<<word<<"]" << std::endl;
            word = "";
        }
        else {
            word += query[i];
        }
    }
    std::cout << "[" << word << "]" << std::endl;

    return 0;
}