#pragma once

#include <vector>
#include <exception>

#include "scopedptr.h"
#include "ptrvector.h"

// Щупальце
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

// Осьминог
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

            tentacle.push_back(t); // Может выбросить исключение bad_alloc

            // Обнуляем указатель на щупальце, которое уже добавили в tentacles_,
            // чтобы не удалить его в обработчике catch повторно
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

            tentacle.push_back(t); // Может выбросить исключение bad_alloc

            // Обнуляем указатель на щупальце, которое уже добавили в tentacles_,
            // чтобы не удалить его в обработчике catch повторно
            sp.Release();
        }
    }

    ~Octopus() {
        // Осьминог владеет объектами в динамической памяти (щупальца),
        // которые должны быть удалены при его разрушении.
        // Деструктор - лучшее место, чтобы прибраться за собой.
        Cleanup();
    }

    // Добавляет новое щупальце с идентификатором,
    // равным (количество_щупалец + 1):
    // 1, 2, 3, ...
    // Возвращает ссылку на добавленное щупальце
    Tentacle& AddTentacle() {
    //  Реализуйте добавление щупальца самостоятельно
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
        // Удаляем щупальца осьминога из динамической памяти
        for (Tentacle* t : tentacles_.GetItems()){
            if(t != nullptr)
                delete t;
        }
        // Очищаем массив указателей на щупальца
        tentacles_.GetItems().clear();
    }

    // Вектор хранит указатели на щупальца. Сами объекты щупалец находятся в куче
    PtrVector<Tentacle> tentacles_;
};
