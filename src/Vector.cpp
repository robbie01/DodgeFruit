// 2D Vector class
// Paige Mickol

#include <cmath>
#include <ostream>
#include <string>

#include "Vector.hpp"

// a small delta (used for comparing floats due to floating point inaccuracies)
#define DELTA 0.00001

/*
 * A 2D Vector class used to control positions and sizes of sprites on the screen
 */

Vector2::Vector2(float x, float y) {
    this->x_ = x;
    this->y_ = y;
    this->mag_ = std::sqrt(x*x + y*y);
}

Vector2::Vector2(float n) : Vector2(n, n) {}

// operator overloading to make vector2 operate as a mathmatical vector

// overload x+y
Vector2 Vector2::operator+(const Vector2& that) const {
    return Vector2(this->x_ + that.x_, this->y_ + that.y_);
}

Vector2 &Vector2::operator+=(const Vector2& that) {
    *this = *this + that;
    return *this;
}

// overload -x
Vector2 Vector2::operator-() const {
    return Vector2(-(this->x_), -(this->y_));
}

// overload x-y
Vector2 Vector2::operator-(const Vector2& that) const {
    return Vector2(this->x_ - that.x_, this->y_ - that.y_);
}

Vector2 &Vector2::operator-=(const Vector2& that) {
    *this = *this - that;
    return *this;
}

// overload x * y (element-by-element multiplication)
Vector2 Vector2::operator*(const Vector2& that) const {
    return Vector2(that.x_ * this->x_, that.y_ * this->y_);
}

Vector2 &Vector2::operator*=(const Vector2& that) {
    *this = *this * that;
    return *this;
}

// overload a == b
bool Vector2::operator==(const Vector2& that) const {
    return std::abs(this->x_ - that.x_) < DELTA && std::abs(this->y_ - that.y_) < DELTA;
}

// overload a != b
bool Vector2::operator!=(const Vector2& that) const {
    return !(*this == that);
}

// puts the vector in the form "(x, y)"
std::string Vector2::toString() const {
    return "(" + std::to_string(this->x_) + ", " + std::to_string(this->y_) + ")";
}

// the dot product of the 2 vectors
float Vector2::dot(const Vector2& that) const {
    return this->x_ * that.x_ + this->y_ * that.y_;
}


// getters for x component, y component, and the magnitude

float Vector2::x() const {
    return x_;
}

float Vector2::y() const {
    return y_;
}

float Vector2::mag() const {
    return mag_;
}

// overload the opposite order for scalar multiplication
Vector2 operator*(float a, Vector2 x) {
    return x * a;
}

// overload ostream with vector
std::ostream& operator<<(std::ostream& os, Vector2 v) {
    os << v.toString();
    return os;
}