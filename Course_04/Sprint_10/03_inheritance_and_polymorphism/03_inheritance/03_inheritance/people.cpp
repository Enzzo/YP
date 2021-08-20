#include "people.h"

#include <stdexcept>

using namespace std;

[[nodiscard]] const std::string& Person::GetName()const noexcept {
    return name_;
}

[[nodiscard]] const int Person::GetAge() const noexcept {
    return age_;
}

[[nodiscard]] const Gender Person::GetGender()const noexcept {
    return gender_;
}

const string& Programmer::GetName() const {
    return Person::GetName();
}

int Programmer::GetAge() const {
    return Person::GetAge();
}

Gender Programmer::GetGender() const {
    return Person::GetGender();
}

void Programmer::AddProgrammingLanguage(ProgrammingLanguage language) {
    languages_.insert(language);
}

bool Programmer::CanProgram(ProgrammingLanguage language) const {
    int count = languages_.count(language);
    return (count > 0) ? true : false;
}

const string& Worker::GetName() const {
    return Person::GetName();
}

int Worker::GetAge() const {
    return Person::GetAge();
}

Gender Worker::GetGender() const {
    return Person::GetGender();
}

void Worker::AddSpeciality(WorkerSpeciality speciality) {
    specs_.insert(speciality);
}

bool Worker::HasSpeciality(WorkerSpeciality speciality) const {
    int count = specs_.count(speciality);
    return (count > 0) ? true : false;
}