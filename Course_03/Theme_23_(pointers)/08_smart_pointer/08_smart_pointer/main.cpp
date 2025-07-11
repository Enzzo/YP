#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std::literals;

// ����� ���������, ��������� ��������� ������ ��� ���� ����������.
// �������� ������� T ����� ��� �������, �� ������� ��������� ���������
template <typename T>
class ScopedPtr {
public:
    // ����������� �� ��������� ������ ������� ���������,
    // ��� ��� ���� ptr_ ����� �������� �� ��������� nullptr
    ScopedPtr() = default;

    // ������ ���������, ����������� �� ���������� raw_ptr.
    // raw_ptr ��������� ���� �� ������, ��������� � ���� ��� ������ new,
    // ���� �������� ������� ����������
    // ������������ noexcept ����������, ��� ����� �� ������� ����������
    explicit ScopedPtr(T* raw_ptr) noexcept {
        // ���������� ��������������
        ptr_ = raw_ptr;
    }

    // ������� � ������ ����������� �����������
    ScopedPtr(const ScopedPtr&) = delete;

    // ����������. ������� ������, �� ������� ��������� ����� ���������.
    ~ScopedPtr() {
        // ���������� ���� ����������� ��������������
        delete ptr_;
    }

    // ���������� ���������, ���������� ������ ScopedPtr
    T* GetRawPtr() const noexcept {
        // �������� ��� ������ ��������������
        return ptr_;
    }

    // ���������� �������� ��������, �� ������� ��������� ����� ���������
    // ���������� ������� �������� "������" ��������� � ������������� ���� ptr_ � null
    T* Release() noexcept {
        // ���������� ��������������
        T* rel = ptr_;
        ptr_ = nullptr;
        return rel;
    }

    // �������� ���������� � ���� bool ��������� ������, ��������� �� ����� ���������
    // �� �����-���� ������
    explicit operator bool() const noexcept {
        // ���������� ��������������
        return ptr_ != nullptr;
    }

    // �������� ������������� ���������� ������ �� ������
    // ����������� ���������� std::logic_error, ���� ��������� �������
    T& operator*() const {
        // ���������� ��������������
        if (ptr_ == nullptr) throw std::logic_error("");
        return *ptr_;
    }

    // �������� -> ������ ���������� ��������� �� ������
    // ����������� ���������� std::logic_error, ���� ��������� �������
    T* operator->() const {
        // ���������� ��������������
        if (ptr_ == nullptr) throw std::logic_error("");
        return ptr_;
    }

private:
    T* ptr_ = nullptr;
};

template <typename T>
class PtrVector {
public:
    PtrVector() = default;

    // ������ ������ ���������� �� ����� �������� �� other
    PtrVector(const PtrVector& other) {
        // ���������� ���������� ����������� ��������������
        T* ptr = nullptr;
        items_.reserve(other.GetItems().size());
        
        for (T* e : other.GetItems()) try{
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

    // ���������� ������� ������� � ����, �� ������� ��������� ���������,
    // � ������� items_
    ~PtrVector() {
        // ���������� ���� ����������� ��������������
        for (T* i : items_) {
            delete i;
        }
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

private:
    std::vector<T*> items_;
};

// ��� ������� main ��������� ������ ������ PtrVector
int main() {
    // ��������������� "�����", ����������� ������ � ���� ��������
    struct DeletionSpy {
        explicit DeletionSpy(bool& is_deleted)
            : is_deleted_(is_deleted) {
        }
        ~DeletionSpy() {
            is_deleted_ = true;
        }
        bool& is_deleted_;
    };

    // ��������� �������� ���������
    {
        bool spy1_is_deleted = false;
        DeletionSpy* ptr1 = new DeletionSpy(spy1_is_deleted);
        {
            PtrVector<DeletionSpy> ptr_vector;
            ptr_vector.GetItems().push_back(ptr1);
            assert(!spy1_is_deleted);

            // ����������� ������ �� ptr_vector
            const auto& const_ptr_vector_ref(ptr_vector);
            // � �����������, � ������������� ������ GetItems
            // ������ ������� ������ �� ���� � ��� �� ������
            assert(&const_ptr_vector_ref.GetItems() == &ptr_vector.GetItems());
        }
        // ��� ���������� ptr_vector ������ ������� ��� �������, �� �������
        // ��������� ����������� ������ ���� �����������
        assert(spy1_is_deleted);
    }

    // ��������������� ������, ����������� ������ � ���� �����������
    struct CopyingSpy {
        explicit CopyingSpy(int& copy_count)
            : copy_count_(copy_count) {
        }
        CopyingSpy(const CopyingSpy& rhs)
            : copy_count_(rhs.copy_count_)  //
        {
            ++copy_count_;
        }
        int& copy_count_;
    };

    // ��������� ����������� ��������� ��� ����������� ������� ����������
    {
        // 10 ���������
        std::vector<int> copy_counters(10);

        PtrVector<CopyingSpy> ptr_vector;
        // �������������� ������������ ������ ����������
        for (auto& counter : copy_counters) {
            ptr_vector.GetItems().push_back(new CopyingSpy(counter));
        }
        // ��������� ������� �������� ������� ���������
        ptr_vector.GetItems().push_back(nullptr);

        auto ptr_vector_copy(ptr_vector);
        // ���������� ��������� � ����� ����� ���������� ��������� ������������� �������
        assert(ptr_vector_copy.GetItems().size() == ptr_vector.GetItems().size());

        // ����� ������ ������� ��������� �� ����� �������
        assert(ptr_vector_copy.GetItems() != ptr_vector.GetItems());
        // ��������� ������� ��������� ������� � ��� ����� - ������� ���������
        assert(ptr_vector_copy.GetItems().back() == nullptr);
        // ���������, ��� �������� ���� ����������� (���������� ������ ����������� �������� �����).
        assert(std::all_of(copy_counters.begin(), copy_counters.end(), [](int counter) {
            return counter == 1;
            }));
    }
}