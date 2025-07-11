#define _USE_MATH_DEFINES
#include "svg.h"

#include <cmath>

using namespace std::literals;
using namespace svg;


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
        ~Triangle() override {};

        // ��������� ����� Draw ���������� svg::Drawable
        void Draw(svg::ObjectContainer& container) const override {
            container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
        }

    private:
        svg::Point p1_, p2_, p3_;
    };

    class Star final: public svg::Drawable{ 
        svg::Point center_;
        double outer_rad_; 
        double inner_rad_;
        int num_rays_;

    public:
        Star(svg::Point center, double outer_rad, double inner_rad, int num_rays) 
            : center_(center)
            , outer_rad_(outer_rad)
            , inner_rad_(inner_rad)
            , num_rays_(num_rays){}
        ~Star() override {}
        void Draw(svg::ObjectContainer& container) const override {
            container.Add(CreateStar(center_, outer_rad_, inner_rad_, num_rays_));
        }
    };
    class Snowman final: public svg::Drawable{
        svg::Point head_center_;
        double head_radius_;

    public:
        Snowman(svg::Point center, double radius) : head_center_(center), head_radius_(radius){}
        ~Snowman() override {}
        void Draw(svg::ObjectContainer& container) const override {            

            svg::Point body_center_ = { head_center_.x, head_center_.y + head_radius_ * 2 };
            double body_radius_ = head_radius_ * 1.5;            

            svg::Point base_center_ = { head_center_.x, body_center_.y + head_radius_ * 3};
            double base_radius_ = head_radius_ * 2;

            container.Add(svg::Circle().SetCenter(base_center_).SetRadius(base_radius_));
            container.Add(svg::Circle().SetCenter(body_center_).SetRadius(body_radius_));
            container.Add(svg::Circle().SetCenter(head_center_).SetRadius(head_radius_));
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

int main() {
    using namespace svg;
    using namespace shapes;
    using namespace std;

    vector<unique_ptr<svg::Drawable>> picture;

    picture.emplace_back(make_unique<Triangle>(Point{ 100, 20 }, Point{ 120, 50 }, Point{ 80, 40 }));
    // 5-������� ������ � ������� {50, 20}, ������ ����� 10 � ���������� �������� 4
    picture.emplace_back(make_unique<Star>(Point{ 50.0, 20.0 }, 10.0, 4.0, 5));
    // �������� � "�������" �������� 10, ������� ����� � ����� {30, 20}
    picture.emplace_back(make_unique<Snowman>(Point{ 30, 20 }, 10.0));

    svg::Document doc;
    // ��� ��� �������� ��������� ��������� ObjectContainer,
    // ��� ����� �������� � DrawPicture � �������� ���� ��� ���������
    DrawPicture(picture, doc);

    // ������� ���������� �������� � stdout
    doc.Render(cout);
}