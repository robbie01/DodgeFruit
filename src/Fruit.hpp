#pragma once

#include "Vector.hpp"
#include "Sprite.hpp"

class Fruit {
    public:
        Sprite sprite;

        enum FruitType {
            APPLE,
            TANGERINE,
            STRAWBERRY,
            WATERMELON,
            LEMON
        };

        Fruit(FruitType, float, const Vector2&);
        void stepPath(float dt);

    private:
        FruitType type_;
        float t_;
        float speed_;
        Vector2 offset_;
};