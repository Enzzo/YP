#pragma once

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value = {};
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node) :node_(node) {}

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }


        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        BasicIterator& operator++() noexcept {
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator<value_type> prev(node_);
            ++(*this);
            return prev;
        }

        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList() = default;

    ~SingleLinkedList() {
        Clear();
    }

    template<typename Iterator>
    SingleLinkedList(Iterator begin, Iterator end) {
        
        assert(size_ == 0 && head_.next_node == nullptr);

        for (Iterator it = begin; it != end; it++) {
            PushBack(*it);
        }

    }

    SingleLinkedList(std::initializer_list<Type> values) : SingleLinkedList(values.begin(), values.end()) {}

    SingleLinkedList(const SingleLinkedList& other) : SingleLinkedList(other.begin(), other.end()) {}

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        SingleLinkedList temp(rhs);
        swap(temp);
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        
        Node* temp_head = other.head_.next_node;
        other.head_.next_node = head_.next_node;
        head_.next_node = temp_head;
        std::swap(this->size_, other.size_);
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        size_++;
    }

    void PushBack(const Type& value) {

        Node* temp = &head_;

        while (temp->next_node != nullptr) {
            temp = temp->next_node;
        }

        temp->next_node = new Node(value, nullptr);
        size_++;
    }

    void Clear() {
        while (head_.next_node != nullptr) {
            Node* temp = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete temp;
            --size_;
        }
    }

    [[nodiscard]] Iterator begin() noexcept {
        BasicIterator<value_type> begin_(head_.next_node);
        return begin_;
    }

    [[nodiscard]] Iterator end() noexcept {
        return end_;
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        BasicIterator<value_type> begin_(head_.next_node);
        return begin_;
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return end_;
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        BasicIterator<value_type> begin_(head_.next_node);
        return begin_;
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return end_;
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return before_begin_;
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return before_begin_;
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return before_begin_;
    }

    /*
     * Вставляет элемент value после элемента, на который указывает pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* node = new Node(value, nullptr);
        Iterator it = begin();
        while (it != pos && it.node_ != nullptr) {
            ++it;
        }
        it.node_ = node;

        return it;
    }

    void PopFront() noexcept {
        if (head_.next_node != nullptr) {
            Node* node = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete node;
            --size_;
        }
    }

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

private:
    Node head_ {};
    size_t size_ = 0;
    Iterator begin_{};
    Iterator before_begin_{ &head_ };
    Iterator end_{};
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs >= lhs;
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}