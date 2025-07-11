#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Animal {
    std::string name;
    int age;
    double weight;
};

template <typename Container, typename KeyMapper>
void SortBy(Container& container, KeyMapper key_mapper, bool reverse = false) {
    // ������ ����� ����������� ���������
        // � ������������ key_mapper(lhs) < key_mapper(rhs)
    std::sort(container.begin(), container.end(), [key_mapper, reverse](const auto& lhs, const auto& rhs) {
        if (reverse) return key_mapper(rhs) < key_mapper(lhs);
        return key_mapper(lhs) < key_mapper(rhs);
        });
}

void PrintNames(const std::vector<Animal>& animals) {
    for (const Animal& animal : animals) {
        std::cout << animal.name << ' ';
    }
    std::cout << std::endl;
}

int main() {
    std::vector<Animal> animals = {
        {"�����",   10, 5},
        {"�����",   5,  1.5},
        {"�������", 2,  4.5},
        {"�����",   12, 3.1},
    };

    PrintNames(animals);

    SortBy(animals, [](const Animal& animal) { return animal.name; }, true);
    PrintNames(animals);

    SortBy(animals, [](const Animal& animal) { return -animal.weight; });
    PrintNames(animals);
}