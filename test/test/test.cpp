#include <iostream>
#include <string>
#include <vector>

int main() {
	int q;
	std::cin >> q;

	std::vector<bool> query;

	for (int i = 0; i < q; ++i) {
		std::string operation_code;
		std::cin >> operation_code;

		//WORRY_COUNT: узнать количество беспокоящихся людей в очереди.
		if (operation_code == "WORRY_COUNT") {
			int count = 0;
			for (const bool b : query) {
				count += b;
			}
			std::cout << count << std::endl;
		}
		
		//WORRY i : пометить i - го человека с начала очереди(в нумерации с 0) как беспокоящегося;
		else if (operation_code == "WORRY" || operation_code == "QUIET") {
			int i;
			std::cin >> i;
			query[i] = operation_code == "WORRY";
		}

		//COME k : добавить k спокойных человек в конец очереди;
		//COME - k: убрать k человек из конца очереди;
		else if (operation_code == "COME") {
			int k;
			std::cin >> k;
			if (k != 0) 
				query.resize(query.size() + k);
			
		}
	}
	return 0;
}