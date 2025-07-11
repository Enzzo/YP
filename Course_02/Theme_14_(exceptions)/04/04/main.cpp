#include "Rational.h"

int main() {
    try {
        const Rational three_fifth{ 3, 5 };
        const Rational zero;
        std::cout << three_fifth << " / " << zero << " = " << (three_fifth / zero) << std::endl;
    }
    
    catch (const std::invalid_argument& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }

    try {
        Rational value{ 3, 5 };
        value /= Rational();
        // Следующая строка не должна выполниться
        std::cout << value << std::endl;
    }

    catch (const std::invalid_argument& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}