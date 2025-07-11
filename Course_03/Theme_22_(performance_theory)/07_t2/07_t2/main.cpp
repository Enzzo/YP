#include <iostream>
#include <string>

using namespace std::literals;

template <typename F>
std::string BruteForce(F check) {
    // верните строку str, для которой check(str) будет true
    std::string str = "aaaaa";
    
    for (char a = 'A'; a <= 'Z'; ++a) {

        str[0] = a;

        for (char b = 'A'; b <= 'Z'; ++b) {

            str[1] = b;

            for (char c = 'A'; c <= 'Z'; ++c) {

                str[2] = c;

                for (char d = 'A'; d <= 'Z'; ++d) {

                    str[3] = d;

                    for (char e = 'A'; e <= 'Z'; ++e) {

                        str[4] = e;

                        if (check(str)) {

                            return str;
                        }
                    }
                }
            }
        }
    }
    return str;
}

int main() {
    
    std::string pass = "ARTUR"s;
    
    auto check = [pass](const std::string& s) {
        return s == pass;
    };
    
    std::cout << BruteForce(check) << std::endl;

    return 0;
}