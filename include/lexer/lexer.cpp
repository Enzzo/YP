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
        size_t indent_level = 0;

        while (in.get(t)) {
            if (isspace(t)) {
                //ВЫЧИСЛИТЬ УРОВЕНЬ INDENT
                if (isspace(in.peek()) && line_[line_.size() - 1].second.Is<Newline>()) {
                    in.get(t);
                    line_.push_back(std::make_pair(indent_level, Indent{}));
                }
                else if (t == '\n' && line_.size() != 0) {
                    if (!line_[line_.size() - 1].second.Is<Newline>()) {
                        line_.push_back(std::make_pair(indent_level, Newline{}));
                    }
                }
                else {
                    continue;
                }                
            }
            else if (isdigit(t)) {
                in.unget();
                int d;
                in >> d;
                line_.push_back(std::make_pair(indent_level, Number{ d }));
            }
            else if (isprint(t)) {
                switch (t) {

                case '=': case '!': case '<': case'>': {
                    if (in.peek() == '=') {
                        if (t == '=') {
                            line_.push_back(std::make_pair(indent_level, Eq({})));
                        }
                        else if(t == '!'){
                            line_.push_back(std::make_pair(indent_level, NotEq({})));
                        }
                        else if (t == '<') {
                            line_.push_back(std::make_pair(indent_level, LessOrEq({})));
                        }
                        else {
                            line_.push_back(std::make_pair(indent_level, GreaterOrEq({})));
                        }
                        in.get();  
                        break;
                    }
                    [[fallthrough]];
                }
                case '*': case '/': case '+':case '-': {
                    line_.push_back(std::make_pair(indent_level, Char{ t }));
                    break;
                }
                case '\'': case '\"': {
                    std::string str;
                    std::getline(in, str, t);
                    line_.push_back(std::make_pair(indent_level, String{ str }));
                    break;
                }
                default: {
                    in.unget();
                    std::string s;
                    in >> s;
                    if (s == "class") {
                        line_.push_back(std::make_pair(indent_level, Class({})));
                    }
                    else if (s == "return") {
                        line_.push_back(std::make_pair(indent_level, Return({})));
                    }
                    else if (s == "if") {
                        line_.push_back(std::make_pair(indent_level, If({})));
                    }
                    else if (s == "else") {
                        line_.push_back(std::make_pair(indent_level, Else({})));
                    }
                    else if (s == "def") {
                        line_.push_back(std::make_pair(indent_level, Def({})));
                    }
                    else if (s == "print") {
                        line_.push_back(std::make_pair(indent_level, Print({})));
                    }
                    else if (s == "or") {
                        line_.push_back(std::make_pair(indent_level, Or({})));
                    }
                    else if (s == "None") {
                        line_.push_back(std::make_pair(indent_level, None({})));
                    }
                    else if (s == "and") {
                        line_.push_back(std::make_pair(indent_level, And({})));
                    }
                    else if (s == "not") {
                        line_.push_back(std::make_pair(indent_level, Not({})));
                    }
                    else if (s == "True") {
                        line_.push_back(std::make_pair(indent_level, True({})));
                    }
                    else if (s == "False") {
                        line_.push_back(std::make_pair(indent_level, False({})));
                    }
                    else {
                        line_.push_back(std::make_pair(indent_level, Id{ s }));
                    }
                }
                }
            }
        }
        line_.push_back(std::make_pair(indent_level, Eof{}));
    }

    const Token& Lexer::CurrentToken() const {
        // Заглушка. Реализуйте метод самостоятельно
        if (head_ < line_.size()) {
            return line_[head_].second;
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