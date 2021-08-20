#include "json.h"

#include <utility>

using namespace std;

namespace json {

    namespace {

        Node LoadNode(istream& input);

        Node LoadArray(istream& input) {
            Array result;

            for (char c; input >> c && c != ']';) {
                if (c != ',') {
                    input.putback(c);
                }
                result.push_back(LoadNode(input));
            }
            if (!input) {
                throw ParsingError("Array parsing error"s);
            }
            return Node(move(result));
        }

        /*Node LoadInt(istream& input) {
            int result = 0;
            while (isdigit(input.peek())) {
                result *= 10;
                result += input.get() - '0';
            }
            return Node(result);
        }*/

        Node LoadString(istream& input) {
            auto it = istreambuf_iterator<char>(input);
            auto end = istreambuf_iterator<char>();
            string s;
            while (true) {
                if (it == end) {
                    throw ParsingError("String parsing error");
                }
                const char ch = *it;
                if (ch == '"') {
                    ++it;
                    break;
                }
                else if (ch == '\\') {
                    ++it;
                    if (it == end) {
                        throw ParsingError("String parsing error");
                    }
                    const char escaped_char = *(it);
                    switch (escaped_char) {
                    case 'n':
                        s.push_back('\n');
                        break;
                    case 't':
                        s.push_back('\t');
                        break;
                    case 'r':
                        s.push_back('\r');
                        break;
                    case '"':
                        s.push_back('"');
                        break;
                    case '\\':
                        s.push_back('\\');
                        break;
                    default:
                        throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
                    }
                }
                else if (ch == '\n' || ch == '\r') {
                    throw ParsingError("Unexpected end of line"s);
                }
                else {
                    s.push_back(ch);
                }
                ++it;
            }

            return Node(move(s));
        }

        string LoadLiteral(istream& input) {
            string s;
            while (isalpha(input.peek())) {
                s.push_back(static_cast<char>(input.get()));
            }
            return s;
        }

        Node LoadNull(istream& input) {
            if (auto literal = LoadLiteral(input); literal == "null"sv) {
                return Node{ nullptr };
            }
            else {
                throw ParsingError("Failed to parse '"s + literal + "' as null"s);
            }
        }

        Node LoadDict(istream& input) {
            Dict dict;

            for (char c; input >> c && c != '}';) {
                if (c == '"') {
                    std::string key = LoadString(input).AsString();
                    if (input >> c && c == ':') {
                        if (dict.find(key) != dict.end()) {
                            throw ParsingError("Duplicate key '"s + key + "' have been found");
                        }
                        dict.emplace(move(key), LoadNode(input));
                    }
                    else {
                        throw ParsingError(": is expected but '"s + c + "' has been found"s);
                    }
                }
                else if (c != ',') {
                    throw ParsingError(R"(',' is expected but ')"s + c + "' has been found"s);
                }
            }
            if (!input) {
                throw ParsingError("Dictionary parsing error"s);
            }
            return Node(move(dict));
        }

        Node LoadBool(istream& input) {
            const auto s = LoadLiteral(input);
            if (s == "true"sv) {
                return Node{ true };
            }
            else if (s == "false"sv) {
                return Node{ false };
            }
            else {
                throw ParsingError("Failed to parse '"s + s + "' as bool"s);
            }
        }

        Node LoadNumber(istream& input) {
            string parsed_num;

            auto read_char = [&parsed_num, &input] {
                parsed_num += static_cast<char>(input.get());
                if (!input) {
                    throw ParsingError("Failed to read number from stream"s);
                }
            };

            auto read_digits = [&input, read_char] {
                if (!isdigit(input.peek())) {
                    throw ParsingError("A digit is expected"s);
                }
                while (isdigit(input.peek())) {
                    read_char();
                }
            };

            if (input.peek() == '-') {
                read_char();
            }

            if (input.peek() == '0') {
                read_char();
            }
            else {
                read_digits();
            }

            bool is_int = true;
            if (input.peek() == '.') {
                read_char();
                read_digits();
                is_int = false;
            }

            if (int ch = input.peek(); ch == 'e' || ch == 'E') {
                read_char();
                if (ch = input.peek(); ch == '+' || ch == '-') {
                    read_char();
                }
                read_digits();
                is_int = false;
            }

            try {
                if (is_int) {
                    try {
                        return Node{ stoi(parsed_num) };
                    }
                    catch (...) {}
                }
                return Node{ stod(parsed_num) };
            }
            catch (...) {
                throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
            }
        }

