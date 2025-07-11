#pragma once
#include "common.h"

class Texture {
public:
    explicit Texture(Image image)
        : image_(std::move(image)) {
    }

    Size GetSize() const {
        return GetImageSize(image_);
    }

    char GetPixelColor(Point p) const {
        Size size = GetSize();

        if (image_.empty() || size.height <= p.y || size.width <= p.x) {
            return '.';
        }

        return image_[p.y][p.x];
    }

private:
    Image image_;
};