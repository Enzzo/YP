#include "safe_add.h"
#include "test.h"

int main() {

    TestAll();

    int64_t a;
    int64_t b;
    std::cin >> a >> b;
    //std::cout << a + b << std::endl;
    auto add = SafeAdd(a, b);
    if (add.has_value()) {
        std::cout << *add;
    }
    else {
        std::cout << "Overflow!\n";
    }
    return 0;
}

