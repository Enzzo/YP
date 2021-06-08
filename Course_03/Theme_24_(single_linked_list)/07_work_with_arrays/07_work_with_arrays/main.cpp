#include <cassert>
#include <vector>

template <typename T>
void ReverseArray(T* start, size_t size) {
    if (size > 1) {
        T* begin = start;
        T* end = start + size - 1;
        int x = 2;
    }
}

int main() {
    using namespace std;

    vector<int> v = { 1, 2 };
    ReverseArray(v.data(), v.size());
    assert(v == (vector<int>{2, 1}));
}