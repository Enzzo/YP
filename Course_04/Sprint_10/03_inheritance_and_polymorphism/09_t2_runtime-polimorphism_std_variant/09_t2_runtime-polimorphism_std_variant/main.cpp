#include "json.h"

#include <cassert>
#include <chrono>
#include <sstream>
#include <string_view>

using namespace json;
using namespace std::literals;

namespace {

    json::Document LoadJSON(const std::string& s) {
        std::istringstream strm(s);
        return json::Load(strm);
    }

    // ���������������� ��� ������� �� ���� ����, ��� ���������� ����������� ����������
    
    std::string Print(const Node& node) {
        std::ostringstream out;
        Print(Document{node}, out);
        return out.str();
    }
    
    void MustFailToLoad(const std::string& s) {
        try {
            LoadJSON(s);
            std::cerr << "ParsingError exception is expected on '"sv << s << "'"sv << std::endl;
            assert(false);
        } catch (const json::ParsingError&) {
            // ok
        } catch (const std::exception& e) {
            std::cerr << "exception thrown: "sv << e.what() << std::endl;
            assert(false);
        } catch (...) {
            std::cerr << "Unexpected error"sv << std::endl;
            assert(false);
        }
    }
    template <typename Fn>
    void MustThrowLogicError(Fn fn) {
        try {
            fn();
            std::cerr << "logic_error is expected"sv << std::endl;
            assert(false);
        } catch (const std::logic_error&) {
            // ok
        } catch (const std::exception& e) {
            std::cerr << "exception thrown: "sv << e.what() << std::endl;
            assert(false);
        } catch (...) {
            std::cerr << "Unexpected error"sv << std::endl;
            assert(false);
        }
    }

    void TestNull() {
        Node null_node;
        assert(null_node.IsNull());

        Node null_node1{nullptr};
        assert(null_node1.IsNull());

        assert(Print(null_node) == "null"s);

        const Node node = LoadJSON("null"s).GetRoot();
        assert(node.IsNull());
        assert(node == null_node);
    }
    void TestNumbers() {
        Node int_node{42};
        assert(int_node.IsInt());
        assert(int_node.AsInt() == 42);
        // ����� ����� �������� ������������� ����� � ��������� �������
        assert(int_node.IsDouble());
        // ����� ���� ������ int, ����� �������� ��������������� ��� double-��������
        assert(int_node.AsDouble() == 42.0);
        assert(!int_node.IsPureDouble());

        Node dbl_node{123.45};
        assert(dbl_node.IsDouble());
        assert(dbl_node.AsDouble() == 123.45);
        assert(dbl_node.IsPureDouble());  // �������� �������� ����� � ��������� �������
        assert(!dbl_node.IsInt());

        assert(Print(int_node) == "42"s);
        assert(Print(dbl_node) == "123.45"s);
        auto x = LoadJSON("null"s).GetRoot();
        assert(LoadJSON("42"s).GetRoot() == int_node);
        assert(LoadJSON("123.45"s).GetRoot() == dbl_node);
        assert(LoadJSON("0.25"s).GetRoot().AsDouble() == 0.25);
        assert(LoadJSON("3e5"s).GetRoot().AsDouble() == 3e5);
        assert(LoadJSON("1.2e-5"s).GetRoot().AsDouble() == 1.2e-5);
        assert(LoadJSON("1.2e+5"s).GetRoot().AsDouble() == 1.2e5);
        assert(LoadJSON("-123456"s).GetRoot().AsInt() == -123456);
    }
    
    void TestStrings() {
        Node str_node{"Hello, \"everybody\""s};
        assert(str_node.IsString());
        assert(str_node.AsString() == "Hello, \"everybody\""s);

        assert(!str_node.IsInt());
        assert(!str_node.IsDouble());

        assert(Print(str_node) == "\"Hello, \\\"everybody\\\"\""s);
        assert(LoadJSON(Print(str_node)).GetRoot() == str_node);
    }

    void TestBool() {
        Node true_node{true};
        assert(true_node.IsBool());
        assert(true_node.AsBool());

        Node false_node{false};
        assert(false_node.IsBool());
        assert(!false_node.AsBool());

        assert(Print(true_node) == "true"s);
        assert(Print(false_node) == "false"s);

        assert(LoadJSON("true"s).GetRoot() == true_node);
        assert(LoadJSON("false"s).GetRoot() == false_node);
    }
    
    void TestArray() {
        Node arr_node{Array{1, 1.23, "Hello"s}};
        assert(arr_node.IsArray());
        const Array& arr = arr_node.AsArray();
        assert(arr.size() == 3);
        assert(arr.at(0).AsInt() == 1);

        assert(LoadJSON("[1, 1.23, \"Hello\"]"s).GetRoot() == arr_node);
        assert(LoadJSON(Print(arr_node)).GetRoot() == arr_node);
    }
    
    void TestMap() {
        Node dict_node{Dict{{"key1"s, "value1"s}, {"key2"s, 42}}};
        assert(dict_node.IsMap());
        const Dict& dict = dict_node.AsMap();
        assert(dict.size() == 2);
        assert(dict.at("key1"s).AsString() == "value1"s);
        assert(dict.at("key2"s).AsInt() == 42);

        assert(LoadJSON("{ \"key1\": \"value1\", \"key2\": 42 }"s).GetRoot() == dict_node);
        assert(LoadJSON(Print(dict_node)).GetRoot() == dict_node);
    }

    void TestErrorHandling() {
        MustFailToLoad("["s);
        MustFailToLoad("]"s);

        MustFailToLoad("{"s);
        MustFailToLoad("}"s);

        MustFailToLoad("\"hello"s);  // ���������� �������

        MustFailToLoad("tru"s);
        MustFailToLoad("fals"s);
        MustFailToLoad("nul"s);

        Node dbl_node{3.5};
        MustThrowLogicError([&dbl_node] {
            dbl_node.AsInt();
        });
        MustThrowLogicError([&dbl_node] {
            dbl_node.AsString();
        });
        MustThrowLogicError([&dbl_node] {
            dbl_node.AsArray();
        });

        Node array_node{Array{}};
        MustThrowLogicError([&array_node] {
            array_node.AsMap();
        });
        MustThrowLogicError([&array_node] {
            array_node.AsDouble();
        });
        MustThrowLogicError([&array_node] {
            array_node.AsBool();
        });
    }
    void Benchmark() {
        const auto start = std::chrono::steady_clock::now();
        Array arr;
        arr.reserve(1'000);
        for (int i = 0; i < 1'000; ++i) {
            arr.emplace_back(Dict{
                {"int"s, 42},
                {"double"s, 42.1},
                {"null"s, nullptr},
                {"string"s, "hello"s},
                {"array"s, Array{1, 2, 3}},
                {"bool"s, true},
                {"map"s, Dict{{"key"s, "value"s}}},
            });
        }
        std::stringstream strm;
        json::Print(Document{arr}, strm);
        const auto doc = json::Load(strm);
        assert(doc.GetRoot() == arr);
        const auto duration = std::chrono::steady_clock::now() - start;
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms"sv << std::endl;
    }
    }  // namespace
    
int main() {
    TestNull();
    TestNumbers();    
    TestStrings();
    TestBool();
    TestArray();
    TestMap();
    TestErrorHandling();

    Benchmark();
    
    return 0;
}