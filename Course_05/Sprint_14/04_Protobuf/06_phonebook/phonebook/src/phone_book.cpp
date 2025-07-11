#include "phone_book.h"

using namespace std;

// Напишите тут реализации методов и функций, 
// декларированных в phone_book.h

PhoneBook::PhoneBook(std::vector<Contact> contacts) : contacts_(contacts) {

};

PhoneBook::ContactRange PhoneBook::FindByNamePrefix(std::string_view name_prefix) const {

	//TODO: если не будут проходить тесты, то попробовать обработать name_prefix с пробелом
	//name_prefix.size() будет большенуля, но вернуть надо всю последовательность

	std::sort(contacts_.begin(), contacts_.end(), [](const Contact& lhs, const Contact& rhs) {
		return lhs.name < rhs.name;
		});
	if (name_prefix.size() == 0) {
		return { contacts_.begin(), contacts_.end() };
	}
	
	auto lb_cmp = [](const Contact& cnt, std::string_view prefix) {
		return cnt.name < prefix;
	};

	auto ub_cmp = [](std::string_view prefix, const Contact& cnt) {
		int x = cnt.name.compare(0, prefix.size(), prefix);
		if (x == 0)return false;
		return prefix < cnt.name;
	};
	Iterator from = std::lower_bound(contacts_.begin(), contacts_.end(), name_prefix, lb_cmp);
	Iterator to = std::upper_bound(from, contacts_.end(), name_prefix, ub_cmp);
		
	auto x = std::distance(from, to);
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
	
	if (contacts.ParseFromIstream(&input)) {
		std::vector<Contact>v_contacts(contacts.contact_size());

		for (size_t i = 0; i < contacts.contact_size(); ++i) {
			auto contact = contacts.contact(i);
			v_contacts[i].name = contact.name();
			
			if (contact.has_birthday()) {
				auto bd  = contact.birthday();
				Date d = { bd.year(), bd.month(), bd.day() };
				v_contacts[i].birthday.emplace(d);
			}
			for (size_t j = 0; j < contact.phone_number_size(); ++j) {
				auto number = contact.phone_number(j);
				v_contacts[i].phones.push_back(number);
			}
		}
		return PhoneBook(v_contacts);
	}

	
	return PhoneBook({});
}