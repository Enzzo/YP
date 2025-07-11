#include <map>
#include <set>
#include <vector>
#include <string>
#include <string_view>
#include <cassert>

#include "translator.h"
#include "my_assert.h"

void TestSimple() {
    using namespace std::literals;

    Translator translator;
    translator.Add(std::string("okno"s), std::string("window"s));
    translator.Add(std::string("stol"s), std::string("table"s));

    assert(translator.TranslateForward("okno"s) == "window"s);
    assert(translator.TranslateBackward("table"s) == "stol"s);
    assert(translator.TranslateForward("table"s) == ""s);
}

int main() {
    TestSimple();
    return 0;
}