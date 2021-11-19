#pragma once

template <typename T>
class PtrVector {
    std::vector<T*> items_;
public:
    PtrVector() = default;

    // Создаёт вектор указателей на копии объектов из other
    PtrVector(const PtrVector& other) {
        // Реализуйте копирующий конструктор самостоятельно
        items_.reserve(other.GetItems().size());
        
        for (T* e : other.GetItems()){
            items_.push_back(e?new T(*e):nullptr);
        }
    }

    // Деструктор удаляет объекты в куче, на которые ссылаются указатели,
    // в векторе items_
    ~PtrVector() {
        // Реализуйте тело деструктора самостоятельно
        for (T* i : items_) {
            delete i;
        }
    }

    PtrVector& operator=(const PtrVector& rhs) {
        if (this != &rhs) {
            PtrVector temp_ptr(rhs);
            Swap(temp_ptr);
        }
        return *this;
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

    bool operator==(const PtrVector& rhs) {
        if (this->GetItems().size() != *rhs->GetItems().size()) {
            return false;
        }

        for (size_t i = 0; i < *rhs->GetItems().size() - 1; ++i) {
            if (GetItems()[i] != *rhs->GetItems()[i] ||
                &GetItems()[i] != rhs->GetItems()[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const PtrVector& rhs) {
        return !(this == rhs);
    }

private:
    void Swap(PtrVector& p) {
        GetItems().swap(p.GetItems());    
    }
};