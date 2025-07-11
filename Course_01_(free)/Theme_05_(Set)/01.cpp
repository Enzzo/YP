Как работают множества
Сохранить массив ключевых слов для поиска можно в векторе строк vector<string> words. Чтобы проверить наличие конкретного слова, нужно пройтись по вектору циклом range-based for и вернуть результат:

bool found = false;
for (string word : words) {
    if (word == word_to_find) {
        found = true;
    }
} 

Но представьте, как обидно потратить время на просмотр всех элементов вектора и не найти нужный. К счастью, в С++ есть контейнер для быстрого поиска объектов. Он называется set.
set или множество — это упорядоченный набор уникальных элементов. Фиксированный порядок гарантирует, что вы быстро найдёте конкретное слово в массиве ключевых слов поиска. Взять хотя бы приведённый выше пример:

bool found = false;
for (string word : words) {
    if (word == word_to_find) {
        found = true;
    }
} 

Вместо цикла достаточно написать bool found = words.count(word_to_find) — короче и эффективнее.
Создание множества синтаксически похоже на создание вектора. Чтобы объединить во множество животных, в том числе трубкозуба, подключите библиотеку <set>, укажите тип элементов множества и перечислите элементы в фигурных скобках:

#include <iostream>
#include <set>

using namespace std;

void PrintSet(set<string> s) {
    for (string x : s) {
        cout << x << endl;
    }
}

int main() {
    set<string> animals = {"cat"s, "dog"s, "aardvark"s, "dog"s, "sheep"s, "ape"s, "sheep"s};
    PrintSet(animals);

    return 0;
} 

Программа выведет:

aardvark
ape
cat
dog
sheep 

Имена животных при выводе содержимого set упорядочены по возрастанию, а не в порядке их упоминания в фигурных скобках. В данном случае — по алфавиту. Все дубликаты пропали.
Как вектор и строка, множество знает свой размер. Чтобы проверить множество на пустоту, примените метод empty(). Он возвращает значение типа bool:

//...

int main() {
    set<string> animals = {"cat"s, "dog"s, "aardvark"s, "dog"s, "sheep"s, "ape"s, "sheep"s};
    if (!animals.empty()) {
        cout << "There are some animals in the set,"s << endl;
        cout << animals.size() << " different animals, to be exact."s;
    }

    return 0;
} 

Программа выведет:

There are some animals in the set,
5 different animals, to be exact. 

Для добавления новых животных в список используйте insert (). Порядок элементов в списке сохранится:

// ...

int main() {
    set<string> animals = {"cat"s, "dog"s, "aardvark"s, "dog"s, "sheep"s, "ape"s, "sheep"s};
    PrintSet(animals);
    cout << endl;
    animals.insert("another dog"s);
    animals.insert("cat"s);
    animals.insert("penguin"s);
    PrintSet(animals);

    return 0;
} 

Программа выведет:

aardvark
ape
cat
dog
sheep

aardvark
another dog
ape
cat
dog
penguin
sheep 

Добавленные животные заняли свои места в алфавитном порядке. Повторное добавление кошки проигнорировано — это имя уже есть во множестве.
Чтобы удалить элементы из set, вызовите метод erase():

// ...

int main() {
    set<string> animals = {"cat"s, "dog"s, "aardvark"s, "dog"s, "sheep"s, "ape"s, "sheep"s};
    PrintSet(animals);
    cout << endl << "Some animals are gone!"s << endl << endl;
    animals.erase("cat"s);
    animals.erase("dog"s);
    animals.erase("ape"s);
    PrintSet(animals);

    return 0;
} 

Результат:

aardvark
ape
cat
dog
sheep

Some animals are gone!

aardvark
sheep 

Контейнер set автоматически сортирует элементы и удаляет дубликаты. Поэтому часто он эффективнее, чем вектор.