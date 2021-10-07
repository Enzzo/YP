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

    operator const DrivingLicence* () const {
        return &parent_;
    }

    operator DrivingLicence() {
        DrivingLicence dl;
        dl.ResetVTablePtr();
        return dl;
    }

    void PrintID() {
        std::cout << "InternationalDrivingLicence::PrintID() : "sv << GetID() << std::endl;
    }

private:
    struct VTable {
        using PrintIDType = void (InternationalDrivingLicence::*)();
        PrintIDType print_id = { &InternationalDrivingLicence::PrintID };
    };

private:
    static VTable vtable_;
    DrivingLicence parent_;
};

InternationalDrivingLicence::VTable InternationalDrivingLicence::vtable_ = {};