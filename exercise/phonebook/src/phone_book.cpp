#include "phone_book.h"

using namespace std;

// Напишите тут реализации методов и функций, 
// декларированных в phone_book.h

PhoneBook::PhoneBook(std::vector<Contact> contacts) : contacts_(contacts) {

};

PhoneBook::ContactRange PhoneBook::FindByNamePrefix(std::string_view name_prefix) const {
	PhoneBook::ContactRange range(nullptr, nullptr);
	return range;
};

void PhoneBook::SaveTo(std::ostream& output) const {
	phone_book_serialize::ContactList phone_book;
	for (const Contact contact : contacts_) {
		phone_book_serialize::Contact pb_contact;
		pb_contact.set_name(contact.name);

		if (contact.birthday.has_value()) {			
			pb_contact.mutable_birthday()->set_day(contact.birthday->day);
			pb_contact.mutable_birthday()->set_month(contact.birthday->month);
			pb_contact.mutable_birthday()->set_year(contact.birthday->year);
		}

		for (const std::string& phone : contact.phones) {
			pb_contact.add_phone_number(phone);
		}
	}
	phone_book.SerializeToOstream(&output);
}

PhoneBook DeserializePhoneBook(std::istream& input) {
	PhoneBook phone_book({});
	return phone_book;
}