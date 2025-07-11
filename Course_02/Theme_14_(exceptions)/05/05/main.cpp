#include "rational.h"

int main() {
    try {
        // ��� ������� ��������������� ����� � ������� ������������
        // ������ ����������� ���������� domain_error
        const Rational invalid_value{ 1, 0 };
        // ��������� ������ �� ������ �����������
        std::cout << invalid_value << std::endl;
    }
    catch (const std::domain_error& e) {
        std::cout << "������: " << e.what() << std::endl;
    }
}