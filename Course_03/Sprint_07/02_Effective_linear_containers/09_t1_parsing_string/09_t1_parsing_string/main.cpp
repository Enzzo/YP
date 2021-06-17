#include <string>
#include <string_view>
#include <iostream>
#include <vector>
/*
std::vector<std::string_view> SplitIntoWordsView_old(std::string_view str) {
    std::vector<std::string_view> result;
    int64_t pos = 0;
    const int64_t pos_end = str.npos;
    while (true) {
        int64_t space = str.find(' ', pos);
        result.push_back(space == pos_end ? str.substr(pos) : str.substr(pos, space - pos));
        if (space == pos_end) {
            break;
        }
        else {
            pos = space + 1;
        }
    }

    return result;
}
*/

std::vector<std::string_view> SplitIntoWordsView(std::string_view str) {
    std::vector<std::string_view> result;
    const int64_t pos_end = str.npos;

    while (true) {
        int64_t space = str.find(' ');
        result.push_back(space == pos_end ? str : str.substr(0, space));
        if (space == pos_end) {
            break;
        }
        else {
            str.remove_prefix(space + 1);
        }
    }

    return result;
}

int main() {
    std::string text = "123 456789";
    SplitIntoWordsView(text);
    int x = 2;
	return 0;
}