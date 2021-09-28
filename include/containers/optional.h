#include <stdexcept>
#include <utility>

// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
    using exception::exception;

    virtual const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    Optional() = default;
    Optional(const T& value);
    Optional(T&& value);
    Optional(const Optional& other);
    Optional(Optional&& other);

    Optional& operator=(const T& value);
    Optional& operator=(T&& rhs);
    Optional& operator=(const Optional& rhs);
    Optional& operator=(Optional&& rhs);

    ~Optional();

    bool HasValue() const;

    // Операторы * и -> не должны делать никаких проверок на пустоту Optional.
    // Эти проверки остаются на совести программиста
    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;

    // Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
    T& Value();
    const T& Value() const;

    void Reset();

private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
    T* value_;
};

//----------------------------------ctor----------------------------------
template<typename T>
Optional<T>::Optional(const T& value) {
    value_ = new(&data_[0]) T(value);
    is_initialized_ = true;
}

template<typename T>
Optional<T>::Optional(T&& value) {
    value_ = new(&data_[0]) T(std::move(value));
    is_initialized_ = true;
}

template<typename T>
Optional<T>::Optional(const Optional& other){
    //assert(this != other)
    Optional temp;

    temp.is_initialized_ = other.is_initialized_;
    temp.value_ = other.value_;
}

template<typename T>
Optional<T>::Optional(Optional&& other) {

}

//----------------------------------assignment----------------------------------
template<typename T>
Optional<T>& Optional<T>::operator=(const T& value) {
    return *this;
}

template<typename T>
Optional<T>& Optional<T>::operator=(T&& value) {
    return *this;
}

template<typename T>
Optional<T>& Optional<T>::operator=(const Optional& rhs) {
    return *this;
}

template<typename T>
Optional<T>& Optional<T>::operator=(Optional&& rhs) {
    return *this;
}

//----------------------------------dtor----------------------------------
template<typename T>
Optional<T>::~Optional() {
    if(value_ != nullptr)
        value_->~T();
}

//----------------------------------methods----------------------------------

template<typename T>
bool Optional<T>::HasValue() const {
    return is_initialized_;
}

template<typename T>
T& Optional<T>::Value() {
    return *value_;
}

template<typename T>
const T& Optional<T>::Value() const {

}

template<typename T>
void Optional<T>::Reset() {
    is_initialized_ = false;
    value_->~T();
}

//----------------------------------operators----------------------------------
template<typename T>
T& Optional<T>::operator*() {
    return *value_;
};

template<typename T>
const T& Optional<T>::operator*() const {
    return *value_;
};

template<typename T>
T* Optional<T>::operator->() {
    return value_;
};

template<typename T>
const T* Optional<T>::operator->() const {
    return value_;
};