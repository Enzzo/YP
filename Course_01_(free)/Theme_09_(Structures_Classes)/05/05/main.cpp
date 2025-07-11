#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>

class SearchServer {
private:
	std::map<std::string, std::set<int>> word_to_documents_;
	std::set<std::string> stop_words_;
public:
	int GetStopWordsSize() const {
		return stop_words_.size();
	}
};

int GetStopWrodsSize(const SearchServer& server) {
	return server.GetStopWordsSize();
}

int main() {
	return 0;
}