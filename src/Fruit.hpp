#pragma once

#include "Vector.hpp"
#include "Sprite.hpp"

class Fruit {
    public:
        enum FruitType {
            APPLE,
            TANGERINE,
            STRAWBERRY,
            WATERMELON,
            LEMON
        };

        Fruit(FruitType, float, const Vector2&);
        void stepPath(float dt);
        Sprite &sprite();

    private:
        Sprite sprite_;
        FruitType type_;
        float t_ = 0;
        float speed_;
        Vector2 offset_;
};