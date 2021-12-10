// 2D Vector class
// Paige Mickol

#include <ostream>
#include <string>

#include "Vector.hpp"

extern "C" {
#include <math.h>
// libstdc++ bug, doesn't implement std::sinf and std::cosf
}

// a small delta (used for comparing floats due to floating point inaccuracies)
#define DELTA 0.00001

/*
 * A 2D Vector class used to control positions and sizes of sprites on the screen
 */

Vector2::Vector2(float x, float y) : x(x), y(y) {}
Vector2::Vector2(float n) : Vector2(n, n) {}

// operator overloading to make vector2 operate as a mathmatical vector

// overload x+y
Vector2 Vector2::operator+(const Vector2& that) const {
    return Vector2(x + that.x, y + that.y);
}

Vector2 &Vector2::operator+=(const Vector2& that) {
    *this = *this + that;
    return *this;
}

// overload -x
Vector2 Vector2::operator-() const {
    return Vector2(-(x), -(y));
}

// overload x-y
Vector2 Vector2::operator-(const Vector2& that) const {
    return Vector2(x - that.x, y - that.y);
}

Vector2 &Vector2::operator-=(const Vector2& that) {
    *this = *this - that;
    return *this;
}

// overload x * y (element-by-element multiplication)
Vector2 Vector2::operator*(const Vector2& that) const {
    return Vector2(that.x * x, that.y * y);
}

Vector2 &Vector2::operator*=(const Vector2& that) {
    *this = *this * that;
    return *this;
}

// overload a == b
bool Vector2::operator==(const Vector2& that) const {
    return fabs(x - that.x) < DELTA && fabs(y - that.y) < DELTA;
}

// overload a != b
bool Vector2::operator!=(const Vector2& that) const {
    return !(*this == that);
}

// overload the opposite order for scalar multiplication
Vector2 operator*(float a, Vector2 x) {
    return x * a;
}