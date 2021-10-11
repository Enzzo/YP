#include "ppm_image.h"

#include <iostream>
#include <string_view>

using namespace std;

int main(int argc, const char** argv) {
    //if (argc != 3) {
    //    cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
    //    return 1;
    //}

    //const auto image = img_lib::LoadPPM(argv[1]);
    //if (!image) {
    //    cerr << "Error loading image"sv << endl;
    //    return 2;
    //}

    //if (!img_lib::SavePPM(argv[2], image)) {
    //    cerr << "Error saving image"sv << endl;
    //    return 3;
    //}

    //cout << "Image saved successfully!"sv << endl;

    const img_lib::Image image = img_lib::LoadPPM("1.kissy++.ppm");
    img_lib::SavePPM("2.kissy++.ppm", image);
    return 0;
}
