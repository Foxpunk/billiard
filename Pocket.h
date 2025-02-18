#pragma once
#include "Vector2D.h"
#include <SDL.h>

class Pocket {
public:
    Pocket(float x, float y, float r);
    Vector2D getPosition() const { return position; }
    float getRadius() const { return radius; }
    void draw(SDL_Renderer* renderer) const;

private:
    Vector2D position;
    float radius;
};
