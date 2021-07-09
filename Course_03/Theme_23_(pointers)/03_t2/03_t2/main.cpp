#include <cassert>
#include <string>

using namespace std::literals;

template <typename T>
bool IsSameObject(const T& l, const T& r) {

    const T* l_ = &l;
    const T* r_ = &r;

    return l_ == r_;
};

int main() {

    const int x = 1;
    const int y = 1;

    IsSameObject(x, x);
    // x и y - разные объекты, хоть и имеющие одинаковое значение
    assert(!IsSameObject(x, y));
    // Оба аргумента - один и тот же объект
    assert(IsSameObject(x, x));

    const std::string name1 = "Harry"s;
    const std::string name1_copy = name1;
    const std::string name2 = "Ronald"s;
    auto name1_ptr = &name1;
    const std::string& name1_ref = name1;

    assert(!IsSameObject(name1, name2));  // Две строки с разными значениями - разные объекты
    assert(!IsSameObject(name1, name1_copy));  // Строка и её копия - разные объекты

    // Оба параметра ссылаются на одну и ту же строку
    assert(IsSameObject(name1, name1));
    assert(IsSameObject(name2, name2));
    assert(IsSameObject(name1_copy, name1_copy));

    // Разыменованный указатель на объект и сам объект - один и тот же объект
    assert(IsSameObject(*name1_ptr, name1));

    // Переменная и ссылка на неё относятся к одному и тому же объекту
    assert(IsSameObject(name1_ref, name1));
    // Ссылка на объект и разыменованный указатель на объект относятся к одному и тому же объекту
    assert(IsSameObject(name1_ref, *name1_ptr));
}