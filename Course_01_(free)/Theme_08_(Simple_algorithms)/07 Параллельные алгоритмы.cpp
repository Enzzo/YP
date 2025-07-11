Параллельные алгоритмы
Алгоритмы из стандартной библиотеки С++ упрощают работу программиста и повышают эффективность кода. Но есть способ ещё увеличить эффективность — делать несколько дел одновременно, а потом объединить результат. В С++ для этого подключают библиотеку <execution> и выполняют алгоритмы параллельно.
У алгоритма accumulate из библиотеки numeric есть близнец — алгоритм reduce, который поддерживает параллельное выполнение. Чтобы использовать reduce, передайте ему первым параметром объект execution::par — политику исполнения, объявленную в <execution>. Это запустит параллельную работу, и код станет быстрее.
Сравните время суммирования вектора чисел для алгоритмов accumulate и reduce. Для этого надо подключить библиотеку <chrono>:

#include <iostream>
#include <chrono> //эта библиотека отвечает за концепты, связанные со временем
#include <vector>
#include <numeric>
#include <execution>

using namespace std;
 
int main()
{
    std::vector<int> v(200'000'001, 1);
 
    const auto startTime = chrono::high_resolution_clock::now();
    int result = std::accumulate(v.begin(), v.end(), 0);
    const auto endTime = chrono::high_resolution_clock::now();
    cout << "std::accumulate execution time: "s << chrono::duration_cast<chrono::duration<int, milli>>(endTime - startTime).count() << " ms."s << endl;

    const auto startTimeReduce = chrono::high_resolution_clock::now();
    int resultReduce = std::reduce(execution::par, v.begin(), v.end(), 0);
    const auto endTimeReduce = chrono::high_resolution_clock::now();
    cout << "std::reduce execution time: "s << chrono::duration_cast<chrono::duration<int, milli>>(endTimeReduce - startTimeReduce).count() << " ms."s << endl;
} 

reduce работает быстрее:

std::accumulate execution time: 1646 ms.
std::reduce execution time: 708 ms. 

Но у reduce есть ограничения. Он подходит только для коммутативных и ассоциативных операций — то есть для тех, которые не меняют результат при перестановке аргументов. Пример такой операции — сложение чисел.
Параллельной бывает и сортировка. Чтобы её запустить, функции sort первым параметром передают политику исполнения — execution::par. Сравним обычную и параллельную версию сортировки из стандартной библиотеки:

#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <ratio>
#include <vector>
#include <execution>

using namespace std;

const size_t testSize = 1'000'000;
const int iterationCount = 5;

int main() {
    random_device rd;

    cout << "Testing with "s << testSize << " integers..."s << endl;
    vector<int> integers(testSize);
    for (auto& i : integers) {
        i = static_cast<int>(rd());
    }

    for (int i = 0; i < iterationCount; ++i)
    {
        vector<int> sorted(integers);
        const auto startTime = chrono::high_resolution_clock::now();
        sort(sorted.begin(), sorted.end());
        const auto endTime = chrono::high_resolution_clock::now();
        cout << "Sequential execution time: "s << chrono::duration_cast<chrono::duration<int, milli>>(endTime - startTime).count() << " ms."s << endl;
    }

    for (int i = 0; i < iterationCount; ++i)
    {
        vector<int> sorted(integers);
        const auto startTime = chrono::high_resolution_clock::now();
        sort(execution::par, sorted.begin(), sorted.end());
        const auto endTime = chrono::high_resolution_clock::now();
        cout << "Parallel execution time: "s << chrono::duration_cast<chrono::duration<int, milli>>(endTime - startTime).count() << " ms."s << endl;
    }
} 

Получится:

Testing with 1000000 integers...
Sequential execution time: 358 ms.
Sequential execution time: 387 ms.
Sequential execution time: 359 ms.
Sequential execution time: 360 ms.
Sequential execution time: 360 ms.
Parallel execution time: 80 ms.
Parallel execution time: 81 ms.
Parallel execution time: 89 ms.
Parallel execution time: 112 ms.
Parallel execution time: 125 ms. 

Параллельная сортировка, как и другие параллельные алгоритмы, экономят время разработчика и пользователя.