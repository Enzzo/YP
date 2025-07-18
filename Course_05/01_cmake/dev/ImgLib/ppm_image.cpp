#include "ppm_image.h"

#include <array>
#include <fstream>
#include <string_view>

#include <string>

using namespace std;

namespace img_lib {

    static const string_view PPM_SIG = "P6"sv;
    static const int PPM_MAX = 255;

    // ���������� ��� ������� ��������������
    bool SavePPM(const Path& file, const Image& image) {
        ofstream ofs(file, ios::binary);
        
        if (!ofs.is_open()) {
            return false;
        }
        ofs << PPM_SIG << "\n" << image.GetWidth() << " " << image.GetHeight() << "\n" << PPM_MAX << "\n";

        std::vector<char> buff(image.GetWidth() * 3);
        for (int y = 0; y < image.GetHeight(); ++y) {
            for (int x = 0; x < image.GetWidth(); ++x) {                
                buff[x * 3 + 0] = static_cast<char>(image.GetPixel(x, y).r);
                buff[x * 3 + 1] = static_cast<char>(image.GetPixel(x, y).g);
                buff[x * 3 + 2] = static_cast<char>(image.GetPixel(x, y).b);
            }
            if (!ofs.write(buff.data(), image.GetWidth() * 3)) {
                return false;
            }
        }
        return true;
    }

    Image LoadPPM(const Path& file) {
        // ��������� ����� � ������ ios::binary
        // ��������� ����� ������ ����� � �������� �������
        ifstream ifs(file, ios::binary);
        std::string sign;
        int w, h, color_max;

        // ������ ���������: �� �������� ������, ������� �����������
        // � ������������ �������� �����
        ifs >> sign >> w >> h >> color_max;

        // �� ������������ ����������� ������ ������� P6
        // � ������������ ��������� ����� 255
        if (sign != PPM_SIG || color_max != PPM_MAX) {
            return {};
        }

        // ���������� ���� ���� - ��� ����� ������
        const char next = ifs.get();
        if (next != '\n') {
            return {};
        }

        Image result(w, h, Color::Black());
        std::vector<char> buff(w * 3);

        for (int y = 0; y < h; ++y) {
            Color* line = result.GetLine(y);
            ifs.read(buff.data(), w * 3);

            for (int x = 0; x < w; ++x) {
                line[x].r = static_cast<byte>(buff[x * 3 + 0]);
                line[x].g = static_cast<byte>(buff[x * 3 + 1]);
                line[x].b = static_cast<byte>(buff[x * 3 + 2]);
            }
        }

        return result;
    }

}  // namespace img_lib
