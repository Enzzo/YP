#pragma once

#include <array>
#include <stdexcept>

template <typename T, size_t N>
class StackVector {
    size_t size_;
    size_t capacity_;
    std::array<T, N> array_;

public:
    explicit StackVector(size_t a_size = 0) : size_(a_size), capacity_(N){
        if (size_ > capacity_) {
            throw std::invalid_argument("size exceeds capacity");
        }
    };

    T& operator[](size_t index){
        return array_[index];
    };
    const T& operator[](size_t index) const {
        return array_[index];
    }

    auto begin() {
        return array_.begin();
    }
    auto end() {
        return array_.begin() + size_;
    }
    auto begin() const {
        return array_.cbegin();
    }
    auto end() const {
        return array_.cbegin() + size_;
    }

    size_t Size() const {
        return size_;
    };
    size_t Capacity() const {
        return capacity_;
    };

    void PushBack(const T& value) {
        if (size_ >= capacity_) {
            throw std::overflow_error("size exceeds capacity");
        }
        array_[size_++] = value;
    }
    T PopBack() {
        if (size_ <= 0) {
            throw std::underflow_error("array is empty");
        }
        T value = array_.at(--size_);
        return value;
    }
};
