#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

template <typename Type>
class Stack {
    std::vector<Type> rep;
public:
    void Push(const Type& element) {
        // �������� ����������
        rep.push_back(element);
    }
    void Pop() {
        // �������� ����������
            rep.pop_back();
    }
    const Type& Peek() const {
        // �������� ����������
        return rep.back();
    }
    Type& Peek() {
        // �������� ����������
        return rep.back();
    }
    void Print() const {
        // �������� ����������
        PrintRange(rep.begin(), rep.end());
    }
    uint64_t Size() const {
        // �������� ����������
        return rep.size();
    }
    bool IsEmpty() const {
        // �������� ����������
        return rep.empty();
    }
};

template <typename Type>
class SortedStack {
    Stack<Type> elements_;
public:
    void Push(const Type& element) {
        // �������� ���������� ������
        if (elements_.Size() > 0){
            if (elements_.Peek() < element) {
                Type e = elements_.Peek();
                elements_.Pop();
                Push(element);
                elements_.Push(e);
            }
            else {
                elements_.Push(element);
            }
        }
        else {
            elements_.Push(element);
        }
    }
    void Pop() {
        // �������� ���������� ������
        elements_.Pop();
    }
    const Type& Peek() const {
        return elements_.Peek();
    }
    Type& Peek() {
        return elements_.Peek();
    }
    void Print() const {
        elements_.Print();
    }
    uint64_t Size() const {
        return elements_.Size();
    }
    bool IsEmpty() const {
        return elements_.IsEmpty();
    }
};

template<typename Type>
struct PairWithMin {
    Type element;
    Type minimum;
};

template<typename Type>
std::ostream& operator<<(std::ostream& os, const PairWithMin<Type>& p) {
    os << p.element;
    return os;
}

template <typename Type>
class StackMin {
    Stack<PairWithMin<Type>> elements_;
    Type minimum;
public:
    void Push(const Type& element) {
        // �������� ���������� ������
        if (elements_.Size() == 0 || minimum > element) {
            minimum = element;
        }
        elements_.Push({ element, minimum });
    }
    void Pop() {
        // �������� ���������� ������
        elements_.Pop();
    }
    const Type& Peek() const {
        return elements_.Peek().element;
    }
    Type& Peek() {
        return elements_.Peek().element;
    }
    void Print() const {
        elements_.Print();
    }
    uint64_t Size() const {
        return elements_.Size();
    }
    bool IsEmpty() const {
        return elements_.IsEmpty();
    }
    const Type& PeekMin() const {
        // �������� ���������� ������
        return elements_.Peek().minimum;
    }
    Type& PeekMin() {
        // �������� ���������� ������
        return elements_.Peek().minimum;
    }
};

int main() {
    SortedStack<int> stack;
    std::vector<int> values(5);

    // ��������� ������ ��� ������������ ������ �����
    iota(values.begin(), values.end(), 1);
    // ������������ ��������
    random_shuffle(values.begin(), values.end());

    // ��������� ���� � ���������, ��� ���������� ����������� ����� ������ �������
    for (int i = 0; i < 5; ++i) {
        std::cout << "����������� ������� = " << values[i] << std::endl;
        stack.Push(values[i]);
        stack.Print();
    }
    return 0;
}