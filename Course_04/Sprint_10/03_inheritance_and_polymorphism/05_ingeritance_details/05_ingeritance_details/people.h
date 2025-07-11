#pragma once

#include <string>

class Person;

// Наблюдатель за состоянием человека.
class PersonObserver {
public:
    // Этот метод вызывается, когда меняется состояние удовлетворённости человека
    virtual void OnSatisfactionChanged(Person&, int, int) {}

protected:
    // Класс PersonObserver не предназначен для удаления напрямую
    virtual ~PersonObserver() = default;
};

/*
    Человек.
    При изменении уровня удовлетворённости уведомляет
    связанного с ним наблюдателя
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

    // Увеличивает на 1 количество походов на танцы
    // Увеличивает удовлетворённость на 1
    virtual void Dance() {
        SetSatisfaction(satisfaction_ + sat_ratio);
        ++dance_count_;
    }

    virtual int GetDanceCount() const {        
        return dance_count_;
    }

    // «Привязывает» наблюдателя к человеку. Привязанный наблюдатель
    // уведомляется об изменении уровня удовлетворённости человека
    // Новый наблюдатель заменяет собой ранее привязанного
    // Если передать nullptr в качестве наблюдателя, это эквивалентно отсутствию наблюдателя
    void SetObserver(PersonObserver* observer) {
        observer_ = observer;
    }

    virtual void SetSatRatio(const int r) {
        sat_ratio = r;
    }

    // Прожить день. Реализация в базовом классе ничего не делает
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

// Рабочий.
// День рабочего проходит за работой
class Worker : public Person{
    int work_point_ = 0;

public:
    Worker(const std::string& name, int age) : Person(name, age) {}

    // Рабочий старше 30 лет и младше 40 за танец получает 2 единицы удовлетворённости вместо 1
    void Dance() override {
        if (GetAge() > 30 && GetAge() < 40) {
            Person::SetSatRatio(2);
        }
        Person::Dance();
    }

    // День рабочего проходит за работой
    void LiveADay() override {
        Work();
    }

    // Увеличивает счётчик сделанной работы на 1, уменьшает удовлетворённость на 5
    void Work() {
        SetSatisfaction(GetSatisfaction() - 5);
        ++work_point_;
    }
    
    // Возвращает значение счётчика сделанной работы
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

// Студент.
// День студента проходит за учёбой
class Student : public Person{
    int study_point_ = 0;

public:
    Student(const std::string& name, int age) : Person(name, age) {}

    void Dance() override {
        Person::Dance();
    }

    // День студента проходит за учёбой
    void LiveADay() override {
        Study();
    }

    // Учёба увеличивает уровень знаний на 1, уменьшает уровень удовлетворённости на 3
    void Study() {
        SetSatisfaction(GetSatisfaction() - 3);
        ++study_point_;
    }

    // Возвращает уровень знаний
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