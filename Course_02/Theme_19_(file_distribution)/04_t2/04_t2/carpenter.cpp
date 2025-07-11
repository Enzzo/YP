#include "carpenter.h"
#include "square_calculation.h"

int Carpenter::CalcShelves(const Wall& wall)const {
	return static_cast<int>(CalcSquare(wall.GetHeight(), wall.GetWidth())) / 2;
}