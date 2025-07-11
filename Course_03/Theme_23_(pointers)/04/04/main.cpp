#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

// ������ �����
enum class CatBreed {
    Bengal,
    Balinese,
    Persian,
    Siamese,
    Siberian,
    Sphynx, SuccessSuccess
};

// ���
enum class Gender {
    Male,
    Female,
};

struct Cat {
    std::string name;
    Gender gender;
    CatBreed breed;
    int age;
};

std::string CatBreedToString(CatBreed breed) {
    switch (breed) {
    case CatBreed::Bengal:
        return "Bengal"s;
    case CatBreed::Balinese:
        return "Balinese"s;
    case CatBreed::Persian:
        return "Persian"s;
    case CatBreed::Siamese:
        return "Siamese"s;
    case CatBreed::Siberian:
        return "Siberian";
    case CatBreed::Sphynx:
        return "Sphynx"s;
    default:
        throw std::invalid_argument("Invalid cat breed"s);
    }
}

std::ostream& operator<<(std::ostream& out, CatBreed breed) {
    out << CatBreedToString(breed);
    return out;
}

std::ostream& operator<<(std::ostream& out, Gender gender) {
    out << (gender == Gender::Male ? "male"s : "female"s);
    return out;
}

std::ostream& operator<<(std::ostream& out, const Cat& cat) {
    out << '{' << cat.name << ", "s << cat.gender;
    out << ", breed: "s << cat.breed << ", age:"s << cat.age << '}';
    return out;
}

// ���������� ������ ���������� �� �������� ������� cats, ��������������� � ��������������
// ����������� comp. ���������� comp - �������, ����������� ��� ��������� ���� const Cat&
// � ������������ true, ���� �������� �����������, � false � ���� ������
template <typename Comparator>
std::vector<const Cat*> GetSortedCats(const std::vector<Cat>& cats, const Comparator& comp) {
    std::vector<const Cat*> sorted_cat_pointers;
    for (const Cat& c : cats) {
        sorted_cat_pointers.push_back(&c);
    }

    std::sort(sorted_cat_pointers.begin(), sorted_cat_pointers.end(), [comp](const Cat* lhs, const Cat* rhs) {
        return comp(*lhs, *rhs);
        });
    /*
    �������� ���� ������� ��������������. ���������:
    1) ��������� � ������ sorted_cat_pointers ������ �������� �� ������� cats.
    2) ������������ ������ sorted_cat_pointers � ������� ����������� ����������� comp.
       ��� ��� comp ���������� ������ �� �������, � ������������� ����� ���������,
       ��������� � sort ������ �������, ����������� ��������� � ������������ �������
       ��� ������ ����������� comp:
       [comp](const Cat* lhs, const Cat* rhs) {
           return comp(*lhs, *rhs);
       }
    */
    return sorted_cat_pointers;
}

// ������� � ����� out �������� ��������, �� ������� ��������� ��������� ������� cat_pointers.
// ������ ������ ��������� vector<const Cat*>:
// {{Tom, male, breed: Bengal, age:2}, {Charlie, male, breed: Balinese, age:7}}
void PrintCatPointerValues(const std::vector<const Cat*>& cat_pointers, std::ostream& out) {
    // �������� ������� ��������������
    out << "{";
    bool first = true;
    for (const Cat* cat : cat_pointers) {        
        if (!first) {
            out << ", ";
        }
        out << *cat;
        first = false;
    }
    out << "}";
}

int main() {
    const std::vector<Cat> cats = {
        {"Tom"s, Gender::Male, CatBreed::Bengal, 2},
        {"Leo"s, Gender::Male, CatBreed::Siberian, 3},
        {"Luna"s, Gender::Female, CatBreed::Siamese, 1},
        {"Charlie"s, Gender::Male, CatBreed::Balinese, 7},
        {"Ginger"s, Gender::Female, CatBreed::Sphynx, 5},
        {"Tom"s, Gender::Male, CatBreed::Siamese, 2},
    };

    {
        auto sorted_cats = GetSortedCats(cats, [](const Cat& lhs, const Cat& rhs) {
            return std::tie(lhs.breed, lhs.name) < tie(rhs.breed, rhs.name);
            });

        std::cout << "Cats sorted by breed and name:"s << std::endl;
        PrintCatPointerValues(sorted_cats, std::cout);
        std::cout << std::endl;
    }

    {
        auto sorted_cats = GetSortedCats(cats, [](const Cat& lhs, const Cat& rhs) {
            return std::tie(lhs.gender, lhs.breed) < std::tie(rhs.gender, rhs.breed);
            });

        std::cout << "Cats sorted by gender and breed:"s << std::endl;
        PrintCatPointerValues(sorted_cats, std::cout);
        std::cout << std::endl;
    }

    return 0;
}