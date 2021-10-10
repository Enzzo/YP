#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class DrivingLicence {
public:
    DrivingLicence()
        : identity_doc_()
    {
        DrivingLicence::SetVTable(this);
        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
    }

    DrivingLicence(const DrivingLicence& other)
        : identity_doc_(other.identity_doc_)
    {
        DrivingLicence::SetVTable(this);
        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
    }

    ~DrivingLicence() {
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
        /* После вызова деструктора и разрушения объекта наследованного класса,
           от объекта останется только его внутренняя часть - объект базового класса.
           vtpr должен соответствовать этому типу, поэтому указатель переставляется
           на таблицу виртуальных методов базового класса */
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
        /* Этот метод не был виртуальным в базовом классе.
           В данном случае нужно просто вызвать соответствующий метод
           для внутреннего объекта базового класса. */
        return identity_doc_.GetID();
    }

    operator IdentityDocument() {
        /* Оператор преобразования типов превращает объект наследованного типа
           в объект базового типа, просто копируя свой внутренний объект базового типа */
        return { identity_doc_ };
    }

    using DeleteFunction = void(*)(DrivingLicence*);
    using PrintIDFunction = void(*)(const DrivingLicence*);

    struct Vtable {
        DeleteFunction delete_this;
        PrintIDFunction print_id;
    };

    static void SetVTable(DrivingLicence* obj) {
        *(DrivingLicence::Vtable**)obj = &DrivingLicence::VTABLE;
    }

    const Vtable* GetVtable() const {
        return (const DrivingLicence::Vtable*)identity_doc_.GetVtable();
    }

    Vtable* GetVtable() {
        return (DrivingLicence::Vtable*)identity_doc_.GetVtable();
    }

    static DrivingLicence::Vtable VTABLE;

private:
    IdentityDocument identity_doc_;
    static void Delete(DrivingLicence* obj) {
        delete obj;
    }

    static void PrintID(const DrivingLicence* obj) {
        std::cout << "DrivingLicence::PrintID() : "sv << obj->GetID() << std::endl;
    }
};

DrivingLicence::Vtable DrivingLicence::VTABLE = { DrivingLicence::Delete,
                                                  DrivingLicence::PrintID };