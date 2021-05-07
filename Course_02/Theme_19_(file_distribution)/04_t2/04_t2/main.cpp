#include <iostream>
#include "carpenter.h"

int main() {

	Carpenter Jack;
	Wall wall(3.5, 2.45);
	std::cout << Jack.CalcShelves(wall); //4

	return 0;
}