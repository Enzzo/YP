
#include "ini.h"

using namespace std;

ini::Section& ini::Document::AddSection(std::string name) {
    sections_[name];
    return sections_.at(name);
};

const ini::Section& ini::Document::GetSection(const std::string& name) const {
    auto it = sections_.find(name);
    ini::Section* s = new Section();

    if (it == sections_.end()) {
        return *s;
    }
    return sections_.at(name);
};

std::size_t ini::Document::GetSectionCount() const {
    return sections_.size();
};

void ini::TrimString(std::string& line, const char l = ' ', const char r = ' ') {
    line.erase(0, line.find_first_not_of(l));
    line.erase(line.find_last_not_of(r) + 1);
}

std::pair<std::string, std::string> ini::Split(const std::string& section) {
    int p = section.find('=');
    std::string left(section.begin(), section.begin() + p);
    std::string right(section.begin() + p + 1, section.end());
    TrimString(left);
    TrimString(right);
    return std::make_pair(left, right);
}

ini::Document ini::Load(std::istream& input) {
    ini::Document result;
    std::string line;
    std::string section_name;
    ini::Section* section_pointer = nullptr;

    while (!input.eof()) {
        std::getline(input, line);
        TrimString(line);

        if (line.size() == 0) {
            continue;
        }
        else if (line[0] == '[') {
            TrimString(line, '[', ']');
            section_pointer = &result.AddSection(line);
        }
        else {
            std::pair<std::string, std::string> section = Split(line);
            section_pointer->emplace(section.first, section.second);
        }
    }
    return result;
}