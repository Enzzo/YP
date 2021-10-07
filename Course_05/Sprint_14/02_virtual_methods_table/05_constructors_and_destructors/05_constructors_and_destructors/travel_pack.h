#pragma once

#include "identity_document.h"
#include "passport.h"
#include "driving_licence.h"

#include <iostream>
#include <string>

using namespace std::string_view_literals;

class TravelPack : public IdentityDocument {
public:
    TravelPack()
        : identity_doc1_(new Passport())
        , identity_doc2_(new DrivingLicence())
    {
        parent_.SetVTablePtr(&vtable_);
        std::cout << "TravelPack::Ctor()"sv << std::endl;
    }

    TravelPack(const TravelPack& other)
        : IdentityDocument(other)
        , identity_doc1_(new Passport(*reinterpret_cast<Passport*>(other.identity_doc1_)))
        , identity_doc2_(new DrivingLicence(*reinterpret_cast<DrivingLicence*>(other.identity_doc2_)))
        , additional_pass_(other.additional_pass_)
        , additional_dr_licence_(other.additional_dr_licence_)
    {
        parent_.SetVTablePtr(&vtable_);
        std::cout << "TravelPack::CCtor()"sv << std::endl;
    }

    ~TravelPack() {
        parent_.ResetVTablePtr();
        delete identity_doc1_;
        delete identity_doc2_;
        std::cout << "TravelPack::Dtor()"sv << std::endl;
    }

    operator const IdentityDocument* () const {
        return &parent_;
    }

    operator IdentityDocument() {
        return { parent_ };
    }

    void PrintID() const {
        identity_doc1_->PrintID();
        identity_doc2_->PrintID();
        additional_pass_.PrintID();
        additional_dr_licence_.PrintID();
    }

    void Delete() {        
        this->~TravelPack();
    }

private:

    struct VTable {
        using PrintIDType = void (TravelPack::*)() const;
        PrintIDType print_id = { &TravelPack::PrintID };

        using DeleteType = void (TravelPack::*)();
        DeleteType del = { &TravelPack::Delete };
    };

private:
    static VTable vtable_;

    IdentityDocument parent_;

private:
    IdentityDocument* identity_doc1_;
    IdentityDocument* identity_doc2_;
    Passport additional_pass_;
    DrivingLicence additional_dr_licence_;
};

TravelPack::VTable TravelPack::vtable_ = {};