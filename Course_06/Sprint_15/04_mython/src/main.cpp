#include "lexer.h"
#include "test_runner_p.h"

#include <iostream>
using namespace std::literals;

namespace parse {
void RunOpenLexerTests(TestRunner& tr);
}

int main() {
    try {
        /*std::string test1{ "long string with a double quote \" inside" };
        std::istringstream In(test1);
        parse::Lexer lex(In);
        auto first = lex.CurrentToken();
        std::cout << first << '\n';
        std::cout << lex.NextToken() << '\n';
        std::cout << lex.NextToken() << '\n';
        std::cout << lex.NextToken() << '\n';
        std::cout << lex.NextToken() << '\n';
        std::cout << lex.NextToken() << '\n';
        std::cout << lex.NextToken() << '\n';*/

        TestRunner tr;
        parse::RunOpenLexerTests(tr);
        
        parse::Token t;
        parse::Token test;
        
//        std::istringstream input(R"(
//print
//print:
//)");
        
        std::istringstream input(R"(
print x, x + 6, 'Hello, ' + w
print x                     
print y
class GCD:
  def calc(a, b):
    if a == 0 or b == 0:
      return a + b
    else:
      if a < b:
        return self.calc(b, a)
      else:
        # В Mython нет операции нахождения остатка от деления,
        # поэтому эмулируем её через имеющиеся операции
        # a - a / b * b == a % b
        return self.calc(b, a - a / b * b)
        y >= x
        yy > = xx
  def is_coprime(a, b):
    return self.calc(a, b) == 1

coprime = False

gcd = None
gcd = GCD()

x = 4
y = 13
coprime = gcd.is_coprime(x, y)
if coprime:
  print x, 'and', y, 'are coprime'
else:
  print x, 'and', y, 'are not coprime'

" quote \" inside" # comment # 1
" quote \' inside" # comment # 2
" quote \n inside" # comment # 1
" quote \t inside" # comment # 2
' quote \" inside' # comment # 1
' quote \' inside' # comment # 2
' quote \n inside' # comment # 1
' quote \t inside' # comment # 2
" string 'string' string"
' string "string" string'
int2str
True False == != <> <= >=

s = 'hello, ' + 'world' and or not

hashtag = "#hash";

class Fire:
  def __init__(obj):
    self.obj = obj

  def __str__():
    return "Burnt " + str(self.obj)

class Tree:
  def __str__():
    return "tree"

if x > 0:
  if y > 0:
    print "Эта строка выведется, если x и y положительные"
else:
  print "Эта строка выведется, если x <= 0" 

x = 4
w = 'world'
print x, x + 6, 'Hello, ' + w 

class Matches: # Спички
  # операция сложения спичек с другими объектами превращает их в огонь
  def __add__(smth):
    return Fire(smth)

result = Matches() + Tree()
print result             # Выведет Burnt tree
print Matches() + result # Выведет Burnt Burnt tree

x = 4#asdf
y = #;l;k"hello"

    #indented comment   

class Point;;;:
  def __init__(self, x, y):
    self.x = x
      self.y = y
        ""
        def __str__(self):
    return str(x) + ' ' + str(y)
"hello"
'world'
'long string with a double quote " inside'
"another long string with a single quote ' inside"

"string with a double quote \" inside"
'string with a single quote \' inside'
'', ""

p = Point(1, 2)
print str(p)
#asdf \n sdf  
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