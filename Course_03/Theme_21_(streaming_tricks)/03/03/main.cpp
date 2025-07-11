#include "log_duration.h"

#include <iostream>

using namespace std::literals;

class StreamUntier {
    std::ostream* tied_before_;
    std::istream& stream_;

public:
    // �������� �����������, ����������
    // � �������������� ���� ������ ��� �������������
    StreamUntier(std::istream& stream) : stream_(stream) {
        tied_before_ = stream_.tie(nullptr);
    };

    ~StreamUntier() {
        stream_.tie(tied_before_);
    };
};

int main() {
    LOG_DURATION("\\n with tie"s);

    StreamUntier guard(std::cin);
    int i;
    while (std::cin >> i) {
        std::cout << i * i << "\n"s;
    }

    return 0;
}