#pragma once

#include <cassert>
#include <initializer_list>
#include <algorithm>

#include "array_ptr.h"

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
        ArrayPtr<Type> temp(size_);
        std::fill(temp.Get(), temp.Get() + size_, value);
        items_.swap(temp);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) :size_(init.size()), capacity_(init.size()) {

        ArrayPtr<Type> temp(size_);
        std::copy(init.begin(), init.end(), temp.Get());
        items_.swap(temp);
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        // Напишите тело самостоятельно
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        // Напишите тело самостоятельно
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        // Напишите тело самостоятельно
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("out of range");
        }
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("out of range");
        }
        return items_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        /*
        if (new_size > size_) {
            ArrayPtr<Type> temp(new_size);
            std::copy(items_.Get(), items_.Get() + size_, temp.Get());
            std::fill(temp.Get() + size_, temp.Get() + new_size, Type());
            items_.swap(temp);
        }
        size_ = new_size;

        while (capacity_ < size_) {
            capacity_ *= 2;
        }
        */
        if (new_size > capacity_) {
            ArrayPtr<Type> temp(new_size);
            std::copy(items_.Get(), items_.Get() + size_, temp.Get());
            std::fill(temp.Get() + size_, temp.Get() + new_size, Type());
            items_.swap(temp);

            while (capacity_ < new_size) {
                capacity_ *= 2;
            }
        }
        else if (new_size > size_) {
            //обнулить диапазон size_ - new_size
            std::fill(items_.Get() + size_, items_.Get() + new_size, Type());
        }
        size_ = new_size;
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        // Напишите тело самостоятельно
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        // Напишите тело самостоятельно
        return items_.Get() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        // Напишите тело самостоятельно
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        // Напишите тело самостоятельно
        return items_.Get() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        // Напишите тело самостоятельно
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        // Напишите тело самостоятельно
        return items_.Get() + size_;
    }
};