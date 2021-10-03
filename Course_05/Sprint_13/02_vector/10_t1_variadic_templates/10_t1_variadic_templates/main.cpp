#include <cassert>
#include <string>
#include <string_view>

using namespace std;

template<typename T, typename... Type>
bool EqualsToOneOf(const T& v0, const Type&... vn) {

    if ((... || (v0 == vn))) {
        return true;
    }

    return false;
}

int main() {

    assert(EqualsToOneOf("hello"sv, "hi"s, "hello"s));
    assert(!EqualsToOneOf(1, 10, 2, 3, 6));
    assert(!EqualsToOneOf(8));
    return 0;
}