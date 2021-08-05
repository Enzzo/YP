#include <array>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

class VehiclePlate {
	std::array<char, 3> letters_;
	int digits_;
	int region_;

public:
	VehiclePlate(char l0, char l1, int digits, char l2, int region)
		: letters_{ l0, l1, l2 }
		, digits_(digits)
		, region_(region){}

	std::string ToString()const {
		std::ostringstream out;
		out << letters_[0] << letters_[1];
		// чтобы дополнить цифровую часть номера слева нулями
		// до трёх цифр, используем подобные манипуляторы:
		// setfill задаёт символ для заполнения,
		// right задаёт выравнивание по правому краю,
		// setw задаёт минимальное желаемое количество знаков
		out << std::setfill('0') << std::right << std::setw(3) << digits_;
		out << letters_[2] << std::setw(2) << region_;

		return out.str();
	}

	int Hash() const {
		return digits_;
	}
};

bool operator==(const VehiclePlate& lhs, const VehiclePlate& rhs) {
	const std::string& ls = lhs.ToString();
	const std::string& rs = rhs.ToString();
	//return std::lexicographical_compare(ls.begin(), ls.end(), rs.begin(), rs.end());
	return ls == rs;
}

std::ostream& operator<<(std::ostream& out, VehiclePlate plate) {
	out << plate.ToString();
	return out;
}

template<typename T>
class HashableContainer {
	std::vector<std::vector<T>> elements_;

public:
	void Insert(T elem) {
		int index = elem.Hash();
		
		// если вектор недостаточно велик для этого индекса,
		// то увеличим его, выделив место с запасом
		if (index >= int(elements_.size())) {
			elements_.resize(index * 2 + 1);
		}
		std::vector<T>& sub_elements = elements_[index];
		auto it = std::find(sub_elements.begin(), sub_elements.end(), elem);
		if(it == sub_elements.end())
			sub_elements.push_back(elem);
	}

	void PrintAll(std::ostream& out) const {
		for (auto& hash_numbers : elements_) {
			for (auto& number : hash_numbers) {
				out << number << std::endl;
			}
		}
	}

	const auto& GetVector() const {
		return elements_;
	}
};

int main() {
	HashableContainer<VehiclePlate> plate_base;
	plate_base.Insert({ 'B','H', 840, 'E', 99 });
	plate_base.Insert({ 'O','K', 942, 'K', 78 });
	plate_base.Insert({ 'O','K', 942, 'K', 78 });
	plate_base.Insert({ 'O','K', 942, 'K', 78 });
	plate_base.Insert({ 'O','K', 942, 'K', 78 });
	plate_base.Insert({ 'H','E', 968, 'C', 79 });
	plate_base.Insert({ 'T','A', 326, 'X', 83 });
	plate_base.Insert({ 'H','H', 831, 'P', 116 });
	plate_base.Insert({ 'A','P', 831, 'Y', 99 });
	plate_base.Insert({ 'P','M', 884, 'K', 23 });
	plate_base.Insert({ 'O','C', 34, 'P', 24 });
	plate_base.Insert({ 'M','Y', 831, 'M', 43 });
	plate_base.Insert({ 'B','P', 831, 'M', 79 });
	plate_base.Insert({ 'K','T', 478, 'P', 49 });
	plate_base.Insert({ 'X','P', 850, 'A', 50 });

	plate_base.PrintAll(std::cout);
}