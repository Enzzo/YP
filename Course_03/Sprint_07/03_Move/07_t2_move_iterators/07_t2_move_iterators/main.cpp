#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <iterator>

using namespace std::literals;

// ќбъ¤вл¤ем Sentence<Token> дл¤ произвольного типа Token
// синонимом vector<Token>.
// Ѕлагодар¤ этому в качестве возвращаемого значени¤
// функции можно указать не малопон¤тный вектор векторов,
// а вектор предложений Ч vector<Sentence<Token>>.
template <typename Token>
using Sentence = std::vector<Token>;

template <typename TokenForwardIt>
TokenForwardIt FindSentenceEnd(TokenForwardIt tokens_begin, TokenForwardIt tokens_end) {
    const TokenForwardIt before_sentence_end
        = std::adjacent_find(tokens_begin, tokens_end, [](const auto& left_token, const auto& right_token) {
        return left_token.IsEndSentencePunctuation() && !right_token.IsEndSentencePunctuation();
            });
    return before_sentence_end == tokens_end ? tokens_end : std::next(before_sentence_end);
}

//  ласс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
std::vector<Sentence<Token>> SplitIntoSentences(std::vector<Token> tokens) {
    // Ќапишите реализацию функции, не копиру¤ объекты типа Token

    std::vector<Sentence<Token>> result;
    auto last = tokens.begin();
    while (last != tokens.end()) {
        auto first = std::exchange(last, FindSentenceEnd(last, tokens.end()));
        Sentence<Token> t;
        std::move(std::make_move_iterator(first), std::make_move_iterator(last), std::back_inserter(t));
        result.push_back(std::move(t));
    }
    return result;
}

struct TestToken {
    std::string data;
    bool is_end_sentence_punctuation = false;


    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

std::ostream& operator<<(std::ostream& stream, const TestToken& token) {
    return stream << token.data;
}

// “ест содержит копировани¤ объектов класса TestToken.
// ƒл¤ проверки отсутстви¤ копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
    assert(SplitIntoSentences(std::vector<TestToken>({ {"Split"s}, {"into"s}, {"sentences"s}, {"!"s} }))
        == std::vector<Sentence<TestToken>>({ {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s}} }));

    assert(SplitIntoSentences(std::vector<TestToken>({ {"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true} }))
        == std::vector<Sentence<TestToken>>({ {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}} }));

    assert(SplitIntoSentences(std::vector<TestToken>(
        { {"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}, {"Without"s}, {"copies"s}, {"."s, true} }))
        == std::vector<Sentence<TestToken>>({
            {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}},
            {{"Without"s}, {"copies"s}, {"."s, true}},
            }));
}

int main() {
    TestSplitting();
    return 0;
}