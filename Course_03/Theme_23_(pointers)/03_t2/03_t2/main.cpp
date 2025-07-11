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
    // x � y - ������ �������, ���� � ������� ���������� ��������
    assert(!IsSameObject(x, y));
    // ��� ��������� - ���� � ��� �� ������
    assert(IsSameObject(x, x));

    const std::string name1 = "Harry"s;
    const std::string name1_copy = name1;
    const std::string name2 = "Ronald"s;
    auto name1_ptr = &name1;
    const std::string& name1_ref = name1;

    assert(!IsSameObject(name1, name2));  // ��� ������ � ������� ���������� - ������ �������
    assert(!IsSameObject(name1, name1_copy));  // ������ � � ����� - ������ �������

    // ��� ��������� ��������� �� ���� � �� �� ������
    assert(IsSameObject(name1, name1));
    assert(IsSameObject(name2, name2));
    assert(IsSameObject(name1_copy, name1_copy));

    // �������������� ��������� �� ������ � ��� ������ - ���� � ��� �� ������
    assert(IsSameObject(*name1_ptr, name1));

    // ���������� � ������ �� �� ��������� � ������ � ���� �� �������
    assert(IsSameObject(name1_ref, name1));
    // ������ �� ������ � �������������� ��������� �� ������ ��������� � ������ � ���� �� �������
    assert(IsSameObject(name1_ref, *name1_ptr));
}