#include <iostream>
#include <vector>
#include <iomanip>


class ReadersManager {
	std::vector<uint32_t> ids_; // size: [0; 10^5]
	std::vector<uint32_t> pages_; //size: [0; 1000]

public:
	ReadersManager() {
		ids_.reserve(1000000);
		pages_.reserve(1000);
	};
	void inline Read(const uint32_t id, const uint32_t page) noexcept;
	[[nodiscard]] inline double Cheer(const uint32_t id) const noexcept;
};

void inline ReadersManager::Read(const uint32_t id, const uint32_t page) noexcept {
	if (ids_.size() <= id) {
		ids_.resize(id + 1);
	}
	if (pages_.size() <= page) {
		pages_.resize(page + 1);
	}

	const uint32_t last_page = ids_[id];
	if (last_page < page) {
		ids_[id] = page;
	}

	for (uint32_t i = last_page + 1; i <= page; ++i) {
		pages_[i]++;
	}
}

[[nodiscard]] inline double ReadersManager::Cheer(const uint32_t id) const noexcept {
	if (ids_.size() <= id) {
		return 0.0;
	}

	const uint32_t last_page_by_id = ids_[id];

	if (last_page_by_id == 0) {
		return 0.0;
	}

	
	uint32_t total_readers = pages_[1];
	if (total_readers == 1) {
		return 1.0;
	}

	const uint32_t top_readers = pages_[last_page_by_id];

	if (total_readers == top_readers) {
		return 0.0;
	}

	double result = (total_readers - top_readers)/static_cast<double>(total_readers-1);
	return result;
}

int main() {
	ReadersManager rm;
	uint32_t q = 0; //[0; 10^6]
	std::string mode;

	std::cin >> q;	

	while (q-- > 0) {
		std::cin >> mode;
		if (mode == "READ") {
			uint32_t id, page;
			std::cin >> id >> page;
			rm.Read(id, page);
		}
		else if (mode == "CHEER") {
			uint32_t id;
			std::cin >> id;
			std::cout << rm.Cheer(id) << std::endl;
		}
	}

	return 0;
}