#pragma once

#include "iterator_range.h"

#include <contact.pb.h>

#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

struct Date {
    int year, month, day;
};

struct Contact {
    std::string name;
    std::optional<Date> birthday;
    std::vector<std::string> phones;
};

class PhoneBook {
    mutable std::vector<Contact> contacts_;

public:
    explicit PhoneBook(std::vector<Contact> contacts);

    // Корректный тип итератора определите сами.
    // Можно и нужно поменять этот using:
    using Iterator = std::vector<Contact>::iterator;

    using ContactRange = IteratorRange<Iterator>;

    ContactRange FindByNamePrefix(std::string_view name_prefix) const;

    void SaveTo(std::ostream& output) const;
};

PhoneBook DeserializePhoneBook(std::istream& input);