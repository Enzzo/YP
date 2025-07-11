#include <cstdint>
#include <iostream>

using namespace std;

// ��������� ��� ���������������� �������,
// ���������� �������� ��������
/*
int64_t T(int i) {
    if (i <= 1) {
        return 0;
    }
    if (i == 2) {
        return 1;
    }

    return T(i - 1) + T(i - 2) + T(i - 3);
}
*/
int64_t T(int i) {
    
    if (i < 2) {
        return 0;
    }
    if (i < 3) {
        return 1;
    }

    uint64_t prev0 = 0, prev1 = 0, prev2 = 1;

    for (int t = 2; t < i; ++t) {
        uint64_t next = prev0 + prev1 + prev2;
        prev0 = prev1;
        prev1 = prev2;
        prev2 = next;
    }

    return prev2;
}

int main() {
    int i;

    while (true) {
        cout << "Enter index: "s;
        if (!(cin >> i)) {
            break;
        }

        cout << "Ti = "s << T(i) << endl;
    }
}