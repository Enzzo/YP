#include "lexer.h"

#include <algorithm>
#include <charconv>
#include <unordered_map>

using namespace std;

namespace parse {

    bool operator==(const Token& lhs, const Token& rhs) {
        using namespace token_type;

        if (lhs.index() != rhs.index()) {
            return false;
        }
        if (lhs.Is<Char>()) {
            return lhs.As<Char>().value == rhs.As<Char>().value;
        }
        if (lhs.Is<Number>()) {
            return lhs.As<Number>().value == rhs.As<Number>().value;
        }
        if (lhs.Is<String>()) {
            return lhs.As<String>().value == rhs.As<String>().value;
        }
        if (lhs.Is<Id>()) {
            return lhs.As<Id>().value == rhs.As<Id>().value;
        }
        return true;
    }

    bool operator!=(const Token& lhs, const Token& rhs) {
        return !(lhs == rhs);
    }

    std::ostream& operator<<(std::ostream& os, const Token& rhs) {
        using namespace token_type;

#define VALUED_OUTPUT(type) \
    if (auto p = rhs.TryAs<type>()) return os << #type << '{' << p->value << '}';

        VALUED_OUTPUT(Number);
        VALUED_OUTPUT(Id);
        VALUED_OUTPUT(String);
        VALUED_OUTPUT(Char);

#undef VALUED_OUTPUT

#define UNVALUED_OUTPUT(type) \
    if (rhs.Is<type>()) return os << #type;

        UNVALUED_OUTPUT(Class);
        UNVALUED_OUTPUT(Return);
        UNVALUED_OUTPUT(If);
        UNVALUED_OUTPUT(Else);
        UNVALUED_OUTPUT(Def);
        UNVALUED_OUTPUT(Newline);
        UNVALUED_OUTPUT(Print);
        UNVALUED_OUTPUT(Indent);
        UNVALUED_OUTPUT(Dedent);
        UNVALUED_OUTPUT(And);
        UNVALUED_OUTPUT(Or);
        UNVALUED_OUTPUT(Not);
        UNVALUED_OUTPUT(Eq);
        UNVALUED_OUTPUT(NotEq);
        UNVALUED_OUTPUT(LessOrEq);
        UNVALUED_OUTPUT(GreaterOrEq);
        UNVALUED_OUTPUT(None);
        UNVALUED_OUTPUT(True);
        UNVALUED_OUTPUT(False);
        UNVALUED_OUTPUT(Eof);

#undef UNVALUED_OUTPUT

        return os << "Unknown token :("sv;
    }

    Lexer::Lexer(std::istream& in) {
        // Реализуйте конструктор самостоятельно
        using namespace parse;
        using namespace token_type;
        char t;
        while (in.get(t)) {
            if (isspace(t)) {
                if (t == '\n') {
                    line_.push_back(Newline{});
                }
                else if (isspace(in.peek())) {
                    in.get(t);
                    line_.push_back(Indent{});
                }
                else {
                    continue;
                }                
            }
            if (isdigit(t)) {
                in.unget();
                int d;
                in >> d;
                line_.push_back(Number{ d });
            }
            if (isprint(t)) {
                switch (t) {
                case '*':case '/': case '+':case '-':case '=': {
                    line_.push_back(Char{ t });
                    break;
                }
                }
            }
            if(isalpha(t)){
                in.unget();
                std::string s;
                in >> s;
                line_.push_back(Id{ s });
            }            
        }
        line_.push_back(Eof{});
    }

    const Token& Lexer::CurrentToken() const {
        // Заглушка. Реализуйте метод самостоятельно
        if (head_ < line_.size()) {
            return line_[head_];
        }
        //throw std::logic_error("Not implemented"s);
    }

    Token Lexer::NextToken() {
        // Заглушка. Реализуйте метод самостоятельно
        if ((head_ + 1) < line_.size()) {
            head_++;
        }        
        return CurrentToken();
        
        //throw std::logic_error("Not implemented"s);
    }

}  // namespace parse