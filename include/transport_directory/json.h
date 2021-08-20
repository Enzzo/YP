#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>
#include <stack>
#include <memory>

namespace json {

    class Node;
    using Dict = std::map<std::string, Node>;
    using Array = std::vector<Node>;

    // Эта ошибка должна выбрасываться при ошибках парсинга JSON
    class ParsingError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    using Value = std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>;

    class Node{
        Value content_;
/*        Array as_array_;
        Dict as_map_;
        int as_int_ = 0;
        std::string as_string_;    */    

    public:
        Node();
        Node(std::nullptr_t);
        Node(Array array);
        Node(Dict map);
        Node(bool b);
        Node(int value);
        Node(double value);
        Node(std::string value);

        //Node(const Value& content = nullptr) : content_(content){}

        bool IsInt() const;
        int AsInt() const;

        bool IsPureDouble() const;
        bool IsDouble() const;
        double AsDouble() const;

        bool IsBool() const;
        bool AsBool() const;
        
        bool IsString() const;
        const std::string& AsString() const;

        bool IsMap() const;

        const Dict& AsMap() const;
        Dict& AsMap();

        bool IsNull() const;

        bool IsArray() const;

        const Array& AsArray() const;
        Array& AsArray();

        const Value& GetValue() const;

        bool operator==(const Node&) const;
        bool operator!=(const Node&) const;
    };

    

    class Document {
    public:
        explicit Document(Node root);

        const Node& GetRoot() const;

        bool operator==(const Document&) const;
        bool operator!=(const Document&) const;

    private:
        Node root_;
    };

    Document Load(std::istream& input);

    void Print(const Document& doc, std::ostream& output);

    class BuildConstructor;
    class BuildContextFirst;
    class BuildContextSecond;
    class KeyContext;
    class ValueKeyContext;
    class ValueArrayContext;
    class DictContext;
    class ArrayContext;
    class Builder;

    //------------ BuildConstructor ---------------

    class BuildConstructor {
    public:
        explicit BuildConstructor(Builder& builder);

    protected:
        Builder& builder_;
    };

    //------------ BuildContextFirst ---------------

    class BuildContextFirst : public BuildConstructor {
    public:
        explicit BuildContextFirst(Builder& builder);

        DictContext& StartDict();

        ArrayContext& StartArray();
    };

    //------------ BuildContextSecond ---------------

    class BuildContextSecond : public BuildConstructor {
    public:
        explicit BuildContextSecond(Builder& builder);

        KeyContext& Key(std::string key);

        Builder& EndDict();
    };

    //------------ KeyContext ---------------

    class KeyContext : public BuildContextFirst {
    public:
        explicit KeyContext(Builder& builder);

        ValueKeyContext& Value(json::Value value);
    };

    //------------ ValueKeyContext ---------------

    class ValueKeyContext : public BuildContextSecond {
    public:
        explicit ValueKeyContext(Builder& builder);
    };

    //------------ ValueArrayContext ---------------

    class ValueArrayContext : public BuildContextFirst {
    public:
        explicit ValueArrayContext(Builder& builder);

        ValueArrayContext& Value(json::Value value);

        Builder& EndArray();
    };

    //------------ DictContext ---------------

    class DictContext : public BuildContextSecond {
    public:
        explicit DictContext(Builder& builder);
    };

    //------------ ArrayContext ---------------

    class ArrayContext : public ValueArrayContext {
    public:
        explicit ArrayContext(Builder& builder);
    };

    //------------ Builder ---------------

    class Builder final : virtual public KeyContext, virtual public ValueKeyContext,
        virtual public DictContext, virtual public ArrayContext {
    public:
        Builder();

        KeyContext& Key(std::string key);

        Builder& Value(json::Value value);

        DictContext& StartDict();

        Builder& EndDict();

        ArrayContext& StartArray();

        Builder& EndArray();

        Node Build() const;

    private:
        bool UnableAdd() const;

        bool IsMakeObj() const;

        bool UnableUseKey() const;

        bool UnableUseValue() const;

        bool UnableUseStartDict() const;

        bool UnableUseEndDict() const;

        bool UnableUseStartArray() const;

        bool UnableUseEndArray() const;

        bool UnableUseBuild() const;

        Builder& AddNode(const Node& node);

        void PushNode(json::Value value);

    private:
        Node root_ = nullptr;
        std::stack<std::unique_ptr<Node>> nodes_;
    };

}  // namespace json