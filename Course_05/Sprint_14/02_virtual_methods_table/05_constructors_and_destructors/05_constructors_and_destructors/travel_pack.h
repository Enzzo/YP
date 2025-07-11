#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class TravelPack {
public:
    TravelPack()
        : identity_doc_base_()
        , identity_doc1_((IdentityDocument*)(new Passport()))
        , identity_doc2_((IdentityDocument*)(new DrivingLicence()))
    {
        TravelPack::SetVTable(this);
        std::cout << "TravelPack::Ctor()"sv << std::endl;
    }

    TravelPack(const TravelPack& other)
        : identity_doc_base_(other.identity_doc_base_)
        , identity_doc1_((IdentityDocument*)(new Passport(*(Passport*)(other.identity_doc1_))))
        , identity_doc2_((IdentityDocument*)(new DrivingLicence(*(DrivingLicence*)(other.identity_doc2_))))
        , additional_pass_(other.additional_pass_)
        , additional_dr_licence_(other.additional_dr_licence_)
    {
        TravelPack::SetVTable(this);
        std::cout << "TravelPack::CCtor()"sv << std::endl;
    }

    ~TravelPack() {
        identity_doc1_->Delete();
        identity_doc2_->Delete();
        std::cout << "TravelPack::Dtor()"sv << std::endl;
        IdentityDocument::SetVTable((IdentityDocument*)this);
    }

    void Delete() {
        GetVtable()->delete_this(this);
    }

    void PrintID() const {
        GetVtable()->print_id(this);
    }

    void PrintUniqueIDCount() {
        IdentityDocument::PrintUniqueIDCount();
    }

    int GetID() const {
        return identity_doc_base_.GetID();
    }

    operator IdentityDocument() {
        return { identity_doc_base_ };
    }

    using DeleteFunction = void(*)(TravelPack*);
    using PrintIDFunction = void(*)(const TravelPack*);

    struct Vtable {
        DeleteFunction delete_this;
        PrintIDFunction print_id;
    };

    static void SetVTable(TravelPack* obj) {
        *(TravelPack::Vtable**)obj = &TravelPack::VTABLE;
    }

    const Vtable* GetVtable() const {
        return (const TravelPack::Vtable*)identity_doc_base_.GetVtable();
    }

    Vtable* GetVtable() {
        return (TravelPack::Vtable*)identity_doc_base_.GetVtable();
    }

    static TravelPack::Vtable VTABLE;

private:
    IdentityDocument identity_doc_base_;
    IdentityDocument* identity_doc1_;
    IdentityDocument* identity_doc2_;
    Passport additional_pass_;
    DrivingLicence additional_dr_licence_;

    static void Delete(TravelPack* obj) {
        delete obj;
    }

    static void PrintID(const TravelPack* obj) {
        obj->identity_doc1_->PrintID();
        obj->identity_doc2_->PrintID();
        obj->additional_pass_.PrintID();
        obj->additional_dr_licence_.PrintID();
    }
};

TravelPack::Vtable TravelPack::VTABLE = { TravelPack::Delete,
                                          TravelPack::PrintID };