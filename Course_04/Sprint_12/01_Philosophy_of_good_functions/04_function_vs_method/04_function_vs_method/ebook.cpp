#include <iostream>
#include <vector>
#include <iomanip>


class ReadersManager {
	std::vector<uint64_t> ids_;
	std::vector<uint64_t> pages_;

public:
	void inline Read(const uint64_t id, const uint64_t page) noexcept;
	[[nodiscard]] inline double Cheer(const uint64_t id) const noexcept;
};

void inline ReadersManager::Read(const uint64_t id, const uint64_t page) noexcept {
	if (ids_.size() <= id) {
		ids_.resize(id + 1);
	}
	if (pages_.size() <= page) {
		pages_.resize(page + 1);
	}

	const uint64_t last_page = ids_[id];
	if (last_page < page) {
		ids_[id] = page;
	}

	for (uint64_t i = last_page + 1; i <= page; ++i) {
		pages_[i]++;
	}
}

[[nodiscard]] inline double ReadersManager::Cheer(const uint64_t id) const noexcept {
	if (ids_.size() <= id) {
		return 0.0;
	}

	uint64_t total_readers = pages_[1];
	if (total_readers == 1) {
		return static_cast<double>(1);
	}

	const uint64_t last_page = ids_[id];
	const uint64_t top_readers = pages_[last_page];

	if (total_readers == top_readers) {
		return 0.0;
	}

	double result = (total_readers - top_readers)/static_cast<double>(total_readers-1);
	return result;
}

int main() {
	ReadersManager rm;
	uint32_t q = 0;
	std::string mode;

	std::cin >> q;	

	while (q-- > 0) {
		std::cin >> mode;
		if (mode == "READ") {
			uint64_t id, page;
			std::cin >> id >> page;
			rm.Read(id, page);
		}
		else if (mode == "CHEER") {
			uint64_t id;
			std::cin >> id;
			std::cout << rm.Cheer(id) << std::endl;
		}
	}

	return 0;
}