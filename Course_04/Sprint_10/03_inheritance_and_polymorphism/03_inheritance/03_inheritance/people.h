#pragma once
#include <string>
#include <unordered_set>

enum class ProgrammingLanguage { CPP, JAVA, PYTHON, PHP };

enum class Gender { MALE, FEMALE };

class Person {
    const std::string name_;
    const int age_;
    Gender gender_;

public:
    Person() = delete;
    explicit Person(const std::string& name, int age, Gender gender) : name_(name), age_(age), gender_(gender) {}

    [[nodiscard]] const std::string& GetName()const noexcept;

    [[nodiscard]] const int GetAge() const noexcept;

    [[nodiscard]] const Gender GetGender()const noexcept;
};

// �����������. ����� ��������� ������ ����������������
class Programmer : public Person {
    std::unordered_set<ProgrammingLanguage> languages_;

public:
    Programmer(const std::string& name, int age, Gender gender) : Person(name, age, gender){}

    const std::string& GetName() const;
    int GetAge() const;
    Gender GetGender() const;

    // ��������� ������������ ������ ����� ���������������� language
    // ����� ������ ����� ������ ����������� ����� ��������������� �� ���� �����
    // � �� ����� �����������
    void AddProgrammingLanguage(ProgrammingLanguage language);

    // ��������, ����� �� ����������� ��������������� �� ����� ���������������� language
    // ���������� ����������� �� ������� �� ����� ������ ����������������
    bool CanProgram(ProgrammingLanguage language) const;
};

enum class WorkerSpeciality { BLACKSMITH, CARPENTER, WOOD_CHOPPER, ENGINEER, PLUMBER };

// �������. ������� ����������� ���������������
class Worker : public Person{
    std::unordered_set<WorkerSpeciality> specs_;

public:
    Worker(const std::string& name, int age, Gender gender) : Person(name, age, gender) {};

    const std::string& GetName() const;
    int GetAge() const;
    Gender GetGender() const;

    // ��������� �������� ����������� �������� �� ������������� speciality
    void AddSpeciality(WorkerSpeciality speciality);

    // ��������, ����� �� ������� �������� �� ��������� �������������.
    // ����� ����� ������ �������� ������� �� ������� �������� ���������������
    bool HasSpeciality(WorkerSpeciality speciality) const;
};