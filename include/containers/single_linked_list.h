#pragma once

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() {};// = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

public:
    SingleLinkedList() = default;
    ~SingleLinkedList() {
        Clear();
    }
    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        // Заглушка. Реализуйте метод самостоятельно
        return size_;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        // Заглушка. Реализуйте метод самостоятельно
        return size_ == 0;
    }

    void PushFront(const Type& value) try{
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
    catch (...) {
        throw;
    }

    // Очищает список за время O(N)
    void Clear() noexcept {
        // Реализуйте метод самостоятельно
        while (head_.next_node != nullptr) {
            Node* temp = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete temp;
            --size_;
        }
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
};