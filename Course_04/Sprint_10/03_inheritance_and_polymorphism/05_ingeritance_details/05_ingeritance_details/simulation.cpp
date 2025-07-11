#include "people.h"
#include "supervisor.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

using namespace std;

// ��������� ����� ������ ����� � ������� num_steps ����
void RunSimulation(vector<unique_ptr<Person>>& people, int num_days) {
    for (int i = 0; i < num_days; ++i) {
        for (auto& person : people) {
            person->LiveADay();
        }
    }
}

void Tests() {
    struct TestObserver : public PersonObserver {
        Person* person = nullptr;
        optional<int> old_satisfaction;
        optional<int> new_satisfaction;

        void OnSatisfactionChanged(Person& p, int old_value, int new_value) override {
            this->person = &p;
            this->old_satisfaction = old_value;
            this->new_satisfaction = new_value;
        }
    };

    {
        Person p{ "Ivan"s, 30 };
        assert(p.GetDanceCount() == 0);
        const int old_satisfaction = p.GetSatisfaction();
        assert(old_satisfaction == 100);
        p.Dance();
        assert(p.GetDanceCount() == 1);
        assert(p.GetSatisfaction() == old_satisfaction + 1);

        TestObserver o;
        p.SetObserver(&o);
        assert(o.person == nullptr);

        p.Dance();
        assert(o.person == &p);
        assert(o.old_satisfaction == old_satisfaction + 1);
        assert(o.new_satisfaction == old_satisfaction + 2);
    }

    {
        Worker w{ "Ivan"s, 35 };
        const int old_satisfaction = w.GetSatisfaction();
        assert(w.GetWorkDone() == 0);
        w.Work();
        assert(w.GetSatisfaction() == old_satisfaction - 5);
        assert(w.GetWorkDone() == 1);

        // �������, ��� ������� �������� ���� ���� �� ������
        w.LiveADay();
        assert(w.GetSatisfaction() == old_satisfaction - 10);
        assert(w.GetWorkDone() == 2);

        assert(w.GetDanceCount() == 0);
        w.Dance();
        assert(w.GetDanceCount() == 1);
        assert(w.GetSatisfaction() == old_satisfaction - 10 + 2);
    }

    {
        Student s{ "Ivan"s, 18 };
        const int old_satisfaction = s.GetSatisfaction();
        assert(s.GetKnowledgeLevel() == 0);
        s.Study();
        assert(s.GetSatisfaction() == old_satisfaction - 3);
        assert(s.GetKnowledgeLevel() == 1);

        // �������, ��� ������� �������� ���� ���� �� �����
        s.LiveADay();
        assert(s.GetSatisfaction() == old_satisfaction - 6);
        assert(s.GetKnowledgeLevel() == 2);

        assert(s.GetDanceCount() == 0);
        s.Dance();
        assert(s.GetDanceCount() == 1);
        assert(s.GetSatisfaction() == old_satisfaction - 6 + 1);
    }

    {
        Worker w("Ivan"s, 35);

        SatisfactionSupervisor sup{ 95, 97 };
        w.SetObserver(&sup);

        // ����� ������� ������� � ������ ������� ������� �� ��������� ���� 95
        w.Work();
        assert(w.GetSatisfaction() == 95);
        // ������� ����������� �� ������� �������� ����������
        assert(w.GetDanceCount() == 0);

        // ����� ������� ������� � ������ ������� ������� ������ ����� 90
        w.Work();
        // ����� �������������� ������� ������ �������,
        // ����������� ������� �������� ����������
        // 35-������ ������� �� 4 ����� �������� ���� ������� ����������������
        // �� ������� �� ���� 97 (�� 2 ������ �� �����)
        assert(w.GetDanceCount() == 4);
        assert(w.GetSatisfaction() == 98);
    }

    // �������� ������������ ���������� ������� Person
    {
        // �����, ������������ � ���� ����������
        struct DestructivePerson : public Person {
            explicit DestructivePerson(bool& destruction_flag)
                : Person("test"s, 20)
                , destruction_flag_(destruction_flag) {
                destruction_flag_ = false;
            }

            ~DestructivePerson() {
                destruction_flag_ = true;
            }

            bool& destruction_flag_;
        };

        bool person_destroyed = false;
        unique_ptr<Person> p = make_unique<DestructivePerson>(person_destroyed);
        assert(!person_destroyed);
        p.reset();
        assert(person_destroyed);
    }
}

int main() {
    Tests();

    vector<unique_ptr<Person>> people;
    people.emplace_back(make_unique<Worker>("Mario"s, 35));
    people.emplace_back(make_unique<Student>("Harry Potter"s, 18));

    //// ��������� �����������, ������� ������, ����� ������� ���������������� ��� � �������� �� 90 �� 100
    SatisfactionSupervisor supervisor(90, 100);
    for (auto& person : people) {
        person->SetObserver(&supervisor);
    }

    RunSimulation(people, 10);

    for (auto& person : people) {
        cout << person->GetName() << "' satisfaction: "s << person->GetSatisfaction() << ", dance count: "s
            << person->GetDanceCount() << endl;
    }

    // ���������� �����������
    for (auto& person : people) {
        person->SetObserver(nullptr);
    }
}