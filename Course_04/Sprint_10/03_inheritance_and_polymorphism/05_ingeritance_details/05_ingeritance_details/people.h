#pragma once

#include <string>

class Person;

// ����������� �� ���������� ��������.
class PersonObserver {
public:
    // ���� ����� ����������, ����� �������� ��������� ���������������� ��������
    virtual void OnSatisfactionChanged(Person&, int, int) {}

protected:
    // ����� PersonObserver �� ������������ ��� �������� ��������
    virtual ~PersonObserver() = default;
};

/*
    �������.
    ��� ��������� ������ ���������������� ����������
    ���������� � ��� �����������
*/
class Person {
    std::string name_;
    PersonObserver* observer_ = nullptr;
    int satisfaction_ = 100;
    int age_;
    int dance_count_ = 0;
    int sat_ratio = 1;

public:
    Person(const std::string& name, int age) : name_(name), age_(age) {}
    virtual ~Person() = default;

    virtual int GetSatisfaction() const {
        return satisfaction_;
    }

    const std::string& GetName() const {
        return name_;
    }

    int GetAge() const {
        return age_;
    }

    // ����������� �� 1 ���������� ������� �� �����
    // ����������� ���������������� �� 1
    virtual void Dance() {
        SetSatisfaction(satisfaction_ + sat_ratio);
        ++dance_count_;
    }

    virtual int GetDanceCount() const {        
        return dance_count_;
    }

    // ������������ ����������� � ��������. ����������� �����������
    // ������������ �� ��������� ������ ���������������� ��������
    // ����� ����������� �������� ����� ����� ������������
    // ���� �������� nullptr � �������� �����������, ��� ������������ ���������� �����������
    void SetObserver(PersonObserver* observer) {
        observer_ = observer;
    }

    virtual void SetSatRatio(const int r) {
        sat_ratio = r;
    }

    // ������� ����. ���������� � ������� ������ ������ �� ������
    virtual void LiveADay() {}

protected:
    void SetSatisfaction(int satisfaction) {
        int old_satisfaction = satisfaction_;
        satisfaction_ = satisfaction;
        
        if (observer_) {
            observer_->OnSatisfactionChanged(*this, old_satisfaction, satisfaction_);
        }
    }  
};

// �������.
// ���� �������� �������� �� �������
class Worker : public Person{
    int work_point_ = 0;

public:
    Worker(const std::string& name, int age) : Person(name, age) {}

    // ������� ������ 30 ��� � ������ 40 �� ����� �������� 2 ������� ���������������� ������ 1
    void Dance() override {
        if (GetAge() > 30 && GetAge() < 40) {
            Person::SetSatRatio(2);
        }
        Person::Dance();
    }

    // ���� �������� �������� �� �������
    void LiveADay() override {
        Work();
    }

    // ����������� ������� ��������� ������ �� 1, ��������� ���������������� �� 5
    void Work() {
        SetSatisfaction(GetSatisfaction() - 5);
        ++work_point_;
    }
    
    // ���������� �������� �������� ��������� ������
    int GetWorkDone() const {        
        return work_point_;
    }

    int GetSatisfaction() const override {
        return Person::GetSatisfaction();
    }

    int GetDanceCount()const override {
        return Person::GetDanceCount();
    }
};

// �������.
// ���� �������� �������� �� ������
class Student : public Person{
    int study_point_ = 0;

public:
    Student(const std::string& name, int age) : Person(name, age) {}

    void Dance() override {
        Person::Dance();
    }

    // ���� �������� �������� �� ������
    void LiveADay() override {
        Study();
    }

    // ����� ����������� ������� ������ �� 1, ��������� ������� ���������������� �� 3
    void Study() {
        SetSatisfaction(GetSatisfaction() - 3);
        ++study_point_;
    }

    // ���������� ������� ������
    int GetKnowledgeLevel() const {
        return study_point_;
    }

    int GetSatisfaction() const override {
        return Person::GetSatisfaction();
    }

    int GetDanceCount()const override {
        return Person::GetDanceCount();
    }
};