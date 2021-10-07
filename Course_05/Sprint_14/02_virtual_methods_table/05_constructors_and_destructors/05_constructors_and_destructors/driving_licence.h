#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class DrivingLicence : public IdentityDocument {
public:
    DrivingLicence() {
        parent_.SetVTablePtr(&vtable_);
        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
    }

    DrivingLicence(const DrivingLicence& other)
        : IdentityDocument(other)
    {
        parent_.SetVTablePtr(&vtable_);
        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
    }

    operator const IdentityDocument* () const {
        return &parent_;
    }

    operator IdentityDocument() {
        return { parent_ };
    }

    ~DrivingLicence() {
        parent_.ResetVTablePtr();
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
    }

    void PrintID() const {
        std::cout << "DrivingLicence::PrintID() : "sv << GetID() << std::endl;
    }

    void Delete() {
        this->~DrivingLicence();
    }

private:

    struct VTable {
        using PrintIDType = void(DrivingLicence::*)()const ;
        PrintIDType print_id = { &DrivingLicence::PrintID };

        using DeleteType = void(DrivingLicence::*)();
        DeleteType del = { &DrivingLicence::Delete };
    };

private:
    static VTable vtable_;
    IdentityDocument parent_;

};

DrivingLicence::VTable DrivingLicence::vtable_ = {};