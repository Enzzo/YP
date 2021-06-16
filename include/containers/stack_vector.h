#pragma once

#include <array>
#include <stdexcept>

template <typename T, size_t N>
class StackVector {
    size_t size_;
    std::array<T, N> array_;
public:
    explicit StackVector(size_t a_size = 0) : size_(a_sizse) {};

    T& operator[](size_t index){
        return array_[index];
    };
    const T& operator[](size_t index) const {
        return array_[index];
    }

    ? ? ? begin();
    ? ? ? end();
    ? ? ? begin() const;
    ? ? ? end() const;

    size_t Size() const {
        return size_;
    };
    size_t Capacity() const;

    void PushBack(const T& value);
    T PopBack();
};
