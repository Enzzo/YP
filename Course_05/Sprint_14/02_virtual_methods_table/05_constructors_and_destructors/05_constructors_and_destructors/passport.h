#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std::string_view_literals;

class Passport : public IdentityDocument {
public:
    Passport()
        : expiration_date_(GetExpirationDate())
    {
        parent_.SetVTablePtr(&vtable_);
        std::cout << "Passport::Ctor()"sv << std::endl;
    }

    Passport(const Passport& other)
        : IdentityDocument(other)
        , expiration_date_(other.expiration_date_)
    {
        parent_.SetVTablePtr(&vtable_);
        std::cout << "Passport::CCtor()"sv << std::endl;
    }

    ~Passport() {
        parent_.ResetVTablePtr();
        std::cout << "Passport::Dtor()"sv << std::endl;
    }

    operator const IdentityDocument* () const {
        return &parent_;
    }

    operator IdentityDocument() {
        return { parent_ };
    }

    void PrintID()const {
        std::cout << "Passport::PrintID() : "sv << GetID();
        std::cout << " expiration date : "sv << expiration_date_.tm_mday << "/"sv << expiration_date_.tm_mon << "/"sv
            << expiration_date_.tm_year + 1900 << std::endl;
    }

    /*virtual*/ void PrintVisa(const std::string& country) const {
        auto print_visa_ptr = static_cast<VTable*>(vtable_ptr_)->print_visa;
        (this->*print_visa_ptr)(country);
    }

    void Delete() {
        this->~Passport();
    }

private:
    void PrintVisaImpl(const std::string& country) const {
        std::cout << "Passport::PrintVisa("sv << country << ") : "sv << GetID() << std::endl;
    }

private:
    struct VTable {
        using PrintIDType = void(Passport::*)() const;
        PrintIDType print_id = { &Passport::PrintID };

        using PrintVisaType = void(Passport::*)(const std::string&) const;
        PrintVisaType print_visa = { &Passport::PrintVisaImpl };

        using DeleteType = void(Passport::*)();
        DeleteType del = { &Passport::Delete };
    };

private:
    static VTable vtable_;
    void* vtable_ptr_ = { &vtable_ };

    IdentityDocument parent_;

private:
    const struct tm expiration_date_;

    tm GetExpirationDate() {
        time_t t = time(nullptr);
        #pragma warning(suppress : 4996)
        tm exp_date = *localtime(&t);
        exp_date.tm_year += 10;
        mktime(&exp_date);
        return exp_date;
    }
};

Passport::VTable Passport::vtable_ = {};