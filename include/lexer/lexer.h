#pragma once

#include <iosfwd>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>

#include <vector>

namespace parse {

    namespace token_type {
        struct Number {  // Лексема «число»
            int value;   // число
        };

        struct Id {             // Лексема «идентификатор»
            std::string value;  // Имя идентификатора
        };

        struct Char {    // Лексема «символ»
            char value;  // код символа
        };

        struct String {  // Лексема «строковая константа»
            std::string value;
        };

        struct Class {};    // Лексема «class»
        struct Return {};   // Лексема «return»
        struct If {};       // Лексема «if»
        struct Else {};     // Лексема «else»
        struct Def {};      // Лексема «def»
        struct Newline {};  // Лексема «конец строки»
        struct Print {};    // Лексема «print»
        struct Indent {};  // Лексема «увеличение отступа», соответствует двум пробелам
        struct Dedent {};  // Лексема «уменьшение отступа»
        struct Eof {};     // Лексема «конец файла»
        struct And {};     // Лексема «and»
        struct Or {};      // Лексема «or»
        struct Not {};     // Лексема «not»
        struct Eq {};      // Лексема «==»
        struct NotEq {};   // Лексема «!=»
        struct LessOrEq {};     // Лексема «<=»
        struct GreaterOrEq {};  // Лексема «>=»
        struct None {};         // Лексема «None»
        struct True {};         // Лексема «True»
        struct False {};        // Лексема «False»
    }  // namespace token_type

    using TokenBase
        = std::variant<token_type::Number, token_type::Id, token_type::Char, token_type::String,
        token_type::Class, token_type::Return, token_type::If, token_type::Else,
        token_type::Def, token_type::Newline, token_type::Print, token_type::Indent,
        token_type::Dedent, token_type::And, token_type::Or, token_type::Not,
        token_type::Eq, token_type::NotEq, token_type::LessOrEq, token_type::GreaterOrEq,
        token_type::None, token_type::True, token_type::False, token_type::Eof>;

    struct Token : TokenBase {
        using TokenBase::TokenBase;

        template <typename T>
        [[nodiscard]] bool Is() const {
            return std::holds_alternative<T>(*this);
        }

        template <typename T>
        [[nodiscard]] const T& As() const {
            return std::get<T>(*this);
        }

        template <typename T>
        [[nodiscard]] const T* TryAs() const {
            return std::get_if<T>(this);
        }
    };

    bool operator==(const Token& lhs, const Token& rhs);
    bool operator!=(const Token& lhs, const Token& rhs);

    std::ostream& operator<<(std::ostream& os, const Token& rhs);

    class LexerError : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class Lexer {
    private:
        std::vector<Token> line_;
        size_t head_ = 0;
        size_t indent_level_ = 0;

    public:
        explicit Lexer(std::istream& input);

        // Возвращает ссылку на текущий токен или token_type::Eof, если поток токенов закончился
        [[nodiscard]] const Token& CurrentToken() const;

        // Возвращает следующий токен, либо token_type::Eof, если поток токенов закончился
        Token NextToken();

        // Если текущий токен имеет тип T, метод возвращает ссылку на него.
        // В противном случае метод выбрасывает исключение LexerError
        template <typename T>
        const T& Expect() const {
            using namespace std::literals;
            if (line_[head_].Is<T>())
                return line_[head_].As<T>();
            // Заглушка. Реализуйте метод самостоятельно
            throw LexerError("Not implemented"s);
        }

        // Метод проверяет, что текущий токен имеет тип T, а сам токен содержит значение value.
        // В противном случае метод выбрасывает исключение LexerError
        template <typename T, typename U>
        void Expect(const U& value) const {
            using namespace std::literals;
            if (!line_[head_].Is<T>() || line_[head_].As<T>().value != value) {
                throw LexerError("Not implemented"s);
            }
            // Заглушка. Реализуйте метод самостоятельно                
        }

        // Если следующий токен имеет тип T, метод возвращает ссылку на него.
        // В противном случае метод выбрасывает исключение LexerError
        template <typename T>
        const T& ExpectNext() {
            if ((head_ + 1) < line_.size()) {
                head_++;
            }
            return Expect<T>();
        }

        // Метод проверяет, что следующий токен имеет тип T, а сам токен содержит значение value.
        // В противном случае метод выбрасывает исключение LexerError
        template <typename T, typename U>
        void ExpectNext(const U& value) {
            if ((head_ + 1) < line_.size()) {
                head_++;
            }
            Expect<T>(value);
        }

    private:
        size_t CheckAndCutLine(std::string&) const;
        int IndentLevelOfLine(const std::string_view)const;
        void ReadLine(std::istringstream&);
        void SetIndentLevel(const size_t);
        void ReadId(std::istringstream&);                   //чтение идентификаторов
        void ReadNumber(std::istringstream&);               //чтение чисел
        void ReadString(std::istringstream&, const char);   //чтение строк в кавычках
        [[nodiscard]] bool EmptyLine(const std::string_view) const;   //проверка, если строка пустая, либо содержит только комментарий
    };

}  // namespace parse