#include "rational.h"

int main() {
    try {
        // При попытке сконструировать дробь с нулевым знаменателем
        // должно выброситься исключение domain_error
        const Rational invalid_value{ 1, 0 };
        // Следующая строка не должна выполниться
        std::cout << invalid_value << std::endl;
    }
    catch (const std::domain_error& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
}