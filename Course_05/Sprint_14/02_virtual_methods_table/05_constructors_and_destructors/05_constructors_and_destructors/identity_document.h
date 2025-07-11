#pragma once

#include <iostream>
#include <string>

using namespace std::string_view_literals;

class IdentityDocument {
public:
    IdentityDocument()
        : unique_id_(++unique_id_count_)
    {
        /* ��������� vptr � �������� �������� ���������� �� ����, ��� ������ ��������������.
           ����� ����������� ���, ���������� ����������� ����� */
        IdentityDocument::SetVTable(this);
        std::cout << "IdentityDocument::Ctor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument(const IdentityDocument& other)
        : unique_id_(++unique_id_count_)
    {
        IdentityDocument::SetVTable(this);
        std::cout << "IdentityDocument::CCtor() : "sv << unique_id_ << std::endl;
    }

    ~IdentityDocument() {
        /* � ������ ������ ����� ����������� ���������� ����� ����� ��� �������.
           ����� ����� ������������ ������ �� ���������*/
        --unique_id_count_;
        std::cout << "IdentityDocument::Dtor() : "sv << unique_id_ << std::endl;
    }

    void Delete() {
        /* ���� ������ ��������� �����������, �� ������������ ������� ����������,
           � ���������� ����� ������������ ������ */
        GetVtable()->delete_this(this);
    }

    IdentityDocument& operator=(const IdentityDocument&) = delete;

    void PrintID() const {
        /* ��� ������� ����������� �����. ������������� vtpr */
        GetVtable()->print_id(this);
    }

    static void PrintUniqueIDCount() {
        /* ����� �� ����������� */
        std::cout << "unique_id_count_ : "sv << unique_id_count_ << std::endl;
    }

    int GetID() const {
        return unique_id_;
    }

    using DeleteFunction = void(*)(IdentityDocument*);
    using PrintIDFunction = void(*)(const IdentityDocument*);

    struct Vtable {
        DeleteFunction delete_this;
        PrintIDFunction print_id;
    };

    static void SetVTable(IdentityDocument* obj) {
        /* ��������� vptr �������� � ����� ������ �������,
           �������, ����� �������� � ���� ������,
           ����� ��������������� C-style ��������������� */
           /* obj - ���������. ��� ���� � ������ ������� ���� ����� ��������� - vptr.
              �� ���� ����� �������, ��� obj - ��� ��������� �� ���������. ����� ������,
              ������� ���������. ������ ��� ����, � ����� ����������, ����� ������ �����,
              �� ������� ��������� vptr */
        *(IdentityDocument::Vtable**)obj = &IdentityDocument::VTABLE;
    }

    const Vtable* GetVtable() const {
        return vptr_;
    }

    Vtable* GetVtable() {
        return vptr_;
    }

    static IdentityDocument::Vtable VTABLE;

private:
    IdentityDocument::Vtable* vptr_;
    static int unique_id_count_;
    int unique_id_;

    static void Delete(IdentityDocument* obj) {
        /* � ���� ������ ��� ������� ��������. ������ ������� ���������.
        ����� delete �������� ������� ������� ������������. */
        delete obj;
    }

    static void PrintID(const IdentityDocument* obj) {
        std::cout << "IdentityDocument::PrintID() : "sv << obj->unique_id_ << std::endl;
    }
};

int IdentityDocument::unique_id_count_ = 0;
IdentityDocument::Vtable IdentityDocument::VTABLE = { IdentityDocument::Delete,
                                                      IdentityDocument::PrintID };