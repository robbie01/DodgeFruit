// Fruit Projectile Class
// Paige Mickol & Alina Mueez

#include "Constants.hpp"
#include "Vector.hpp"
#include "Sprite.hpp"
#include "Fruit.hpp"

#include <cstring>
#include <fstream>
#include <iostream>

extern "C" {
#include <math.h>
// libstdc++ bug, doesn't implement std::sinf and std::cosf
}

// fruit types!
static Vector2 applepos(float t, Vector2 offset);
static Vector2 tangerinePos(float t, Vector2 offset);
static Vector2 strawberryPos(float t, Vector2 offset);
static Vector2 watermelonpos(float t, Vector2 offset);
static Vector2 lemonpos(float t, Vector2 offset);

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
    sprite_("fruits/" + fruittype_str(type), FRUIT_SIZE), offset_(offset), type_(type), speed_(speed) {}

void Fruit::stepPath(float dt) {
    // time * speed = position
    t_ += dt * speed_;

    // pick function based on fruit type
    if(type_ == APPLE) {
        sprite_.move(applepos(t_, offset_));
    } else if (type_ == WATERMELON) {
        sprite_.move(watermelonpos(t_, offset_));
    } else if (type_ == LEMON) {
        sprite_.move(lemonpos(t_, offset_));
    } else if (type_ == TANGERINE) {
        sprite_.move(tangerinePos(t_, offset_));
    }
}

Sprite &Fruit::sprite() {
    return sprite_;
}

// star
static Vector2 applepos(float t, Vector2 offset) {
    return Vector2(SCREEN_SIZE_Y/2 * powf(cosf(t),5) + offset.x(), SCREEN_SIZE_Y/2 * powf(sinf(t),5) + offset.y());
}

// diamond
static Vector2 tangerinePos(float t, Vector2 offset) {
    return Vector2(SCREEN_SIZE_Y/2 * t + offset.x(), SCREEN_SIZE_Y/2 * sinf(t) + offset.y());
}

// sin wave
static Vector2 strawberryPos(float t, Vector2 offset) {
    return Vector2();
}

// infinity
static Vector2 watermelonpos(float t, Vector2 offset) {
    return Vector2(SCREEN_SIZE_Y/2 * cosf(t) + offset.x(), SCREEN_SIZE_Y/2*((sinf(2*t))/2) + offset.y());
}

// circle
static Vector2 lemonpos(float t, Vector2 offset) {
    return Vector2(SCREEN_SIZE_Y/2 * cosf(t) + offset.x(), SCREEN_SIZE_Y/2 * sinf(t) + offset.y());
}
