#include <cassert>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

struct Cat {
    Cat(const string& name, int age)
        : name_(name)
        , age_(age)  //
    {
    }
    const string& GetName() const noexcept {
        return name_;
    }
    int GetAge() const noexcept {
        return age_;
    }
    ~Cat() {
    }
    void Speak() const {
        cout << "Meow!"s << endl;
    }

private:
    string name_;
    int age_;
};

// ������� ������ ���������� �����
unique_ptr<Cat> CreateCat(const string& name) {
    return make_unique<Cat>(name, 2);
}

class Witch {
    string name_;
    unique_ptr<Cat> cat_;

public:
    explicit Witch(const string& name)
        : name_(name) {
    }

    const string& GetName() const noexcept {
        return name_;
    }
    void SetCat(unique_ptr<Cat>&& cat) noexcept {
        cat_ = std::move(cat);
    }
    unique_ptr<Cat> ReleaseCat() noexcept {
        return std::move(cat_);
    }

    Witch(Witch&&) = default;
    Witch& operator=(Witch&&) = default;

    Witch(const Witch& other) {
        Witch temp("");
        temp.name_ = other.name_;
        temp.cat_ = std::make_unique<Cat>(*other.cat_);        
        this->Swap(temp);
    }

    Witch& operator=(const Witch& other) {
        Witch temp(other);
        this->Swap(temp);
        return *this;
    }
private:
    void Swap(Witch& other){
        std::swap(this->name_, other.name_);
        std::swap(this->cat_, other.cat_);
    }
};

void Test() {
    // ������� Witch ����� ����������
    {
        Witch witch("Hermione"s);
        auto cat = CreateCat("Crookshanks"s);
        Cat* raw_cat = cat.get();
        assert(raw_cat);
        witch.SetCat(move(cat));

        Witch moved_witch(std::move(witch));
        auto released_cat = moved_witch.ReleaseCat();
        assert(released_cat.get() == raw_cat);  // ��� ������������ �� witch � moved_witch
    }

    // ����� ������������ ������������ �������� ������������
    {
        Witch witch("Hermione"s);
        auto cat = CreateCat("Crookshanks"s);
        Cat* raw_cat = cat.get();
        witch.SetCat(move(cat));

        Witch witch2("Minerva McGonagall");
        witch2 = move(witch);
        auto released_cat = witch.ReleaseCat();
        assert(!released_cat);
        released_cat = witch2.ReleaseCat();
        assert(released_cat.get() == raw_cat);
    }

    // ����� ���������� ���������
    {
        Witch witch("Hermione");
        auto cat = CreateCat("Crookshanks"s);
        witch.SetCat(move(cat));

        Witch witch_copy(witch);
        assert(!cat);
        cat = witch.ReleaseCat();
        assert(cat);  // � ������ ���������� ��� ������ �� �����

        auto cat_copy = witch_copy.ReleaseCat();
        assert(cat_copy != nullptr && cat_copy != cat);
        assert(cat_copy->GetName() == cat->GetName());  // ����� ���������� �������� ����� ����
    }

    // �������� ���������� ������������ ���������
    {
        Witch witch("Hermione"s);
        auto cat = CreateCat("Crookshanks"s);
        witch.SetCat(move(cat));

        Witch witch2("Minerva McGonagall"s);
        witch2 = witch;

        assert(!cat);
        cat = witch.ReleaseCat();
        assert(cat);  // � ������ ���������� ��� ������ �� �����

        auto cat_copy = witch2.ReleaseCat();
        assert(cat_copy != nullptr && cat_copy != cat);
        assert(cat_copy->GetName() == cat->GetName());  // ��� ������������ ������������ ���
    }
}

int main() {
    Test();
}