#include "lexer.h"
#include "test_runner_p.h"

#include <iostream>
using namespace std::literals;

namespace parse {
void RunOpenLexerTests(TestRunner& tr);
}

int main() {
    try {
        TestRunner tr;
        parse::RunOpenLexerTests(tr);
        
        parse::Token t;
        parse::Token test;
        
        std::istringstream input(R"(
x = 4#asdf
y = #;l;k"hello"

    #indented comment   

class Point:
  def __init__(self, x, y):
    self.x = x
      self.y = y

        def __str__(self):
    return str(x) + ' ' + str(y)

p = Point(1, 2)
print str(p)
)"s);
        parse::Lexer test_lexer(input);
        while ((t = test_lexer.CurrentToken()) != parse::token_type::Eof{}) {
            std::cout << t << std::endl;
            test_lexer.NextToken();
        }

        /*parse::Lexer lexer(std::cin);
        test = lexer.CurrentToken();
        while ((t = lexer.CurrentToken()) != parse::token_type::Eof{}) {
            std::cout << t << std::endl;
            lexer.NextToken();
        }*/
    } catch (const std::exception& e) {
        std::cerr << e.what();
        return 1;
    }
}