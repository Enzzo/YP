#include "domain.h"

Road operator+(const Road& lhs, const Road& rhs) {
	return { lhs.minutes + rhs.minutes, {}, {} };
}

bool operator<(const Road& lhs, const Road& rhs) {
	return lhs.minutes < rhs.minutes;
}

bool operator>(const Road& lhs, const Road& rhs) {
	return rhs < lhs;
}