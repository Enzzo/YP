#include <iostream>
#include <vector>
#include <iomanip>


class ReadersManager {
	std::vector<int> ids_;
	std::vector<int> pages_;

public:
	void inline Read(const int id, const int page) noexcept;
	[[nodiscard]] inline float Cheer(const int id) const noexcept;
};

void inline ReadersManager::Read(const int id, const int page) noexcept {
	if (ids_.size() <= id) {
		ids_.resize(id + 1);
	}
	if (pages_.size() <= page) {
		pages_.resize(page + 1);
	}

	const int last_page = ids_[id];
	if (last_page < page) {
		ids_[id] = page;
	}

	for (int i = 0; i <= page; ++i) {
		pages_[i]++;
	}
}

[[nodiscard]] inline float ReadersManager::Cheer(const int id) const noexcept {
	if (ids_.size() <= id) {
		return 0.0;
	}
	return 1 / static_cast<float>(pages_[ids_[id]]);
}

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
