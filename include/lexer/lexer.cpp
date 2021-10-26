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
        std::string inp_line;

        while (getline(in, inp_line)) {
            if (inp_line.size() == 0) {
                continue;
            }
            size_t next_indent = CheckAndCutLine(inp_line);

            SetIndentLevel(next_indent);

            std::istringstream istring(inp_line);

            ReadLine(istring);            
        }
        SetIndentLevel(0);
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
    size_t Lexer::CheckAndCutLine(std::string& in) const {
        size_t result = in.find_first_not_of(' ', 0);
        if (result >= 2) {
            result /= 2;
        }
        in = in.substr(result);
        return result;
    }

    void Lexer::SetIndentLevel(const int new_level) {
        using namespace parse::token_type;
        for (size_t i = indent_level_; i < new_level; ++i) {
                line_.push_back(Indent({}));
            }
        for (size_t i = indent_level_; i > new_level; --i) {
            line_.push_back(Dedent({}));
        }
        if (indent_level_ != new_level) {
            indent_level_ = new_level;
        }
    }

    void Lexer::ReadLine(std::istringstream& istring) {
        using namespace parse::token_type;
        char t;
        while (istring.get(t)) {
            if (t == ' ') {
                continue;
            }
            if (isdigit(t)) {
                istring.unget();
                int d;
                istring >> d;
                line_.push_back(Number{ d });
            }
            else if (isprint(t)) {
                switch (t) {
                case '_': {
                    if (istring.peek() == '_') {
                        istring.get();
                        std::string id;
                        id += "__";
                        while (istring.get(t)) {
                            id += t;
                            if (t == '_' && istring.peek() == '_') {
                                istring.get(t);
                                id += t;
                                line_.push_back(Id({ id }));
                                break;
                            }
                        }
                    }
                    else {
                        istring.unget();
                        ReadId(istring);
                    }
                    break;
                }
                case '=': case '!': case '<': case'>': {
                    if (istring.peek() == '=') {
                        if (t == '=') {
                            line_.push_back(Eq({}));
                        }
                        else if (t == '!') {
                            line_.push_back(NotEq({}));
                        }
                        else if (t == '<') {
                            line_.push_back(LessOrEq({}));
                        }
                        else {
                            line_.push_back(GreaterOrEq({}));
                        }
                        istring.get();
                        break;
                    }
                    [[fallthrough]];
                }
                case '*': case '/': case '+':case '-': case '(': case ')': case ',': case ':': case '.':{
                    line_.push_back(Char{ t });
                    break;
                }
                case '\'': case '\"': {
                    std::string str;
                    std::getline(istring, str, t);
                    line_.push_back(String{ str });
                    break;
                }
                default: {
                    istring.unget();
                    ReadId(istring);
                }
                }
            }
        }
        line_.push_back(Newline{});
    }

    void Lexer::ReadId(std::istringstream& istring) {
        using namespace parse::token_type;
        std::string s;
        istring >> s;
        if (s == "class") {
            line_.push_back(Class({}));
        }
        else if (s == "return") {
            line_.push_back(Return({}));
        }
        else if (s == "if") {
            line_.push_back(If({}));
        }
        else if (s == "else") {
            line_.push_back(Else({}));
        }
        else if (s == "def") {
            line_.push_back(Def({}));
        }
        else if (s == "print") {
            line_.push_back(Print({}));
        }
        else if (s == "or") {
            line_.push_back(Or({}));
        }
        else if (s == "None") {
            line_.push_back(None({}));
        }
        else if (s == "and") {
            line_.push_back(And({}));
        }
        else if (s == "not") {
            line_.push_back(Not({}));
        }
        else if (s == "True") {
            line_.push_back(True({}));
        }
        else if (s == "False") {
            line_.push_back(False({}));
        }
        else if (ispunct(*(s.end() - 1))) {
            s = s.substr(0, s.size() - 1);
            line_.push_back(Id({ s }));
            line_.push_back(Char({ *(s.end() - 1) }));
        }
        else {
            line_.push_back(Id({ s }));
        }
    }
}  // namespace parse