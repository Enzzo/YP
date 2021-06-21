#pragma once

#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>

#include "array_ptr.h"

class ReserveProxyObj {
    size_t size_;
public:
    ReserveProxyObj(size_t size = 0) : size_(size) {}
    inline const size_t GetSize() const {
        return size_;
    }
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {

    ArrayPtr<Type> items_;
    size_t size_ = 0;
    size_t capacity_ = 0;

public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    explicit SimpleVector(size_t size) : SimpleVector(size, Type()) {}

    SimpleVector(size_t size, const Type& value) :size_(size), capacity_(size) {
        if (size_ > 0) {
            ArrayPtr<Type> temp(size_);
            std::fill(temp.Get(), temp.Get() + size_, value);
            items_.swap(temp);
        }
    }

    SimpleVector(ReserveProxyObj capacity){
        Reserve(capacity.GetSize());
    }

    SimpleVector(std::initializer_list<Type> init) :size_(init.size()), capacity_(init.size()) {
        if (size_ > 0) {
            ArrayPtr<Type> temp(size_);
            std::copy(init.begin(), init.end(), temp.Get());
            items_.swap(temp);
        }
    }

    //v1
    SimpleVector(const SimpleVector& other) {
        SimpleVector<Type> temp(other.GetSize());
        std::copy(other.begin(), other.end(), temp.begin());
        temp.size_ = other.size_;
        temp.capacity_ = other.capacity_;
        swap(temp);
    }
    //v2
    SimpleVector(SimpleVector&& other){
        SimpleVector<Type> temp(other.GetSize());
        std::copy(std::make_move_iterator(other.begin()), std::make_move_iterator(other.end()), temp.begin());
        temp.size_ = std::exchange(other.size_, 0);
        temp.capacity_ = std::exchange(other.capacity_, 0);
        swap(temp);
    }
    //v1
    SimpleVector& operator=(const SimpleVector& rhs) {
        assert(*this != rhs);
        SimpleVector<Type> temp(rhs);
        this->swap(temp);
        return *this;
    }
    //v2
    SimpleVector& operator=(SimpleVector&& rhs) {
        assert(*this != rhs);
        SimpleVector<Type> temp(std::move(rhs));
        this->swap(temp);
        return *this;
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            ArrayPtr<Type> temp(new_capacity);

            if (begin() != nullptr)
                std::copy(items_.Get(), items_.Get() + capacity_, temp.Get());
            std::fill(temp.Get() + capacity_, temp.Get() + new_capacity, Type());
            items_.swap(temp);
            capacity_ = new_capacity;
        }
    }
    //v1
    
    void PushBack(const Type& item) {

        ++size_;
        while (size_ > capacity_) {
            capacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
        }
        ArrayPtr<Type> temp(capacity_);
        
        if (size_ > 1) {
            std::copy(begin(), end(), temp.Get());
        }

        *(temp.Get() + size_-1) = item;
        items_.swap(temp);
    }
    //v2
    void PushBack(Type&& item) {

        ++size_;
        while (size_ > capacity_) {
            capacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
        }
        ArrayPtr<Type> temp(capacity_);

        if (size_ > 1) {
            std::copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), temp.Get());
        }

        *(temp.Get() + size_ - 1) = std::move(item);
        items_.swap(temp);
    }

    //v1
    Iterator Insert(ConstIterator pos, const Type& value) {
        Iterator p = const_cast<Iterator>(pos);
        auto d = std::distance(begin(), p);

        ++size_;
        if(size_ > capacity_) {
            Resize(size_);
        }
        if (begin() == end() - 1) {
            *begin() = value;
            return begin();
        }

        std::copy_backward(begin()+d, end() - 1, end());
        
        *(begin() + d) = value;
        
        return begin() + d;
    }
    //v2
    Iterator Insert(ConstIterator pos, Type&& value) {
        Iterator p = const_cast<Iterator>(pos);
        auto d = std::distance(begin(), p);

        ++size_;
        if (size_ > capacity_) {
            Resize(size_);
        }
        if (begin() == end() - 1) {
            *begin() = std::move(value);
            return begin();
        }

        std::copy_backward(begin() + d, end() - 1, end());

        *(begin() + d) = std::move(value);

        return begin() + d;
    }

    void PopBack() noexcept {
        assert(size_ > 0);
        --size_;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        assert(size_ > 0);        
        --size_;

        Iterator p = const_cast<Iterator>(pos);
        std::copy_backward(p+1, end()+1, end());

        
        return p;
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        items_.swap(other.items_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    size_t GetSize() const noexcept {
        return size_;
    }

    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    Type& operator[](size_t index) noexcept {
        return items_[index];
    }

    const Type& operator[](size_t index) const noexcept {
        return items_[index];
    }

    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("out of range");
        }
        return items_[index];
    }

    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range();
        }
        return items_[index];
    }

    void Clear() noexcept {
        size_ = 0;
    }

    void Resize(size_t new_size) noexcept{
        if (new_size > capacity_) {
            ArrayPtr<Type> temp(new_size);
            
            if(begin() != nullptr)
                std::copy(items_.Get(), items_.Get() + size_, temp.Get());
            std::fill(temp.Get() + size_, temp.Get() + new_size, Type());
            items_.swap(temp);

            capacity_ = (capacity_ == 0) ? new_size : new_size * 2;
        }
        else if (new_size > size_) {
            std::fill(items_.Get() + size_, items_.Get() + new_size, Type());
        }
        size_ = new_size;
    }

    Iterator begin() noexcept {
        return items_.Get();
    }

    Iterator end() noexcept {
        return items_.Get() + size_;
    }

    ConstIterator begin() const noexcept {
        return items_.Get();
    }

    ConstIterator end() const noexcept {
        return items_.Get() + size_;
    }

    ConstIterator cbegin() const noexcept {
        return items_.Get();
    }

    ConstIterator cend() const noexcept {
        return items_.Get() + size_;
    }
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (&lhs == &rhs) || (lhs.GetSize() == rhs.GetSize() && std::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {    
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs >= lhs;
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}