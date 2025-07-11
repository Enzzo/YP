#pragma once

#include <vector>
#include <exception>

#include "scopedptr.h"
#include "ptrvector.h"

// ��������
class Tentacle {
public:
    explicit Tentacle(int id) noexcept
        : id_(id) {
    }

    int GetId() const noexcept {
        return id_;
    }

    Tentacle* GetLinkedTentacle() const noexcept {
        return linked_tentacle_;
    }
    void LinkTo(Tentacle& tentacle) noexcept {
        linked_tentacle_ = &tentacle;
    }
    void Unlink() noexcept {
        linked_tentacle_ = nullptr;
    }
private:
    int id_ = 0;
    Tentacle* linked_tentacle_ = nullptr;
};

// ��������
class Octopus {
public:
    Octopus()
        : Octopus(8) {
    }
    explicit Octopus(int num_tentacles) {

        Tentacle* t = nullptr;
        for (int i = 1; i <= num_tentacles; ++i) {
            t = new Tentacle(i);
            ScopedPtr<Tentacle> sp(t);      

            std::vector<Tentacle*>& tentacle = tentacles_.GetItems();

            tentacle.push_back(t); // ����� ��������� ���������� bad_alloc

            // �������� ��������� �� ��������, ������� ��� �������� � tentacles_,
            // ����� �� ������� ��� � ����������� catch ��������
            sp.Release();
        }
    }
    explicit Octopus(Octopus& other) {
        Tentacle* t = nullptr;
        for (int i = 0; i < other.GetTentacleCount(); ++i) {
            t = new Tentacle(i+1);
            ScopedPtr<Tentacle> sp(t);

            auto linked = other.GetTentacle(i).GetLinkedTentacle();
            if (linked != nullptr) {
                t->LinkTo(*linked);
            }

            std::vector<Tentacle*>& tentacle = tentacles_.GetItems();

            tentacle.push_back(t); // ����� ��������� ���������� bad_alloc

            // �������� ��������� �� ��������, ������� ��� �������� � tentacles_,
            // ����� �� ������� ��� � ����������� catch ��������
            sp.Release();
        }
    }

    ~Octopus() {
        // �������� ������� ��������� � ������������ ������ (��������),
        // ������� ������ ���� ������� ��� ��� ����������.
        // ���������� - ������ �����, ����� ���������� �� �����.
        Cleanup();
    }

    // ��������� ����� �������� � ���������������,
    // ������ (����������_������� + 1):
    // 1, 2, 3, ...
    // ���������� ������ �� ����������� ��������
    Tentacle& AddTentacle() {
    //  ���������� ���������� �������� ��������������
        std::vector<Tentacle*>* items = &tentacles_.GetItems();

        items->push_back(new Tentacle((items->size() + 1)));
        return *items->at(items->size() - 1);
    }

    int GetTentacleCount() const noexcept {
        return static_cast<int>(tentacles_.GetItems().size());
    }

    const Tentacle& GetTentacle(size_t index) const {
        return *tentacles_.GetItems().at(index);
    }
    Tentacle& GetTentacle(size_t index) {
        return *tentacles_.GetItems().at(index);
    }

    PtrVector<Tentacle>& GetTentacles() {
        return tentacles_;
    }

    Octopus& operator=(Octopus& rhs) {
        GetTentacles() = rhs.GetTentacles();
        return *this;
    }

private:
    void Cleanup() {
        // ������� �������� ��������� �� ������������ ������
        for (Tentacle* t : tentacles_.GetItems()){
            if(t != nullptr)
                delete t;
        }
        // ������� ������ ���������� �� ��������
        tentacles_.GetItems().clear();
    }

    // ������ ������ ��������� �� ��������. ���� ������� ������� ��������� � ����
    PtrVector<Tentacle> tentacles_;
};
