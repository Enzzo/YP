#include <stack>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

template <typename Type>
class Queue {
    std::stack<Type> stack1_;
    std::stack<Type> stack2_;
public:
    void Push(const Type& element) {
        stack1_.push(element);
    }
    void Pop() {
        MoveElements();
        stack2_.pop();
    }
    Type& Front() {
        MoveElements();
        return stack2_.top();
    }
    const Type& Front() const {
        MoveElements();
        return stack2_.top();
    }
    uint64_t Size() const {
        return stack1_.size() + stack2_.size();
    }
    bool IsEmpty() const {
        return (stack1_.empty() && stack2_.empty());
    }
private:
    void MoveElements() {
        if (stack2_.empty()) {
            while (!stack1_.empty()) {
                stack2_.push(stack1_.top());
                stack1_.pop();
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "rus");
    Queue<int> queue;
    std::vector<int> values(5);

    // ��������� ������ ��� ������������ �������
    std::iota(values.begin(), values.end(), 1);
    // ������������ ��������
    std::random_shuffle(values.begin(), values.end());

    PrintRange(values.begin(), values.end());

    std::cout << "��������� �������" << std::endl;

    // ��������� ������� � ������� ������� � ������ �������
    for (int i = 0; i < 5; ++i) {
        queue.Push(values[i]);
        std::cout << "����������� ������� " << values[i] << std::endl;
        std::cout << "������ ������� ������� " << queue.Front() << std::endl;
    }

    std::cout << "�������� �������� �� �������" << std::endl;

    // ������� ������� � ������ ������� � ����������� �������� �� ������
    while (!queue.IsEmpty()) {
        // ������� ����� ��������� ��������� �������, � ����� �����������,
        // ��� ��� �������� Front �� ������ ������� �� ����������
        std::cout << "����� �������� ������� " << queue.Front() << std::endl;
        queue.Pop();
    }
    return 0;
}