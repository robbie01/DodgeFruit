#pragma once

#include <memory>
#include <variant>
#include <vector>
#include <chrono>

#include "Sprite.hpp"
#include "Fruit.hpp"

class Screen;

using ScreenPtr = std::unique_ptr<Screen>;
using ScreenUpdateReturn = std::variant<std::monostate, ScreenPtr>;

class Screen {
public:
    virtual void init();
    virtual ScreenUpdateReturn update();
};

class MenuScreen : public Screen {
    Sprite logo, play, stats, howto, credits;
public:
    MenuScreen();
    void init();
    ScreenUpdateReturn update();
};

class GameplayScreen : public Screen {
    Sprite character;
    std::vector<Fruit> projectiles;
    std::chrono::time_point<std::chrono::system_clock> lastTime, lastFruitSpawnTime;
    float playerTime;
    int xpos, ypos;
public:
    GameplayScreen();
    void init();
    ScreenUpdateReturn update();
};

class BackButtonScreen : public Screen {
    Sprite backButton;
public:
    BackButtonScreen();
    virtual void init();
    ScreenUpdateReturn update();
};

class HowtoScreen : public BackButtonScreen {
public:
    void init();
};

class GameOverScreen : public Screen {
    float score;
    bool touching;
public:
    GameOverScreen(float);
    void init();
    ScreenUpdateReturn update();
};

class CreditsScreen : public BackButtonScreen {
public:
    void init();
};

class StatsScreen : public BackButtonScreen {
    float highScore;
    int gameCounter;
public:
    StatsScreen();
    void init();
};