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
    T& operator*()&;
    const T& operator*() const&;
    T* operator->();
    const T* operator->() const;

    T&& operator*()&&;
    T&& Value()&&;

    // Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
    T& Value()&;
    const T& Value() const&;

    void Reset();

    template<typename... Args>
    void Emplace(Args&&...);

private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
    T* value_ = nullptr;
};

//----------------------------------ctor----------------------------------
template<typename T>
Optional<T>::Optional(const T& value) {
    value_ = new(&data_[0]) T(value);
    is_initialized_ = true;
}

template<typename T>
Optional<T>::Optional(T&& value) {
    value_ = new(&data_[0]) T(std::forward<T>(value));
    is_initialized_ = true;
}

template<typename T>
Optional<T>::Optional(const Optional& other){
    if (HasValue() && !other.HasValue()) {
        Reset();
    }
    else if (!this->HasValue() && other.HasValue()) {
        value_ = new(&data_[0]) T(other.Value());
        is_initialized_ = true;
    }
    else if (HasValue() && other.HasValue()) {
        *value_ = other.Value();
        is_initialized_ = true;
    }
}

template<typename T>
Optional<T>::Optional(Optional&& other) {
    if (this->HasValue() && !other.HasValue()) {
        Reset();
    }
    else if (!HasValue() && other.HasValue()) {
        value_ = new(&data_[0]) T(std::forward<T>(other.Value()));
        is_initialized_ = true;
    }
    else if(HasValue() && other.HasValue()){        
        *value_ = std::forward<T>(other.Value());
        is_initialized_ = true;
    }
}

//----------------------------------assignment----------------------------------
template<typename T>
Optional<T>& Optional<T>::operator=(const T& value) {
    if (!HasValue()) {
        value_ = new(&data_[0]) T(value);
        is_initialized_ = true;
    }
    else {
        *value_ = value;
    }
    return *this;
}

template<typename T>
Optional<T>& Optional<T>::operator=(T&& value) {
    if (!HasValue()) {
        value_ = new(&data_[0]) T(std::forward<T>(value));
        is_initialized_ = true;
    }
    else {
        *value_ = std::forward<T>(value);
    }
    return *this;
}

template<typename T>
Optional<T>& Optional<T>::operator=(const Optional& rhs)  {
    if (HasValue() && !rhs.HasValue()) {
        Reset();
    }
    else if (!HasValue() && rhs.HasValue()) {
        value_ = new(&data_[0]) T(rhs.Value());
        is_initialized_ = true;
    }    
    else if (HasValue() && rhs.HasValue()) {
        *value_ = rhs.Value();
        is_initialized_ = true;
    }
    return *this;
}

template<typename T>
Optional<T>& Optional<T>::operator=(Optional&& rhs) {
    if (HasValue() && !rhs.HasValue()) {
        Reset();
    }
    else if (!HasValue() && rhs.HasValue()) {
        value_ = new(&data_[0]) T(std::forward<T>(rhs.Value()));
        is_initialized_ = true;
    }
    else if (HasValue() && rhs.HasValue()) {
        *value_ = std::forward<T>(rhs.Value());
        is_initialized_ = true;
    }
    return *this;
}

//----------------------------------dtor----------------------------------
template<typename T>
Optional<T>::~Optional() {
    if (is_initialized_) {
        value_->~T();
    }
}

//----------------------------------methods----------------------------------

template<typename T>
bool Optional<T>::HasValue() const {
    return is_initialized_;
}

template<typename T>
T& Optional<T>::Value()& {
    if (!is_initialized_) {
        throw BadOptionalAccess();
    }
    return *value_;
}

template<typename T>
T&& Optional<T>::Value()&& {
    if (!is_initialized_) {
        throw BadOptionalAccess();
    }
    return std::move(*value_);
}

template<typename T>
const T& Optional<T>::Value() const& {
    if (!is_initialized_) {
        throw BadOptionalAccess();
    }
    return *value_;
}

template<typename T>
void Optional<T>::Reset() {
    if (is_initialized_) {
        is_initialized_ = false;
        value_->~T();
    }
}

template<typename T>
template<typename... Args>
void Optional<T>::Emplace(Args&&... args) {
    if (is_initialized_) {
        Reset();
    }
    value_ = new(&data_[0]) T(std::forward<Args>(args)...);
    is_initialized_ = true;
}

//----------------------------------operators----------------------------------
template<typename T>
T& Optional<T>::operator*()& {
    return Value();
}

template<typename T>
const T& Optional<T>::operator*() const& {
    return Value();
}

template<typename T>
T&& Optional<T>::operator*()&& {
    return std::move(Value());
}

template<typename T>
T* Optional<T>::operator->() {
    return &Value();
}

template<typename T>
const T* Optional<T>::operator->() const {
    return &Value();
}