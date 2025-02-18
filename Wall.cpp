#include "Wall.h"

Wall::Wall(const Vector2D& start, const Vector2D& end) : start(start), end(end) {}

Vector2D Wall::getStart() const {
    return start;
}

Vector2D Wall::getEnd() const {
    return end;
}
