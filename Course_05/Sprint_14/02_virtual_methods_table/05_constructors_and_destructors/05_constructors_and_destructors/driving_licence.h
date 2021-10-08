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

    ~DrivingLicence() {
        parent_.ResetVTablePtr();
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
    }

    void PrintID() const {
        auto impl = static_cast<VTable*>(vtable_ptr_)->print_id;
        return (this->*impl)();
    }

    void Delete() {
        auto impl = static_cast<VTable*>(vtable_ptr_)->del;
        return (this->*impl)();
    }

private:

    void PrintIDImpl() const {
        std::cout << "DrivingLicence::PrintID() : "sv << GetID() << std::endl;
    }

    void DeleteImpl() {
        this->~DrivingLicence();
    }

    struct VTable {
        using PrintIDType = void(DrivingLicence::*)()const ;
        using DeleteType = void(DrivingLicence::*)();

        PrintIDType print_id;        
        DeleteType del;
    };

private:
    static VTable vtable_;
    void* vtable_ptr_ = &vtable_;
    IdentityDocument parent_;

};

DrivingLicence::VTable DrivingLicence::vtable_ = {
    &DrivingLicence::PrintIDImpl,
    &DrivingLicence::DeleteImpl
};