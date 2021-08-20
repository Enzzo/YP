#pragma once
#include "texture.h"

#include <memory>

// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType { RECTANGLE, ELLIPSE };

class Shape {
    ShapeType type_;
    Point pos_;
    Size size_;
    std::shared_ptr<Texture> texture_;

public:
    // Фигура после создания имеет нулевые координаты и размер,
    // а также не имеет текстуры
    explicit Shape(ShapeType type):type_(type) {}

    void SetPosition(Point pos) {
        pos_ = pos;
    }

    void SetSize(Size size) {
        size_ = size;
    }

    void SetTexture(std::shared_ptr<Texture> texture) {
        texture_ = std::move(texture);
    }

    // Рисует фигуру на указанном изображении
    // В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
    // Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
    // должны отображаться с помощью символа точка '.'
    // Части фигуры, выходящие за границы объекта image, должны отбрасываться.
    void Draw(Image& image) const {
        Size image_size = GetImageSize(image);
        if (type_ == ShapeType::RECTANGLE) {
            for (int x = pos_.x, w = 0; w < size_.width; ++x, ++w) {
                for (int y = pos_.y, h = 0; h < size_.height; ++y, ++h) {
                    if (texture_ == nullptr) {
                        image[y][x] = '.';
                    }
                    else if ((x < image_size.width && y < image_size.height)) {
                        image[y][x] = texture_->GetPixelColor({ w, h });
                    }
                }
            }
        }
        else {
            for (int x = pos_.x, w = 0; w < size_.width; ++x, ++w) {
                for (int y = pos_.y, h = 0; h < size_.height; ++y, ++h) {
                    if (texture_ == nullptr) {
                        image[y][x] = '.';
                    }
                    else if ((x < image_size.width && y < image_size.height) && IsPointInEllipse({ w, h }, size_)) {
                        image[y][x] = texture_->GetPixelColor({ w, h });
                    }
                }
            }
        }
    }
};