#include "Vector2D.h"
#include <cmath>

Vector2D::Vector2D() : x(0), y(0) {}

Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

float Vector2D::getX() const {
    return x;
}

float Vector2D::getY() const {
    return y;
}

void Vector2D::setX(float x) {
    this->x = x;
}

void Vector2D::setY(float y) {
    this->y = y;
}

float Vector2D::dot(const Vector2D& other) const {
    return x * other.x + y * other.y;
}

void Vector2D::normalize() {
    float length = std::sqrt(x * x + y * y);
    if (length > 0) {
        x /= length;
        y /= length;
    }
}

float Vector2D::length() const {
    return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::operator+(const Vector2D& other) const {
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::operator*(float scalar) const {
    return Vector2D(x * scalar, y * scalar);
}
