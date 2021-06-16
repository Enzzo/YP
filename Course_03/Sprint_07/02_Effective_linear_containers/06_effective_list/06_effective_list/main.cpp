#include <string>
#include <iostream>
#include <cmath>
#include <cassert>
#include <list>

using namespace std::literals;

#include "editor.h"

int main() {
    {
        Editor test;
        const std::string text = "a";
        test.Insert('a');
        assert(test.GetText() == "a");
    }
    {
        Editor test;
        const std::string text = "ab";
        for (char c : text) {
            test.Insert(c);
        }
        test.Insert('c');
        assert(test.GetText() == "abc");
    }
    {
        Editor test;
        const std::string text = "bc";
        for (char c : text) {
            test.Insert(c);
        }
        test.Left(); test.Left();
        test.Insert('a');
        assert(test.GetText() == "abc");
    }
    {
        Editor test;
        std::string text = "world";
        for (char c : text) {
            test.Insert(c);
        }
        test.Left(); test.Left(); test.Left(); test.Left(); test.Left();
        text = "hello_";
        for (char c : text) {
            test.Insert(c);
        }
        assert(test.GetText() == "hello_world");
    }
    {
        Editor test;
        std::string text = "hello_";
        for (char c : text) {
            test.Insert(c);
        }
        
        text = "world";
        for (char c : text) {
            test.Insert(c);
        }
        assert(test.GetText() == "hello_world");
    }
    {
        Editor test;
        std::string text = "12378";
        for (char c : text) {
            test.Insert(c);
        }
        test.Left(); test.Left();

        text = "456";
        for (char c : text) {
            test.Insert(c);
        }
        assert(test.GetText() == "12345678");
    }
    Editor editor;
    const std::string text = "hello, world"s;
    for (char c : text) {
        editor.Insert(c);
    }
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    std::cout << editor.GetText();
    return 0;
}