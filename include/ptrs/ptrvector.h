#pragma once

template <typename T>
class PtrVector {
public:
    PtrVector() = default;

    // Создаёт вектор указателей на копии объектов из other
    PtrVector(const PtrVector& other) {
        // Реализуйте копирующий конструктор самостоятельно
        //T* ptr = nullptr;
        items_.reserve(other.GetItems().size());

        for (T* other_item : other.GetItems()) {
            items_.push_back(other_item == nullptr ? other_item : new T(*other_item));
            /*
            if (e == nullptr) {
                ptr = nullptr;
            }
            else {
                //Выделяем новую память под существуещее значение
                ptr = new T(*e);
            }
            items_.push_back(ptr);
            ptr = nullptr;*/
        }
        //delete ptr;
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