#include <cassert>
#include <cstddef>  // нужно для nullptr_t
#include <utility>

using namespace std;

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
    T* ptr_ = nullptr;
public:
    UniquePtr() = default;
    explicit UniquePtr(T* ptr) : ptr_(ptr) {}

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr(UniquePtr&& other);
    UniquePtr& operator=(const UniquePtr&) = delete;
    UniquePtr& operator=(nullptr_t);
    UniquePtr& operator=(UniquePtr&& other);
    ~UniquePtr();

    T& operator*() const;
    T* operator->() const;
    T* Release();
    void Reset(T* ptr);
    void Swap(UniquePtr&);
    T* Get() const;
};

template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) {
    UniquePtr<T> temp;
    temp.ptr_ = std::move(other.Release());
    Swap(temp);
}

template<typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) {
    UniquePtr<T> temp;
    temp.ptr_ = std::move(other.Release());
    Swap(temp);
    return *this;
}

template<typename T>
UniquePtr<T>& UniquePtr<T>::operator=(nullptr_t) {
    UniquePtr<T> temp(nullptr);
    Swap(temp);
    return *this;
}

template<typename T>
UniquePtr<T>::~UniquePtr() {
    if (ptr_) {
        delete(ptr_);
    }
}

template<typename T>
T& UniquePtr<T>::operator*() const {
    return *ptr_;
}

template<typename T>
T* UniquePtr<T>::operator->()const {
    return ptr_;
}

template<typename T>
T* UniquePtr<T>::Release() {
    T* out = ptr_;
    ptr_ = nullptr;
    return out;    
}

template<typename T>
void UniquePtr<T>::Reset(T* ptr) {
    if (ptr_) {
        delete ptr_;
    }
    ptr_ = ptr;
}

template<typename T>
void UniquePtr<T>::Swap(UniquePtr& other) {
    T* temp;
    temp = other.ptr_;
    other.ptr_ = ptr_;
    ptr_ = temp;
}

template<typename T>
T* UniquePtr<T>::Get() const {
    return ptr_;
}

struct Item {
    static int counter;
    int value;
    Item(int v = 0)
        : value(v)
    {
        ++counter;
    }
    Item(const Item& other)
        : value(other.value)
    {
        ++counter;
    }
    ~Item() {
        --counter;
    }
};

int Item::counter = 0;

void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        ptr.Reset(new Item);
        assert(Item::counter == 1);
    }
    assert(Item::counter == 0);

    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        auto rawPtr = ptr.Release();
        assert(Item::counter == 1);

        delete rawPtr;
        assert(Item::counter == 0);
    }
    assert(Item::counter == 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    assert(ptr.Get()->value == 42);
    assert((*ptr).value == 42);
    assert(ptr->value == 42);
}

int main() {
    TestLifetime();
    TestGetters();
}