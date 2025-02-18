#pragma once
#include <cmath>

class Vector2D {
public:
    Vector2D();
    Vector2D(float x, float y);

    float getX() const;
    float getY() const;
    void setX(float x);
    void setY(float y);

    float dot(const Vector2D& other) const;
    void normalize();
    float length() const;

    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(float scalar) const;

private:
    float x, y;
};
