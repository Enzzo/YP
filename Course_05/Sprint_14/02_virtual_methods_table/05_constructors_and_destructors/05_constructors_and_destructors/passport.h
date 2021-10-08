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

    void PrintID()const {
        std::cout << "Passport::PrintID() : "sv << GetID();
        std::cout << " expiration date : "sv << expiration_date_.tm_mday << "/"sv << expiration_date_.tm_mon << "/"sv
            << expiration_date_.tm_year + 1900 << std::endl;
    }

    /*virtual*/ void PrintVisa(const std::string& country) const {
        std::cout << "Passport::PrintVisa("sv << country << ") : "sv << GetID() << std::endl;
    }

    void Delete() {
        this->~Passport();
    }

private:
    struct VTable {
        using PrintIDType = void(Passport::*)() const;
        using PrintVisaType = void(Passport::*)(const std::string&) const;
        using DeleteType = void(Passport::*)();

        PrintIDType print_id;
        PrintVisaType print_visa;
        DeleteType del;
    };

private:
    static VTable vtable_;
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

Passport::VTable Passport::vtable_ = {
    &Passport::PrintID,
    &Passport::PrintVisa,
    &Passport::Delete
};