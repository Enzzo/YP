#include "common.h"

#include <cctype>
#include <sstream>
#include <regex>
#include <cmath>

const int LETTERS = 26;
const int MAX_POSITION_LENGTH = 17;
const int MAX_POS_LETTER_COUNT = 3;

const Position Position::NONE = {-1, -1};

// Реализуйте методы:
bool Position::operator==(const Position rhs) const {
	return this->row == rhs.row 
		&& this->col == rhs.col;
}

bool Position::operator<(const Position rhs) const {
	
	return col < rhs.col
			&& row < rhs.row;
}

bool Position::IsValid() const {
	return 0 <= col && col < MAX_COLS
		&& 0 <= row && row < MAX_ROWS;
}

std::string Position::ToString() const {
	if (!IsValid()) {
		return "";
	}
	std::string result;
	int c = col;

	while(true){
		int p = c % 26;
		c /= 26;
		result.insert(result.begin(),char(p + 65));	
		if (c == 0) {
			break;
		}
		--c;
	};

	std::stringstream ss;
	ss << (row + 1);
	result += ss.str();
	return result;
}

Position Position::FromString(std::string_view str) {
	std::regex r_template("([A-Z]{1,3})([0-9]{1,5})");

	if (std::regex_match(std::string(str), r_template)) {

		auto mid = std::find_if(str.begin(), str.end(), [](const char ch) {
			return isdigit(ch);
			});

		Position result;

		//конвертируем столбцы
		size_t part = std::distance(str.begin(), mid);
		for (size_t i = 0; i < part; ++i) {
			const char& ch = *(str.begin() + (part - 1 - i));
			result.col += (ch - 64) * pow(26, i);
		}
		--result.col;

		//конвертируем ряды		
		std::istringstream ist(std::string(mid, str.end()));
		ist >> result.row;
		--result.row;

		//возвращаем результат
		if (result.IsValid()) {
			return result;
		}
	}
	return Position::NONE;
}