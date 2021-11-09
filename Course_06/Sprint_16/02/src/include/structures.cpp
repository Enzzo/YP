#include "common.h"

#include <cctype>
#include <sstream>
#include <ranges>

const int LETTERS = 26;
const int MAX_POSITION_LENGTH = 17;
const int MAX_POS_LETTER_COUNT = 3;

const Position Position::NONE = {-1, -1};

// Реализуйте методы:
bool Position::operator==(const Position rhs) const {
	return false;
}

bool Position::operator<(const Position rhs) const {
	return false;
}

bool Position::IsValid() const {
	return false;
}

std::string Position::ToString() const {
	return {};
}

Position Position::FromString(std::string_view str) {
	static std::regex r_template("([A-Z]{1,3})([0-9]{1,5})");
	if (std::regex_match(std::string(str), r_template)) {
		auto mid = std::find(str.begin(), str.end(), [](const char ch) {
			return isdigit(ch);
			});
		
	}
	return {};
}