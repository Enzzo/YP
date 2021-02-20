#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>

int CountAndAddNewDogs(
	const std::vector<std::string>&,
	const std::map<std::string, int>&,
	std::map<std::string, int>&);

int main() {
	std::map<std::string, int> shelter = { {"landseer", 1}, {"otterhound", 2}, {"pekingese", 2}, {"pointer", 3} };

	std::cout << CountAndAddNewDogs({ "landseer", "otterhound", "otterhound", "otterhound", "pointer" , "pointer" , "pointer" , "pointer" , "pointer" , "pointer" , "pointer" },
									{ {"landseer", 2}, {"otterhound", 3}, {"pekingese", 4}, {"pointer", 7} },
									shelter);

	return 0;
}

//new_dogs - вектор пород, попадающих в shelter
//max_amount - список пород и максимальное количество
//shelter - накопление new_dogs

int CountAndAddNewDogs(
	const std::vector<std::string>& new_dogs,
	const std::map<std::string, int>& max_amount,
		  std::map<std::string, int>& shelter){

	return count_if(new_dogs.begin(), new_dogs.end(), [&max_amount, &shelter](const std::string& dog) {

		if (shelter.at(dog) < max_amount.at(dog)) {
			++shelter[dog];
			return true;
		}

		return false;
		});
}