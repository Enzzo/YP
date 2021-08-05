#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

class MoneyBox {
	std::vector<int64_t> nominals_;
	std::vector<int> counts_;

public:
	explicit MoneyBox(std::vector<int64_t> nominals)
		: nominals_(std::move(nominals))
		, counts_(nominals_.size()){}

	const std::vector<int>& GetCounts() const {
		return counts_;
	}

	void PushCoin(int64_t value) {
		counts_[GetNominalIdx(value)]++;
	}

	void PrintCoins(std::ostream& out)const {
		for(const int64_t nominal : nominals_){
			int i = GetNominalIdx(nominal);
			if(counts_[i] > 0)
				out << nominals_[i] << ": " << counts_[i] << "\n";
		}
	}

private:
	int GetNominalIdx(int64_t nominal)const{
		return std::find(nominals_.begin(), nominals_.end(), nominal) - nominals_.begin();
	}
};

std::ostream& operator<<(std::ostream& out, const MoneyBox& cash) {
	cash.PrintCoins(out);
	return out;
}

int main() {

	MoneyBox cash({ 1, 500, 10000 });
	cash.PushCoin(500);
	cash.PushCoin(500);
	cash.PushCoin(10000);
	assert((cash.GetCounts() == vector<int>{0, 2, 1}));
	cout << cash << endl;
}