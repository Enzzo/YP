#pragma once

#include "driving_licence.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class InternationalDrivingLicence : public DrivingLicence {
public:
    InternationalDrivingLicence() {
        parent_.SetVTablePtr(&vtable_);
        std::cout << "InternationalDrivingLicence::Ctor()"sv << std::endl;
    }

    InternationalDrivingLicence(const InternationalDrivingLicence& other)
        : DrivingLicence(other)
    {
        parent_.SetVTablePtr(&vtable_);
        std::cout << "InternationalDrivingLicence::CCtor()"sv << std::endl;
    }

    ~InternationalDrivingLicence() {
        parent_.ResetVTablePtr();
        std::cout << "InternationalDrivingLicence::Dtor()"sv << std::endl;
    }

    void PrintID() {
        std::cout << "InternationalDrivingLicence::PrintID() : "sv << GetID() << std::endl;
    }

    void Delete() {
        this->~InternationalDrivingLicence();
    }

private:
    struct VTable {
        using PrintIDType = void (InternationalDrivingLicence::*)();
        PrintIDType print_id = { &InternationalDrivingLicence::PrintID };

        using DeleteType = void (InternationalDrivingLicence::*)();
        DeleteType del = { &InternationalDrivingLicence::Delete };
    };

private:
    static VTable vtable_;
    DrivingLicence parent_;
};

InternationalDrivingLicence::VTable InternationalDrivingLicence::vtable_ = {
    &InternationalDrivingLicence::PrintID,
    &InternationalDrivingLicence::Delete
};