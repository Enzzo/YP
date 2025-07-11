#pragma once
#include "square_calculation.h"
#include "wall.h"

#include <utility>

class Accountant {
public:
    template <class BuildingObject>
    double CalcPaintNeeded(const BuildingObject& building_object) const;
    template <class BuildingObject>
    double CalcBricksNeeded(const BuildingObject& building_object) const;
};

#include "square_calculation.h"


using namespace std;

template <class BuildingObject>
double Accountant::CalcPaintNeeded(const BuildingObject& building_object) const {
    pair<double, double> sizes = building_object.GetSizes();
    return CalcSquare(sizes.first, sizes.second) * 0.4;
}

template <class BuildingObject>
double Accountant::CalcBricksNeeded(const BuildingObject& building_object) const {
    pair<double, double> sizes = building_object.GetSizes();
    return CalcSquare(sizes.first, sizes.second) * 5;
}