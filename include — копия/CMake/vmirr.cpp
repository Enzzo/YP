#include "ppm_image.h"

#include <algorithm>
#include <iostream>
#include <string_view>

using namespace std;

//реализуйте вертикальное отражение
void VMirrInplace(img_lib::Image& image);

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
        return 1;
    }

    auto image = img_lib::LoadPPM(argv[1]);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    VMirrInplace(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}

void VMirrInplace(img_lib::Image& image) {
    for (int y = 0; y < image.GetHeight() / 2; ++y) {
        int bottom_index = image.GetHeight() - y - 1;

        if (y != bottom_index) {
            img_lib::Color* top_row = image.GetLine(y);
            img_lib::Color* bottom_row = image.GetLine(bottom_index);
            //std::swap(top_row, bottom_row);
            std::swap_ranges(top_row, top_row + image.GetWidth(), bottom_row);
        }
    }
}

