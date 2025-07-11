#include "runtime.h"

#include <cassert>
#include <optional>
#include <sstream>

using namespace std;

namespace runtime {

ObjectHolder::ObjectHolder(std::shared_ptr<Object> data)
    : data_(std::move(data)) {
}

void ObjectHolder::AssertIsValid() const {
    assert(data_ != nullptr);
}

ObjectHolder ObjectHolder::Share(Object& object) {
    // Возвращаем невладеющий shared_ptr (его deleter ничего не делает)
    return ObjectHolder(std::shared_ptr<Object>(&object, [](auto* /*p*/) { /* do nothing */ }));
}

ObjectHolder ObjectHolder::None() {
    return ObjectHolder();
}

Object& ObjectHolder::operator*() const {
    AssertIsValid();
    return *Get();
}

Object* ObjectHolder::operator->() const {
    AssertIsValid();
    return Get();
}

Object* ObjectHolder::Get() const {
    return data_.get();
}

ObjectHolder::operator bool() const {
    return Get() != nullptr;
}

bool IsTrue(const ObjectHolder& object) {
    if (const auto* ptr = object.TryAs<Number>()) {
        return ptr->GetValue() != 0;
    }
    else if (const auto* ptr = object.TryAs<String>()) {
        return !ptr->GetValue().empty();
    }
    else if (const auto* ptr = object.TryAs<Bool>()) {
        return ptr->GetValue() == true;
    }
    else {
        return false;
    }
}

void ClassInstance::Print(std::ostream& os, Context& context) {
    // Заглушка, реализуйте метод самостоятельно
    if (HasMethod("__str__", 0))
        Call("__str__", {}, context).Get()->Print(os, context);
    else
        os << this;
}

bool ClassInstance::HasMethod(const std::string& method, size_t argument_count) const {
     //Заглушка, реализуйте метод самостоятельно
    const Method* str = class_.GetMethod(method);    
    if (str != nullptr) {
        if (str->formal_params.size() == argument_count) {
            return true;
        }
    }
    return false;
}

Closure& ClassInstance::Fields() {
    // Заглушка. Реализуйте метод самостоятельно
    return fields_;
    //throw std::logic_error("Not implemented"s);
}

const Closure& ClassInstance::Fields() const {
    // Заглушка. Реализуйте метод самостоятельно
    return fields_;
    //throw std::logic_error("Not implemented"s);
}

ClassInstance::ClassInstance(const Class& cls) : class_(cls){
    // Реализуйте метод самостоятельно
}

ObjectHolder ClassInstance::Call(const std::string& method,
                                 const std::vector<ObjectHolder>& actual_args,
                                 Context& context) {
    // Заглушка. Реализуйте метод самостоятельно.
    if (HasMethod(method, actual_args.size())) {
        Closure args;
        args["self"s] = ObjectHolder::Share(*this);

        const Method* method_ptr = class_.GetMethod(method);
        for (size_t i = 0; i < actual_args.size(); ++i) {
            args[method_ptr->formal_params[i]] = actual_args[i];
        }
        return method_ptr->body->Execute(args, context);
    }    

    throw std::runtime_error("Not implemented"s);
}

Class::Class(std::string name, std::vector<Method> methods, const Class* parent)
    : name_(std::move(name))
    , methods_(std::move(methods))
{
    parent_ = (parent) ? parent : this;
}

const Method* Class::GetMethod(const std::string& name) const {
    // Заглушка. Реализуйте метод самостоятельно
    for (size_t i = 0; i < methods_.size(); ++i) {
        if (methods_[i].name == name) {
            return &methods_[i];
        }
    }

    const std::vector<Method>& p_methods = parent_->methods_;
    for (size_t i = 0; i < p_methods.size(); ++i) {
        if (p_methods[i].name == name) {
            return &p_methods[i];
        }
    }

    return nullptr;
}

[[nodiscard]] const std::string& Class::GetName() const {
    return name_;
}

void Class::Print(ostream& os, [[maybe_unused]] Context& context) {
    os << "Class " << GetName();
}

void Bool::Print(std::ostream& os, [[maybe_unused]] Context& context) {
    os << (GetValue() ? "True"sv : "False"sv);
}

bool Equal(const ObjectHolder& lhs, const ObjectHolder& rhs, Context& context) {
    // Заглушка. Реализуйте функцию самостоятельно
    if (lhs.TryAs<Number>() && rhs.TryAs<Number>()) {
        return lhs.TryAs<Number>()->GetValue() == rhs.TryAs<Number>()->GetValue();
    }
    else if (lhs.TryAs<String>() && rhs.TryAs<String>()) {
        return lhs.TryAs<String>()->GetValue() == rhs.TryAs<String>()->GetValue();
    }
    else if (lhs.TryAs<Bool>() && rhs.TryAs<Bool>()) {
        return lhs.TryAs<Bool>()->GetValue() == rhs.TryAs<Bool>()->GetValue();
    }
    else if (!lhs && !rhs) {
        return true;
    }
    else if (lhs.TryAs<ClassInstance>() && lhs.TryAs<ClassInstance>()->HasMethod("__eq__"s, 1)) {
        return lhs.TryAs<ClassInstance>()->Call("__eq__"s, {rhs}, context).TryAs<Bool>()->GetValue();
    }
    throw std::runtime_error("Cannot compare objects for equality"s);
}

bool Less(const ObjectHolder& lhs, const ObjectHolder& rhs, Context& context) {
    // Заглушка. Реализуйте функцию самостоятельно
    if (lhs.TryAs<Number>() && rhs.TryAs<Number>()) {
        return lhs.TryAs<Number>()->GetValue() < rhs.TryAs<Number>()->GetValue();
    }
    else if (lhs.TryAs<String>() && rhs.TryAs<String>()) {
        return lhs.TryAs<String>()->GetValue() < rhs.TryAs<String>()->GetValue();
    }
    else if (lhs.TryAs<Bool>() && rhs.TryAs<Bool>()) {
        return lhs.TryAs<Bool>()->GetValue() < rhs.TryAs<Bool>()->GetValue();
    }
    else if (lhs.TryAs<ClassInstance>() && lhs.TryAs<ClassInstance>()->HasMethod("__lt__"s, 1)) {
        return lhs.TryAs<ClassInstance>()->Call("__lt__"s, { rhs }, context).TryAs<Bool>()->GetValue();
    }
    throw std::runtime_error("Cannot compare objects for less"s);
}

bool NotEqual(const ObjectHolder& lhs, const ObjectHolder& rhs, Context& context) {
    // Заглушка. Реализуйте функцию самостоятельно
    return !Equal(lhs, rhs, context);
}

bool Greater(const ObjectHolder& lhs, const ObjectHolder& rhs, Context& context) {
    // Заглушка. Реализуйте функцию самостоятельно
    return !Less(lhs, rhs, context) && !Equal(lhs, rhs, context);
}

bool LessOrEqual(const ObjectHolder& lhs, const ObjectHolder& rhs, Context& context) {
    return !Greater(lhs, rhs, context);
}

bool GreaterOrEqual(const ObjectHolder& lhs, const ObjectHolder& rhs, Context& context) {
    return !Less(lhs, rhs, context);
}

}  // namespace runtime