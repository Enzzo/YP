#include <iostream>
#include <string>

void IsPalindrome(const std::string&);

int main(){

    IsPalindrome("aa");
    IsPalindrome("deer");
    IsPalindrome("");

    return 0;
}

void IsPalindrome(const std::string& s) {
    bool is_palindrome = true;

    for (size_t i = 0; i < s.size() / 2; i++) {
        if (s[i] != s[s.size() - i -1]) {
            is_palindrome = false;
            break;
        }
    }

    std::cout << is_palindrome << std::endl;
}