#include <iostream>
#include <string>

using namespace std;

struct Circle {
    double x;
    double y;
    double r;
};

struct Dumbbell {
    Circle circle1;
    Circle circle2;
    string text;
};

struct CircleHash {
    size_t operator() (const Circle& circle) {
        std::hash<double> hash;

        const size_t x = hash(circle.x) * (37 ^ 3);
        const size_t y = hash(circle.y) * (37 ^ 4);
        const size_t r = hash(circle.r) * (37 ^ 5);
        return x + y + r;
    }
};

struct DumbbellHash {
    size_t operator()(const Dumbbell& dumbbell) {
        std::hash<std::string> s_hash;
        CircleHash circle_hash;
        const size_t h0 = s_hash(dumbbell.text);
        const size_t h1 = circle_hash(dumbbell.circle1);
        const size_t h2 = circle_hash(dumbbell.circle2) * (37 ^ 3);

        return h0 + h1 + h2;
    }
};

int main() {
    DumbbellHash hash;
    Dumbbell dumbbell{ {1000000, 1000000, 2.3}, {3.14, 15, -8}, "abc"s };
    cout << "Dumbbell hash "s << hash(dumbbell);
}