        Node LoadNode(istream& input) {
            char c;
            if (!(input >> c)) {
                throw ParsingError("Unexpected EOF"s);
            }
            switch (c) {
            case '[':
                return LoadArray(input);
            case '{':
                return LoadDict(input);
            case '"':
                return LoadString(input);
            case 't':
                [[fallthrough]];
            case 'f':
                input.putback(c);
                return LoadBool(input);
            case 'n':
                input.putback(c);
                return LoadNull(input);
            default:
                input.putback(c);
                return LoadNumber(input);
            }
        }

        struct PrintContext {
            std::ostream& out;
            int indent_step = 4;
            int indent = 0;

            void PrintIndent() const {
                for (int i = 0; i < indent; ++i) {
                    out.put(' ');
                }
            }

            PrintContext Indented() const {
                return { out, indent_step, indent_step + indent };
            }
        };
        void PrintNode(const Node& value, const PrintContext& ctx);

        template <typename Value>
        void PrintValue(const Value& value, const PrintContext& ctx) {
            ctx.out << value;
        }

        void PrintString(const string& value, ostream& out) {
            out.put('"');
            for (const char c : value) {
                switch (c) {
                case '\r':
                    out << "\\r"sv;
                    break;
                case '\n':
                    out << "\\n"sv;
                    break;
                case '"':
                    [[fallthrough]];
                case '\\':
                    out.put('\\');
                    [[fallthrough]];
                default:
                    out.put(c);
                    break;
                }
            }
            out.put('"');
        }

        template <>
        void PrintValue<string>(const string& value, const PrintContext& ctx) {
            PrintString(value, ctx.out);
        }

        template <>
        void PrintValue<nullptr_t>(const nullptr_t&, const PrintContext& ctx) {
            ctx.out << "null"sv;
        }

        template <>
        void PrintValue<bool>(const bool& value, const PrintContext& ctx) {
            ctx.out << (value ? "true"sv : "false"sv);
        }

        template <>
        void PrintValue<Array>(const Array& nodes, const PrintContext& ctx) {
            ostream& out = ctx.out;
            out << "[\n"sv;
            bool first = true;
            auto inner_ctx = ctx.Indented();
            for (const Node& node : nodes) {
                if (first) {
                    first = false;
                }
                else {
                    out << ",\n"sv;
                }
                inner_ctx.PrintIndent();
                PrintNode(node, inner_ctx);
            }
            out.put('\n');
            ctx.PrintIndent();
            out.put(']');
        }

        template <>
        void PrintValue<Dict>(const Dict& nodes, const PrintContext& ctx) {
            ostream& out = ctx.out;
            out << "{\n"sv;
            bool first = true;
            auto inner_ctx = ctx.Indented();
            for (const auto& [key, node] : nodes) {
                if (first) {
                    first = false;
                }
                else {
                    out << ",\n"sv;
                }
                inner_ctx.PrintIndent();
                PrintString(key, ctx.out);
                out << ": "sv;
                PrintNode(node, inner_ctx);
            }
            out.put('\n');
            ctx.PrintIndent();
            out.put('}');
        }

        void PrintNode(const Node& node, const PrintContext& ctx) {
            visit(
                [&ctx](const auto& value) {
                    PrintValue(value, ctx);
                },
                node.GetValue());
        }

    }  // namespace

    Node::Node() {
        content_ = nullptr;
    }

    Node::Node(std::nullptr_t t) {
        content_ = t;
    }
    Node::Node(Array array)
        : content_(move(array)) {
    }

    Node::Node(Dict map)
        : content_(move(map)) {
    }

    Node::Node(bool b)
        : content_(move(b)) {
    }

