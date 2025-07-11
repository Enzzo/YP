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

    // заполняем вектор для тестирования очереди
    std::iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    std::random_shuffle(values.begin(), values.end());

    PrintRange(values.begin(), values.end());

    std::cout << "Заполняем очередь" << std::endl;

    // заполняем очередь и выводим элемент в начале очереди
    for (int i = 0; i < 5; ++i) {
        queue.Push(values[i]);
        std::cout << "Вставленный элемент " << values[i] << std::endl;
        std::cout << "Первый элемент очереди " << queue.Front() << std::endl;
    }

    std::cout << "Вынимаем элементы из очереди" << std::endl;

    // выводим элемент в начале очереди и вытаскиваем элементы по одному
    while (!queue.IsEmpty()) {
        // сначала будем проверять начальный элемент, а потом вытаскивать,
        // так как операция Front на пустой очереди не определена
        std::cout << "Будем вынимать элемент " << queue.Front() << std::endl;
        queue.Pop();
    }
    return 0;
}