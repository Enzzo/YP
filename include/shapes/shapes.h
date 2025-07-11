#pragma once
#include "texture.h"

#include <memory>

// �������������� ���� �����: ������������� � ������
enum class ShapeType { RECTANGLE, ELLIPSE };

class Shape {
    ShapeType type_;
    Point pos_;
    Size size_;
    std::shared_ptr<Texture> texture_;

public:
    // ������ ����� �������� ����� ������� ���������� � ������,
    // � ����� �� ����� ��������
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

    // ������ ������ �� ��������� �����������
    // � ����������� �� ���� ������ ������ ���������� ���� ������, ���� �������������
    // ������� ������, ��������� �� ������� ��������, � ����� � ������, ����� �������� �� ������,
    // ������ ������������ � ������� ������� ����� '.'
    // ����� ������, ��������� �� ������� ������� image, ������ �������������.
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