    Node::Node(int value)
        : content_(value) {
    }

    Node::Node(double value)
        : content_(value) {
    }

    Node::Node(string value)
        : content_(move(value)) {
    }

    bool Node::IsArray() const {
        return std::holds_alternative<Array>(content_);
    }

    const Array& Node::AsArray() const {
        if (!IsArray()) {
            throw logic_error("Not an array"s);
        }
        return std::get<Array>(content_);
    }

    Array& Node::AsArray() {
        if (!IsArray()) {
            throw logic_error("Not an array"s);
        }
        return std::get<Array>(content_);
    }

    bool Node::IsInt() const {
        return std::holds_alternative<int>(content_);
    }

    int Node::AsInt() const {
        if (!IsInt()) {
            throw logic_error("Not an int"s);
        }
        return std::get<int>(content_);
    }

    bool Node::IsPureDouble() const {
        return std::holds_alternative<double>(content_);
    }

    bool Node::IsDouble() const {
        return IsInt() || IsPureDouble();
    }

    double Node::AsDouble() const {
        return IsPureDouble() ? std::get<double>(content_) : AsInt();
    }
    
    bool Node::IsBool() const {
        return std::holds_alternative<bool>(content_);
    }

    bool Node::AsBool() const {
        if (!IsBool()) {
            throw logic_error("Not a bool"s);
        }
        return std::get<bool>(content_);
    }

    bool Node::IsString() const {
        return std::holds_alternative<std::string>(content_);
    }

    const string& Node::AsString() const {
        if (!IsString()) {
            throw logic_error("Not a string"s);
        }
        return std::get<std::string>(content_);
    }

    bool Node::IsMap() const {
        return std::holds_alternative<Dict>(content_);
    }

    const Dict& Node::AsMap() const {
        if (!IsMap()) {
            throw logic_error("Not a dict"s);
        }
        return std::get<Dict>(content_);
    }

    Dict& Node::AsMap() {
        if (!IsMap()) {
            throw logic_error("Not a dict"s);
        }
        return std::get<Dict>(content_);
    }

    bool Node::IsNull() const {
        return std::holds_alternative<std::nullptr_t>(content_);
    }

    const Value& Node::GetValue() const {
        return content_;
    }

    bool Node::operator==(const Node& other) const {
        return (this->content_ == other.content_);
    }

    bool Node::operator!=(const Node& other) const {
        return !(*this == other);
    }

    Document::Document(Node root)
        : root_(move(root)) {
    }

    bool Document::operator==(const Document& other) const {
        return (this->root_ == other.root_);
    }

    bool Document::operator!=(const Document& other) const {
        return !(*this == other);
    }

    const Node& Document::GetRoot() const {
        return root_;
    }

    Document Load(istream& input) {
        return Document{ LoadNode(input) };
    }

    void Print(const Document& doc, std::ostream& output) {


        PrintNode(doc.GetRoot(), PrintContext{ output });
    }

    //------------ BuildConstructor ---------------

    BuildConstructor::BuildConstructor(Builder& builder)
        : builder_(builder) {}

    //------------ BuildContextFirst ---------------

    BuildContextFirst::BuildContextFirst(Builder& builder)
        : BuildConstructor(builder) {}

    DictContext& BuildContextFirst::StartDict() {
        return builder_.StartDict();
    }

    ArrayContext& BuildContextFirst::StartArray() {
        return builder_.StartArray();
    }

    //------------ BuildContextSecond ---------------

    BuildContextSecond::BuildContextSecond(Builder& builder)
        : BuildConstructor(builder) {}

    KeyContext& BuildContextSecond::Key(std::string key) {
        return builder_.Key(key);
    }

    Builder& BuildContextSecond::EndDict() {
        return builder_.EndDict();
    }

    //------------ KeyContext ---------------

    KeyContext::KeyContext(Builder& builder)
        : BuildContextFirst(builder) {}

    ValueKeyContext& KeyContext::Value(json::Value value) {
        return builder_.Value(value);
    }

    //------------ ValueKeyContext ---------------

