#include <iostream>

class ReadersManager;

int main() {
	ReadersManager rm;
	int q = 0;
	std::string mode;

	std::cin >> q;	

	while (q-- > 0) {
		std::cin >> mode;
		if (mode == "READ") {
			int id, page;
			std::cin >> id >> page;
			rm.Read(id, page);
		}
		else if (mode == "CHEER") {
			int id;
			std::cin >> id;
			std::cout << rm.Cheer(id);
		}
		else {
			//throw
		}
	}

	return 0;
}

class ReadersManager {
public:
	void inline Read(const int id, const int page) noexcept;
	[[nodiscard]] inline double Cheer(const int id) const noexcept;
};

void inline ReadersManager::Read(const int id, const int page) noexcept{

}

[[nodiscard]] inline double ReadersManager::Cheer(const int id) const noexcept{
	return 0.0;
}