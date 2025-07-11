#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

void Assert(bool expression,
    const std::string& str,
    const std::string& file,
    const unsigned line,
    const std::string& function,
    const std::string& hint) {

    if (!expression) {
        std::cout << file << "(" << line << "): " << function << ": ASSERT(" << str << ") failed. ";
        if (!hint.empty()) {
            std::cout << "Hint: " << hint;
        }
        std::cout << std::endl;
        abort();
    }
}

template<typename T, typename U>
void AssertEqual(const T t, const U u, 
    const std::string& str,
    const std::string& file,
    const unsigned line,
    const std::string& function,
    const std::string& hint) {
    Assert(t == u, str, file, line, function, hint);
}

#define ASSERT_HINT(expr, hint) Assert((expr), #expr, __FILE__, __LINE__, __FUNCTION__, hint)
#define ASSERT(expr) ASSERT_HINT(expr, "")

#define ASSERT_EQUAL_HINT(left, right, hint)
#define ASSERT_EQUAL(left, right) ASSERT_EQUAL_HINT(left, right, "");

class Synonyms {
public:
    void Add(const std::string& first_word, const std::string& second_word) {
        synonyms_[first_word].insert(second_word);
        synonyms_[second_word].insert(first_word);
    }

    size_t GetSynonymCount(const std::string& word) const {
        if (synonyms_.count(word) != 0) {
            return synonyms_.at(word).size();
        }
        return 0;
    }

    bool AreSynonyms(const std::string& first_word, const std::string& second_word) const {
        if (synonyms_.count(first_word) != 0) {
            return synonyms_.at(first_word).count(second_word) != 0;
        }
        return false;
    }

private:
    std::map<std::string, std::set<std::string>> synonyms_;
};

void TestAddingSynonymsIncreasesTheirCount() {
    Synonyms synonyms;
    assert(synonyms.GetSynonymCount("music") == 0);
    assert(synonyms.GetSynonymCount("melody" ) == 0);

    synonyms.Add("music", "melody");
    assert(synonyms.GetSynonymCount("music") == 1);
    assert(synonyms.GetSynonymCount("melody") == 1);

    synonyms.Add("music", "tune");
    assert(synonyms.GetSynonymCount("music") == 2);
    assert(synonyms.GetSynonymCount("tune") == 1);
    assert(synonyms.GetSynonymCount("melody") == 1);
}

void TestAreSynonyms() {
    Synonyms synonyms;
    synonyms.Add("winner", "champion");
    synonyms.Add("good", "nice");

    assert(synonyms.AreSynonyms("winner", "champion"));
    assert(synonyms.AreSynonyms("champion", "winner"));

    assert(!synonyms.AreSynonyms("good", "champion"));
    assert(synonyms.AreSynonyms("good", "nice"));
}

void TestSynonyms() {
    TestAddingSynonymsIncreasesTheirCount();
    TestAreSynonyms();
}

int main() {
    TestSynonyms();

    Synonyms synonyms;

    std::string line;
    while (getline(std::cin, line)) {
        std::istringstream command(line);
        std::string action;
        command >> action;

        if (action == "ADD") {
            std::string first_word, second_word;
            command >> first_word >> second_word;
            synonyms.Add(first_word, second_word);
        }
        else if (action == "COUNT") {
            std::string word;
            command >> word;
            std::cout << synonyms.GetSynonymCount(word) << std::endl;
        }
        else if (action == "CHECK") {
            std::string first_word, second_word;
            command >> first_word >> second_word;
            if (synonyms.AreSynonyms(first_word, second_word)) {
                std::cout << "YES" << std::endl;
            }
            else {
                std::cout << "NO" << std::endl;
            }
        }
        else if (action == "EXIT") {
            break;
        }
    }
}