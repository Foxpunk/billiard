#pragma once
#include "Vector2D.h"

class Wall {
public:
    Wall(const Vector2D& start, const Vector2D& end);
    Vector2D getStart() const;
    Vector2D getEnd() const;

private:
    Vector2D start;
    Vector2D end;
};
