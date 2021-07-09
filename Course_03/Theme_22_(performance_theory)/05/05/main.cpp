#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// функция возвращает true, если векторы из одинаковых элементов
// перепишите эту функцию, улучшив её асимптотическую сложность
bool TestPermut(const vector<int>& v1, const vector<int>& v2) {
    // если они разной длины, элементы заведомо разные
    if (v1.size() != v2.size()) {
        return false;
    }
    vector<int> v1_ = v1;
    vector<int> v2_ = v2;

    sort(v1_.begin(), v1_.end());
    sort(v2_.begin(), v2_.end());
    return v1_ == v2_;
    /*
    for (int i : v1) {
        // проверяем, что каждый элемент первого вектора
        // содержится одинаковое количество раз в обоих векторах
        if (count(v1.begin(), v1.end(), i) != count(v2.begin(), v2.end(), i)) {
            return false;
        }
    }
    */
    //return true;
}

int main() {
    int n;
    cin >> n;
    vector<int> v1, v2;
    v1.reserve(n);
    v2.reserve(n);

    for (int i = 0; i < n; ++i) {
        v1.push_back(rand());
        v2.push_back(rand());
    }

    // оба вектора случайны, вряд ли они совпадут
    cout << "Random vectors match? "s << flush;
    cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;

    // делаем один перестановкой другого явным образом
    v2 = v1;
    random_shuffle(v2.begin(), v2.end());
    cout << "Permuted vectors match? "s << flush;
    cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;
}