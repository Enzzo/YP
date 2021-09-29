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
    if (this->HasValue() && !other.HasValue()) {
        value_->~T();
        is_initialized_ = false;
    }
    else if (!this->HasValue() && other.HasValue()) {
        this->value_ = new(&data_[0]) T(other.Value());
        this->is_initialized_ = true;
    }
    else {

        Optional temp(other.Value());
        std::swap(value_, temp.value_);
        is_initialized_ = true;
    }
}

template<typename T>
Optional<T>::Optional(Optional&& other){
    if (!other.HasValue()) {
        throw BadOptionalAccess();
    }
    Optional temp(std::move(other.Value()));
    std::swap(value_, temp.value_);
    is_initialized_ = true;
}

//----------------------------------assignment----------------------------------
template<typename T>
Optional<T>& Optional<T>::operator=(const T& value) { 
    value_ = new(&data_[0]) T(value);
    is_initialized_ = true;
    return *this;
}

template<typename T>
Optional<T>& Optional<T>::operator=(T&& value) {
    value_ = new(&data_[0]) T(std::move(value));
    is_initialized_ = true;
    return *this;
}

template<typename T>
Optional<T>& Optional<T>::operator=(const Optional& rhs) {
    if (!HasValue() && rhs.HasValue()) {
        value_ = new(&data_[0]) T(rhs.Value());
        is_initialized_ = true;
        return *this;
    }
    else if (HasValue() && !rhs.HasValue()) {
        Reset();
    }
    else {
        *value_ = rhs.Value();
        is_initialized_ = true;
    }
    return *this;
}

template<typename T>
Optional<T>& Optional<T>::operator=(Optional&& rhs) {
    if (!rhs.HasValue()) {
        throw BadOptionalAccess();
    }

    *value_ = std::move(rhs.Value());
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
    /*if (value_ == nullptr) {
        throw BadOptionalAccess();
    }*/
    return *value_;
}

template<typename T>
const T& Optional<T>::Value() const {
    //if (value_ == nullptr) {
    //    throw BadOptionalAccess();
    //}
    return *value_;
}

template<typename T>
void Optional<T>::Reset() {
    is_initialized_ = false;
    value_->~T();
}

//----------------------------------operators----------------------------------
template<typename T>
T& Optional<T>::operator*() {
    if (value_ == nullptr) {
        throw BadOptionalAccess();
    }
    return *value_;
};

template<typename T>
const T& Optional<T>::operator*() const {
    if (value_ == nullptr) {
        throw BadOptionalAccess();
    }
    return *value_;
};

template<typename T>
T* Optional<T>::operator->() {
    if (value_ == nullptr) {
        throw BadOptionalAccess();
    }
    return value_;
};

template<typename T>
const T* Optional<T>::operator->() const {
    if (value_ == nullptr) {
        throw BadOptionalAccess();
    }
    return value_;
};