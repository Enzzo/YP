#include <iostream>
#include <string>

template <typename Func>
void RunTestImpl(Func f, const std::string& s) {
    f();
    std::cerr << s << " OK ";
}

#define RUN_TEST(func)  RunTestImpl((func), #func)

void Test1() {
}
void Test2() {
}

int main() {
    RUN_TEST(Test1);
    RUN_TEST(Test2);
}