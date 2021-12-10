// Fruit Projectile Class
// Paige Mickol & Alina Mueez

#include "Constants.hpp"
#include "Vector.hpp"
#include "Sprite.hpp"
#include "Fruit.hpp"

#include <string>

extern "C" {
#include <math.h>
// libstdc++ bug, doesn't implement std::sinf and std::cosf
}

static Vector2 fruitpos(float t, Vector2 offset, Fruit::FruitType type) {
    switch (type) {
    case Fruit::APPLE:
        // star
        return Vector2(SCREEN_SIZE_Y/2 * powf(cosf(t),5) + offset.x, SCREEN_SIZE_Y/2 * powf(sinf(t),5) + offset.y);
    case Fruit::TANGERINE:
        // sin wave
        return Vector2(SCREEN_SIZE_Y/2 * t + offset.x, SCREEN_SIZE_Y/2 * sinf(t) + offset.y);
    case Fruit::STRAWBERRY:
        // unimplemented?
        return Vector2();
    case Fruit::WATERMELON:
        // infinity
        return Vector2(SCREEN_SIZE_Y/2 * cosf(t) + offset.x, SCREEN_SIZE_Y/2*((sinf(2*t))/2) + offset.y);
    case Fruit::LEMON:
        // circle
        return Vector2(SCREEN_SIZE_Y/2 * cosf(t) + offset.x, SCREEN_SIZE_Y/2 * sinf(t) + offset.y);
    default:
        return Vector2();
    }
}

static std::string fruittype_str(Fruit::FruitType type) {
    switch (type) {
    case Fruit::APPLE: return "apple";
    case Fruit::TANGERINE: return "tangerine";
    case Fruit::STRAWBERRY: return "strawberry";
    case Fruit::WATERMELON: return "watermelon";
    case Fruit::LEMON: return "lemon";
    default: return std::string();
    }
}

Fruit::Fruit(FruitType type, float speed, const Vector2 &offset) :
    sprite_("fruits/" + fruittype_str(type), FRUIT_SIZE), offset_(offset), type_(type), speed_(speed), t_(0) {}

void Fruit::stepPath(float dt) {
    // time * speed = position
    t_ += dt * speed_;

    // pick function based on fruit type
    sprite_.move(fruitpos(t_, offset_, type_));
}

Sprite &Fruit::sprite() {
    return sprite_;
}