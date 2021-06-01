#pragma once

template <typename T>
class PtrVector {
public:
    PtrVector() = default;

    // Создаёт вектор указателей на копии объектов из other
    PtrVector(const PtrVector& other) {
        // Реализуйте копирующий конструктор самостоятельно
        T* ptr = nullptr;
        items_.reserve(other.GetItems().size());

        for (T* e : other.GetItems()) try {
            if (e == nullptr) {
                ptr = nullptr;
            }
            else {
                ptr = new T(*e);
            }
            items_.push_back(ptr);
            ptr = nullptr;
        }
        catch (std::logic_error) {
            throw std::logic_error("");
        }
        delete ptr;
    }

    // Деструктор удаляет объекты в куче, на которые ссылаются указатели,
    // в векторе items_
    ~PtrVector() {
        // Реализуйте тело деструктора самостоятельно
        for (T* i : items_) {
            delete i;
        }
    }

    // Возвращает ссылку на вектор указателей
    std::vector<T*>& GetItems() noexcept {
        // Реализуйте метод самостоятельно
        return items_;
    }

    // Возвращает константную ссылку на вектор указателей
    std::vector<T*> const& GetItems() const noexcept {
        // Реализуйте метод самостоятельно
        return items_;
    }

private:
    std::vector<T*> items_;
};