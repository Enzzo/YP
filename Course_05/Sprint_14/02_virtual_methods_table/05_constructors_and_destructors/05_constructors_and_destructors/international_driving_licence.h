#pragma once

#include "driving_licence.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class InternationalDrivingLicence {
public:
    InternationalDrivingLicence()
        : driving_lic_()
    {
        InternationalDrivingLicence::SetVTable(this);
        std::cout << "InternationalDrivingLicence::Ctor()"sv << std::endl;
    }

    InternationalDrivingLicence(const InternationalDrivingLicence& other)
        : driving_lic_(other.driving_lic_)
    {
        InternationalDrivingLicence::SetVTable(this);
        std::cout << "InternationalDrivingLicence::CCtor()"sv << std::endl;
    }

    ~InternationalDrivingLicence() {
        std::cout << "InternationalDrivingLicence::Dtor()"sv << std::endl;
        DrivingLicence::SetVTable((DrivingLicence*)this);
    }

    void Delete() {
        GetVtable()->delete_this(this);
    }

    void PrintID() const {
        GetVtable()->print_id(this);
    }

    int GetID() const {
        return driving_lic_.GetID();
    }

    operator DrivingLicence() const {
        return { driving_lic_ };
    }

    using DeleteFunction = void(*)(InternationalDrivingLicence*);
    using PrintIDFunction = void(*)(const InternationalDrivingLicence*);

    struct Vtable {
        DeleteFunction delete_this;
        PrintIDFunction print_id;
    };

    static void SetVTable(InternationalDrivingLicence* obj) {
        *(InternationalDrivingLicence::Vtable**)obj = &InternationalDrivingLicence::VTABLE;
    }

    const Vtable* GetVtable() const {
        return (const InternationalDrivingLicence::Vtable*)driving_lic_.GetVtable();
    }

    Vtable* GetVtable() {
        return (InternationalDrivingLicence::Vtable*)driving_lic_.GetVtable();
    }

    static InternationalDrivingLicence::Vtable VTABLE;

private:
    DrivingLicence driving_lic_;

    static void Delete(InternationalDrivingLicence* obj) {
        delete obj;
    }

    static void PrintID(const InternationalDrivingLicence* obj) {
        std::cout << "InternationalDrivingLicence::PrintID() : "sv << obj->GetID() << std::endl;
    }
};

InternationalDrivingLicence::Vtable InternationalDrivingLicence::VTABLE = { InternationalDrivingLicence::Delete,
                                                                            InternationalDrivingLicence::PrintID };