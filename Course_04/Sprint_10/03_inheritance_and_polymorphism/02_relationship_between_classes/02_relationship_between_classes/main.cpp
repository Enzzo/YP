#include <cassert>
#include <string>
#include <stdexcept>
#include <exception>
#include <iostream>

class House {
    int length_;
    int width_;
    int height_;

public:
    explicit House(int length, int width, int height) : length_(length), width_(width), height_(height) {}

    [[nodiscard]] inline int GetLength() const noexcept{
        return length_;
    }

    [[nodiscard]] inline int GetWidth() const noexcept {
        return width_;
    }

    [[nodiscard]] inline int GetHeight() const noexcept {
        return height_;
    }
};

class Resources {
    int brick_count_;
public:
    Resources() = delete;
    explicit Resources(const int brick_count) : brick_count_(brick_count) {}

    void TakeBricks(const int count) {
        if (count < 0 || brick_count_ - count < 0) {
            throw std::out_of_range("");
        }
        brick_count_ -= count;
    }

    [[nodiscard]] inline int GetBrickCount() const noexcept {
        return brick_count_;
    }
};

struct HouseSpecification {
    int length = 0;
    int width = 0;
    int height = 0;
};

class Builder {
    Resources& resources_;
public:
    Builder() = delete;
    explicit Builder(Resources& resources) : resources_(resources) {}

    const House& BuildHouse(const HouseSpecification& spec) try {
        int s = spec.length * spec.height * 64 + spec.width * spec.height * 64;
        resources_.TakeBricks(s);
        return *(new House(spec.length, spec.width, spec.height));
    }
    catch (std::out_of_range const&) {
        throw std::runtime_error("runtime error");
    }
};

int main() try {
    Resources resources{ 10000 };
    Builder builder1{ resources };
    Builder builder2{ resources };

    using namespace std::literals;

    House house1 = builder1.BuildHouse(HouseSpecification{ 12, 9, 3 });
    assert(house1.GetLength() == 12);
    assert(house1.GetWidth() == 9);
    assert(house1.GetHeight() == 3);
    std::cout << resources.GetBrickCount() << " bricks left"s << std::endl;

    House house2 = builder2.BuildHouse(HouseSpecification{ 8, 6, 3 });
    assert(house2.GetLength() == 8);
    std::cout << resources.GetBrickCount() << " bricks left"s << std::endl;

    House banya = builder1.BuildHouse(HouseSpecification{ 4, 3, 2 });
    assert(banya.GetHeight() == 2);
    std::cout << resources.GetBrickCount() << " bricks left"s << std::endl;
}
catch (std::runtime_error const&) {
}