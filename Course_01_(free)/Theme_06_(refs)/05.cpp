Хитрости константных ссылок
В работе с константными ссылками есть особенности. Вспомните решение задачи «Очевидцы»:

void UpdateIfGreater(int limit, int& value) {
        if (limit > value) {
                value = limit;
        }
} 

Целочисленный параметр limit здесь передаётся по значению. На это есть причины:

    Параметр limit — «лёгкий» объект, его не жалко скопировать.
    Если бы limit был ссылкой int&, не получилось бы передать туда литерал: UpdateIfGreater(50, value).

С лёгкостью всё прозаично: bool, char, int и другие простые типы передаются по значению. Строки и всевозможные контейнеры — по константной ссылке.
Проблема с литералом есть у всех неконстантных ссылок. Нельзя написать int& ref = 50;, так как литерал 50 нельзя изменить. Ошибка компиляции будет такой:

error: cannot bind non-const lvalue reference of type 'int&' to an rvalue of type 'int' 

Так происходит, потому что non-const lvalue reference of type 'int&' — неконстантная ссылка, а rvalue of type 'int' — временный объект, к которому ссылку не привяжешь.
Точно так же нельзя привязать ссылку на строку к строковому литералу "..."s:

#include <string>

using namespace std;

void AddLocation(string& query) {
        query += " near central station"s;
}

int main() {
        AddLocation("white cat"s);
} 

Ошибка будет похожей, но с поправкой на громоздкость настоящего названия типа string:

cannot bind non-const lvalue reference of type 'std::string&' {aka 'std::__cxx11::basic_string<char>&'} to an rvalue of type 'std::__cxx11::basic_string<char>' 

Та же неприятность возникнет, если пытаться передать в UpdateSearchQuery строчку, только что полученную из другой функции:

#include <iostream>
#include <string>

using namespace std;

void UpdateSearchQuery(string& query) {
        query += " free download";
}

string ReadSearchQuery() {
        string query;
        getline(cin, query);
        return query;
}

int main() {
        UpdateSearchQuery(ReadSearchQuery());
} 

Невозможно поменять объект, у которого нет названия, ведь его не положили в переменную. С константными ссылками таких проблем не бывает. Они связаны не с изменением объекта, а с передачей без копирования.