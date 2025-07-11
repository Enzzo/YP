#include <iostream>
#include <vector>

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

private:
    std::vector<Type> elements_;
};

int main() {

    Stack<int> stack;
    for (uint32_t i = 0; i < 10; ++i) {
        stack.Push(i);
        stack.Print();
    }
    while (!stack.IsEmpty()) {
        stack.Pop();
        stack.Print();
    }
    return 0;
}