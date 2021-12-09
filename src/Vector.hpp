#pragma once

#include <string>

class Vector2 {
    float x_, y_, mag_;
public:
    Vector2(float x, float y);
    Vector2(float n = 0);
    Vector2 operator+(const Vector2&) const;
    Vector2 &operator+=(const Vector2&);
    Vector2 operator-() const;
    Vector2 operator-(const Vector2&) const;
    Vector2 &operator-=(const Vector2&);
    Vector2 operator*(const Vector2&) const;
    Vector2 &operator*=(const Vector2&);
    bool operator==(const Vector2&) const;
    bool operator!=(const Vector2&) const;
    std::string toString() const;
    float dot(const Vector2&) const;
    float x() const;
    float y() const;
    float mag() const;
};

Vector2 operator*(float a, Vector2 x);
std::ostream& operator<<(std::ostream& os, Vector2 v);