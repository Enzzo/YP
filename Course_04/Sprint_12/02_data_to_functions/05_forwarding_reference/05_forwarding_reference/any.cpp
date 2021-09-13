#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <memory>

using namespace std;

//----------------AnyStorageBase----------------
class AnyStorageBase {
public:
    virtual void Print(std::ostream&) const = 0;
    virtual ~AnyStorageBase() = default;
};

//------------------AnyStorage------------------
template<typename T>
class AnyStorage : public AnyStorageBase {
    T data_;

public:
    AnyStorage(T&& data) : data_(std::forward<T>(data)) {}

    AnyStorage(const T& data) : data_(data) {}

    void Print(std::ostream& out) const override {
        out << data_;
    }
};

//----------------------Any----------------------
class Any{
    std::unique_ptr<AnyStorageBase> storage_ptr_;

public:
    template<typename T>
    Any(T&& value){
        using Initial = std::remove_reference_t<T&&>;
        storage_ptr_ = std::make_unique<AnyStorage<Initial>>(std::forward<T>(value));
    }

    void Print(std::ostream& out) const {
        storage_ptr_->Print(out);
    }
};

//----------------------Dumper----------------------
class Dumper {
public:
    Dumper() {
        std::cout << "construct"sv << std::endl;
    }
    ~Dumper() {
        std::cout << "destruct"sv << std::endl;
    }
    Dumper(const Dumper&) {
        std::cout << "copy"sv << std::endl;
    }
    Dumper(Dumper&&) noexcept{
        std::cout << "move"sv << std::endl;
    }
    Dumper& operator=(const Dumper&) {
        std::cout << "= copy"sv << std::endl;
        return *this;
    }
    Dumper& operator=(Dumper&&) noexcept{
        std::cout << "= move"sv << std::endl;
        return *this;
    }
};

ostream& operator<<(ostream& out, const Any& arg) {
    arg.Print(out);
    return out;
}

ostream& operator<<(ostream& out, const Dumper&) {
    return out;
}

int main() {
    Any any_int(42);
    Any any_string("abc"s);

    // операция вывода Any в поток определена чуть выше в примере
    cout << any_int << endl << any_string << endl;

    Any any_dumper_temp{ Dumper() };

    Dumper auto_dumper;
    Any any_dumper_auto(auto_dumper);
}