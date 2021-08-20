#define _USE_MATH_DEFINES
#include "svg.h"

#include <cmath>
#include <fstream>

namespace shapes {

    //-------------------- FUNCTIONS ----------------------
    svg::Polyline CreateStar(svg::Point center, double outer_rad, double inner_rad, int num_rays) {
        using namespace svg;
        Polyline polyline;
        for (int i = 0; i <= num_rays; ++i) {
            double angle = 2 * M_PI * (i % num_rays) / num_rays;
            polyline.AddPoint({ center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle) });
            if (i == num_rays) {
                break;
            }
            angle += M_PI / num_rays;
            polyline.AddPoint({ center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle) });
        }
        return polyline;
    }

    //----------------- CLASSES -----------------------
    class Triangle : public svg::Drawable {
    public:
        Triangle(svg::Point p1, svg::Point p2, svg::Point p3)
            : p1_(p1)
            , p2_(p2)
            , p3_(p3) {
        }

        // –еализует метод Draw интерфейса svg::Drawable
        void Draw(svg::ObjectContainer& container) const override {
            container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
        }

    private:
        svg::Point p1_, p2_, p3_;
    };

    class Star final : public svg::Drawable {
        svg::Point center_;
        double outer_rad_;
        double inner_rad_;
        int num_rays_;
        svg::Color fill_color_ = "red";
        svg::Color stroke_color_ = "black";

    public:
        Star(svg::Point center, double outer_rad, double inner_rad, int num_rays)
            : center_(center)
            , outer_rad_(outer_rad)
            , inner_rad_(inner_rad)
            , num_rays_(num_rays) {}

        void Draw(svg::ObjectContainer& container) const override {
            container.Add(CreateStar(center_, outer_rad_, inner_rad_, num_rays_)
                .SetFillColor(fill_color_)
                .SetStrokeColor(stroke_color_));
        }
    };

    class Snowman final : public svg::Drawable {
        svg::Point head_center_;
        double head_radius_;
        svg::Color fill_color_ = "rgb(240,240,240)";
        svg::Color stroke_color_ = "black";

    public:
        Snowman(svg::Point center, double radius)
            : head_center_(center)
            , head_radius_(radius) {}

        void Draw(svg::ObjectContainer& container) const override {

            svg::Point body_center_ = { head_center_.x, head_center_.y + head_radius_ * 2 };
            double body_radius_ = head_radius_ * 1.5;

            svg::Point base_center_ = { head_center_.x, body_center_.y + head_radius_ * 3 };
            double base_radius_ = head_radius_ * 2;

            container.Add(svg::Circle()
                .SetCenter(base_center_)
                .SetRadius(base_radius_)
                .SetFillColor(fill_color_)
                .SetStrokeColor(stroke_color_));

            container.Add(svg::Circle()
                .SetCenter(body_center_)
                .SetRadius(body_radius_)
                .SetFillColor(fill_color_)
                .SetStrokeColor(stroke_color_));

            container.Add(svg::Circle()
                .SetCenter(head_center_)
                .SetRadius(head_radius_)
                .SetFillColor(fill_color_)
                .SetStrokeColor(stroke_color_));
        }
    };
}

template <typename DrawableIterator>
void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer& target) {
    for (auto it = begin; it != end; ++it) {
        (*it)->Draw(target);
    }
}

template <typename Container>
void DrawPicture(const Container& container, svg::ObjectContainer& target) {
    using namespace std;
    DrawPicture(begin(container), end(container), target);
}
// Выполняет линейную интерполяцию значения от from до to в зависимости от параметра t
uint8_t Lerp(uint8_t from, uint8_t to, double t) {
    return static_cast<uint8_t>(std::round((to - from) * t + from));
}

// Выполняет линейную интерполяцию Rgb цвета от from до to в зависимости от параметра t
svg::Rgb Lerp(svg::Rgb from, svg::Rgb to, double t) {
    return { Lerp(from.red, to.red, t), Lerp(from.green, to.green, t), Lerp(from.blue, to.blue, t) };
}

int main() {
    using namespace svg;
    using namespace std;

    Rgb start_color{ 0, 255, 30 };
    Rgb end_color{ 20, 20, 150 };

    const int num_circles = 10;
    Document doc;
    for (int i = 0; i < num_circles; ++i) {
        const double t = double(i) / (num_circles - 1);

        const Rgb fill_color = Lerp(start_color, end_color, t);

        doc.Add(Circle()
            .SetFillColor(fill_color)
            .SetStrokeColor("black"s)
            .SetCenter({ i * 20.0 + 40, 40.0 })
            .SetRadius(15));
    }
    doc.Render(cout);
}