#include <iostream>
#include <string>
#include <cassert>

// ќпредел€ет, будет ли слово палиндромом
// text может быть строкой, содержащей строчные символы английского алфавита и пробелы
// ѕустые строки и строки, состо€щие только из пробелов, Ч это не палиндромы

std::string off_spaces(const std::string& text) {
    std::string s;
    for (const char ch : text) {
        if (!isspace(ch))s += ch;
    }
    return s;
};

bool IsPalindrome(const std::string& text) {
    std::string str = off_spaces(text);
    if (str.size() > 0) {
        int r;
        for (size_t l = 0; l < str.size() / 2; l++) {
            r = str.size() - (l+1);
            if (str[l] != str[r]) return false;            
        }
        return true;
    }
    return false;
}

void TestPalindromes() {
    assert(IsPalindrome("abba"));
    assert(IsPalindrome("") == true);
    std::cout << "TestPalindromes OK";
}

int main() {
    /*
    std::string text;
    getline(std::cin, text);
    
    if (IsPalindrome(text)) {
        std::cout << "palindrome" << std::endl;
    }
    else {
        std::cout << "not a palindrome" << std::endl;
    }
    */
    TestPalindromes();
}