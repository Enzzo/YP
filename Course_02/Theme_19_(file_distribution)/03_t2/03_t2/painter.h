#include "wall.h"

class Painter {
public:
    // �������� ����� Painter
    void Paint(Wall& wall, const Wall::Color color){
        wall.SetColor(color);
    }
};