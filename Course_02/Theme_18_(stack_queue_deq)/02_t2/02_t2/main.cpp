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
        // напишите реализацию
        rep.push_back(element);
    }
    void Pop() {
        // напишите реализацию
            rep.pop_back();
    }
    const Type& Peek() const {
        // напишите реализацию
        return rep.back();
    }
    Type& Peek() {
        // напишите реализацию
        return rep.back();
    }
    void Print() const {
        // напишите реализацию
        PrintRange(rep.begin(), rep.end());
    }
    uint64_t Size() const {
        // напишите реализацию
        return rep.size();
    }
    bool IsEmpty() const {
        // напишите реализацию
        return rep.empty();
    }
};

template <typename Type>
class SortedStack {
    Stack<Type> elements_;
public:
    void Push(const Type& element) {
        // напишите реализацию метода
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
        // напишите реализацию метода
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
        // напишите реализацию метода
        if (elements_.Size() == 0 || minimum > element) {
            minimum = element;
        }
        elements_.Push({ element, minimum });
    }
    void Pop() {
        // напишите реализацию метода
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
        // напишите реализацию метода
        return elements_.Peek().minimum;
    }
    Type& PeekMin() {
        // напишите реализацию метода
        return elements_.Peek().minimum;
    }
};

int main() {
    SortedStack<int> stack;
    std::vector<int> values(5);

    // заполняем вектор для тестирования нашего стека
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    random_shuffle(values.begin(), values.end());

    // заполняем стек и проверяем, что сортировка сохраняется после каждой вставки
    for (int i = 0; i < 5; ++i) {
        std::cout << "Вставляемый элемент = " << values[i] << std::endl;
        stack.Push(values[i]);
        stack.Print();
    }
    return 0;
}