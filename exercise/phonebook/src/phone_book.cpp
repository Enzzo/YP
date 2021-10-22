#include "phone_book.h"

using namespace std;

// Напишите тут реализации методов и функций, 
// декларированных в phone_book.h

PhoneBook::PhoneBook(std::vector<Contact> contacts) : contacts_(contacts) {

};

PhoneBook::ContactRange PhoneBook::FindByNamePrefix(std::string_view name_prefix) const {

	std::sort(contacts_.begin(), contacts_.end(), [](const Contact& lhs, const Contact& rhs) {
		return lhs.name < rhs.name;
		});
	int y = 3;
	if (name_prefix.size() == 0) {
		return { contacts_.begin(), contacts_.end() };
	}

	auto lb_cmp = [](const Contact& cnt, const std::string_view prefix) {
		int x = 2;
		return cnt.name < prefix;
	};

	auto ub_cmp = [](const std::string_view prefix, const Contact& cnt) {
		int x = 2;
		return !(prefix < cnt.name);
	};

	Iterator from = std::lower_bound(contacts_.begin(), contacts_.end(), name_prefix, lb_cmp);
	Iterator to = std::upper_bound(contacts_.begin(), contacts_.end(), name_prefix, ub_cmp);	
	
	return { from, to};
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
		phone_book.mutable_contact()->Add(std::move(pb_contact));
	}
	phone_book.SerializeToOstream(&output);
}

PhoneBook DeserializePhoneBook(std::istream& input) {
	
	phone_book_serialize::ContactList contacts;
	contacts.ParseFromIstream(&input);

	PhoneBook phone_book({});
	return phone_book;
}