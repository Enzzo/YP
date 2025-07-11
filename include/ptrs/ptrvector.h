#pragma once

template <typename T>
class PtrVector {
    std::vector<T*> items_;
public:
    PtrVector() = default;

    // ������ ������ ���������� �� ����� �������� �� other
    PtrVector(const PtrVector& other) {
        // ���������� ���������� ����������� ��������������
        items_.reserve(other.GetItems().size());
        
        for (T* e : other.GetItems()){
            items_.push_back(e?new T(*e):nullptr);
        }
    }

    // ���������� ������� ������� � ����, �� ������� ��������� ���������,
    // � ������� items_
    ~PtrVector() {
        // ���������� ���� ����������� ��������������
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

    // ���������� ������ �� ������ ����������
    std::vector<T*>& GetItems() noexcept {
        // ���������� ����� ��������������
        return items_;
    }

    // ���������� ����������� ������ �� ������ ����������
    std::vector<T*> const& GetItems() const noexcept {
        // ���������� ����� ��������������
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