    ValueKeyContext::ValueKeyContext(Builder& builder)
        : BuildContextSecond(builder) {}

    //------------ ValueArrayContext ---------------

    ValueArrayContext::ValueArrayContext(Builder& builder)
        : BuildContextFirst(builder) {}

    ValueArrayContext& ValueArrayContext::Value(json::Value value) {
        return builder_.Value(value);
    }

    Builder& ValueArrayContext::EndArray() {
        return builder_.EndArray();
    }

    //------------ DictContext ---------------

    DictContext::DictContext(Builder& builder)
        : BuildContextSecond(builder) {}

    //------------ ArrayContext ---------------

    ArrayContext::ArrayContext(Builder& builder)
        : ValueArrayContext(builder) {}

    //------------ Builder ---------------

    Builder::Builder()
        : KeyContext(*this)
        , ValueKeyContext(*this)
        , DictContext(*this)
        , ArrayContext(*this) {}

    KeyContext& Builder::Key(string key) {
        if (UnableUseKey()) {
            throw logic_error("Key ñan't be applied"s);
        }
        nodes_.push(make_unique<Node>(key));
        return *this;
    }

    Builder& Builder::Value(json::Value value) {
        if (UnableUseValue()) {
            throw std::logic_error("Value ñan't be applied"s);
        }
        PushNode(value);
        return AddNode(*nodes_.top().release());
    }

    DictContext& Builder::StartDict() {
        if (UnableUseStartDict()) {
            throw logic_error("StartDict ñan't be applied"s);
        }
        nodes_.push(make_unique<Node>(Dict()));
        return *this;
    }

    Builder& Builder::EndDict() {
        if (UnableUseEndDict()) {
            throw logic_error("EndDict ñan't be applied"s);
        }
        return AddNode(*nodes_.top().release());
    }

    ArrayContext& Builder::StartArray() {
        if (UnableUseStartArray()) {
            throw logic_error("StartArray ñan't be applied"s);
        }
        nodes_.push(make_unique<Node>(Array()));
        return *this;
    }

    Builder& Builder::EndArray() {
        if (UnableUseEndArray()) {
            throw logic_error("EndArray ñan't be applied"s);
        }
        return AddNode(*nodes_.top().release());
    }

    Node Builder::Build() const {
        if (UnableUseBuild()) {
            throw logic_error("Builder ñan't be applied"s);
        }
        return root_;
    }

    bool Builder::UnableAdd() const {
        return !(nodes_.empty()
            || nodes_.top()->IsArray()
            || nodes_.top()->IsString());
    }

    bool Builder::IsMakeObj() const {
        return !root_.IsNull();
    }

    bool Builder::UnableUseKey() const {
        return IsMakeObj()
            || nodes_.empty()
            || !nodes_.top()->IsMap();
    }

    bool Builder::UnableUseValue() const {
        return IsMakeObj()
            || UnableAdd();
    }

    bool Builder::UnableUseStartDict() const {
        return UnableUseValue();
    }

    bool Builder::UnableUseEndDict() const {
        return IsMakeObj()
            || nodes_.empty()
            || !nodes_.top()->IsMap();
    }

    bool Builder::UnableUseStartArray() const {
        return UnableUseValue();
    }

    bool Builder::UnableUseEndArray() const {
        return IsMakeObj()
            || nodes_.empty()
            || !nodes_.top()->IsArray();
    }

    bool Builder::UnableUseBuild() const {
        return !IsMakeObj();
    }

    Builder& Builder::AddNode(const Node& node) {
        nodes_.pop();
        if (nodes_.empty()) {
            root_ = node;
        }
        else if (nodes_.top()->IsArray()) {
            nodes_.top()->AsArray().push_back(node);
        }
        else {
            const Node& key = *nodes_.top().release();
            nodes_.pop();
            nodes_.top()->AsMap().emplace(key.AsString(), node);
        }
        return *this;
    }

    void Builder::PushNode(json::Value value) {
        std::visit([this](auto&& val) {
            nodes_.push(make_unique<Node>(val));
            }, value);
    }

}  // namespace json