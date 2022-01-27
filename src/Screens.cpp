#include <fstream>

#include "SDL.hpp"

#include "Constants.hpp"
#include "Screens.hpp"
#include "Vector.hpp"
#include "LCD.hpp"

Screen::~Screen() {}

void Screen::init() {}

ScreenUpdateReturn Screen::update() {
    LCD.Update();

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_QUIT:
            return std::monostate();
        default:
            break;
        }
    }
    return nullptr;
}

MenuScreen::MenuScreen() :
    logo("menu/logo", Vector2(41, 21)),
    play("menu/play_button", Vector2(46, 25)),
    stats("menu/stats_button", Vector2(26, 19)),
    howto("menu/how_to_button", Vector2(27, 16)),
    credits("menu/credits_button", Vector2(26, 15)) {}

void MenuScreen::init() {
    LCD.bgcolor = BLACK;
    LCD.Clear();

    logo.scale = MENU_SCALE;
    logo.move(Vector2(7, 4) * MENU_SCALE);

    play.scale = MENU_SCALE;
    play.move(Vector2(4, 30) * MENU_SCALE);

    stats.scale = MENU_SCALE;
    stats.move(Vector2(52, 2) * MENU_SCALE);

    howto.scale = MENU_SCALE;
    howto.move(Vector2(52, 22) * MENU_SCALE);

    credits.scale = MENU_SCALE;
    credits.move(Vector2(52, 41) * MENU_SCALE);
}

ScreenUpdateReturn MenuScreen::update() {
    LCD.Update();

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_QUIT:
            return std::monostate();
        case SDL_MOUSEBUTTONDOWN: {
            SDL_MouseButtonEvent mev = ev.button;
            WindowSize sz = LCD.size();
            if (mev.button != SDL_BUTTON_LEFT) break;
            Vector2 screenPoint(mev.x*SCREEN_SIZE_X/sz.w, mev.y*SCREEN_SIZE_Y/sz.h);
            if (play.isPointWithin(screenPoint)) {
                return ScreenPtr(new GameplayScreen);
            } else if (stats.isPointWithin(screenPoint)) {
                return ScreenPtr(new StatsScreen);
            } else if (howto.isPointWithin(screenPoint)) {
                return ScreenPtr(new HowtoScreen);
            } else if (credits.isPointWithin(screenPoint)) {
                return ScreenPtr(new CreditsScreen);
            } else break;
        }
        default:
            break;
        }
    }

    return nullptr;
}

GameplayScreen::GameplayScreen() :
    character("pointer", Vector2(8, 8)),
    beginTime(std::chrono::system_clock::now()),
    lastTime(beginTime),
    lastFruitSpawnTime(lastTime - std::chrono::seconds(4)),
    playerTime(0)
{
    character.anchorPoint = Vector2(0.5, 0.5);
    character.scale = 2;
    character.nowrap = true;
}

void GameplayScreen::init() {
    SDL_GetMouseState(&xpos, &ypos);
    LCD.bgcolor = BLACK;
}

static Fruit makeRandomFruit() {
    // list all possible fruits
    Fruit::FruitType fruits[] = {Fruit::LEMON, Fruit::APPLE, Fruit::WATERMELON, Fruit::TANGERINE};
    // choose a random one
    auto chosenFruit = fruits[std::rand() % 4];

    // choose a spawning position for the fruit
    Vector2 spawnPos(std::rand() % 32767, std::rand() % 32767); // emulate (semi-broken) randint behavior

    // choose a random speed for the fruit between 1 and 2
    float speed = 1 + (std::rand() % 10)/10.0;

    // make the fruit and scale it
    Fruit newFruit(chosenFruit, speed, spawnPos);
    newFruit.sprite.anchorPoint = Vector2(0.5, 0.5);
    newFruit.sprite.scale = 2;

    return newFruit;
}

ScreenUpdateReturn GameplayScreen::update() {
    LCD.Clear();

    WindowSize sz = LCD.size();
    float w, h, woff, hoff;
    if (sz.w > sz.h * SCREEN_SIZE_X / SCREEN_SIZE_Y) {
        w = sz.h * SCREEN_SIZE_X / SCREEN_SIZE_Y;
        h = sz.h;
        woff = (sz.w-w)/2.f;
        hoff = 0.f;
    } else {
        w = sz.w;
        h = sz.w * SCREEN_SIZE_Y / SCREEN_SIZE_X;
        woff = 0.f;
        hoff = (sz.h-h)/2.f;
    }
    Vector2 mousePos((xpos-woff)*SCREEN_SIZE_X/w, (ypos-hoff)*SCREEN_SIZE_Y/h);
    if (mousePos.x < 0) mousePos.x = 0;
    else if (mousePos.x >= SCREEN_SIZE_X) mousePos.x = SCREEN_SIZE_X - 1;
    if (mousePos.y < 0) mousePos.y = 0;
    else if (mousePos.y >= SCREEN_SIZE_Y) mousePos.y = SCREEN_SIZE_Y - 1;
    character.move(mousePos);

    auto now = std::chrono::system_clock::now();
    float dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastTime).count();

    playerTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - beginTime).count();

    for(Fruit &f : projectiles) {
        f.stepPath(dt);
        if(f.sprite.isPointWithin(mousePos)) {
            return ScreenPtr(new GameOverScreen(playerTime));
        }
    }

    lastTime = now;

    if (now - lastFruitSpawnTime > std::chrono::seconds(3)) {
        projectiles.emplace_back(makeRandomFruit());
        lastFruitSpawnTime = now;
    }

    //write current time to top right corner of screen
    LCD.WriteAt(playerTime, 240, 15);

    // update screen
    LCD.Update();

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_QUIT:
            return std::monostate();
        case SDL_MOUSEBUTTONUP:
            if (ev.button.button == SDL_BUTTON_LEFT) return ScreenPtr(new GameOverScreen(playerTime));
            break;
        case SDL_MOUSEMOTION: {
            SDL_MouseMotionEvent mev = ev.motion;
            xpos = mev.x;
            ypos = mev.y;
            break;
        }
        default:
            break;
        }
    }

    return nullptr;
}

BackButtonScreen::BackButtonScreen() :
    backButton("menu/back_button", Vector2(16, 8)),
    touching(false)
{
    backButton.anchorPoint = Vector2(0, 1);
    backButton.pos = Vector2(8, SCREEN_SIZE_Y - 8);
}

void BackButtonScreen::init() {
    backButton.draw();
}

ScreenUpdateReturn BackButtonScreen::update() {
    LCD.Update();

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_QUIT:
            return std::monostate();
        case SDL_MOUSEBUTTONDOWN:
            if (ev.button.button == SDL_BUTTON_LEFT) touching = true;
            break;
        case SDL_MOUSEBUTTONUP: {
            SDL_MouseButtonEvent mev = ev.button;
            WindowSize sz = LCD.size();
            if (mev.button == SDL_BUTTON_LEFT && touching && backButton.isPointWithin(Vector2(mev.x*SCREEN_SIZE_X/sz.w, mev.y*SCREEN_SIZE_Y/sz.h)))
                return ScreenPtr(new MenuScreen);
        }
        default:
            break;
        }
    }
    return nullptr;
}

void HowtoScreen::init() {
    LCD.bgcolor = BLACK;
    LCD.Clear();

    // draw howto screen in the middle
    Sprite howToBack("menu/instruction_background", Vector2(80, 60));
    howToBack.scale = 3;
    howToBack.anchorPoint = Vector2(0.5, 0.5);
    howToBack.pos = Vector2(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/2);
    howToBack.draw();

    BackButtonScreen::init();
}

void CreditsScreen::init() {
    LCD.bgcolor = BLACK;
    LCD.Clear();

    Sprite creditsBack("menu/credits_background", Vector2(80, 60));
    creditsBack.scale = 4;
    creditsBack.anchorPoint = Vector2(0.5, 0.5);
    creditsBack.pos = Vector2(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/2);
    creditsBack.draw();

    BackButtonScreen::init();
}

// find the player's highscore from the file
static float playerHighScore() {
    std::ifstream playerTimeFile("playerTimeFile.txt");
    
    float score, highScore=0;
    while (playerTimeFile >> score){
        if (highScore < score) {
            highScore = score;
        }
    }

    return highScore;
}

// count the saved games
static int gameCount() {
    std::ifstream playerTimeFile("playerTimeFile.txt");

    int counter = 0;
    float temp;
    while (playerTimeFile >> temp){
        counter++;
    }

    return counter;
}

StatsScreen::StatsScreen() :
    highScore(playerHighScore()), gameCounter(gameCount()) {}

void StatsScreen::init() {
    LCD.bgcolor = BLACK;
    LCD.Clear();

    // draw stats background in the center of the screen
    Sprite statsBack("menu/stats_background", Vector2(80, 60));
    statsBack.scale = 3;
    statsBack.anchorPoint = Vector2(0.5, 0.5);
    statsBack.pos = Vector2(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/2);
    statsBack.draw();

    //display high score
    LCD.color = BLACK;
    LCD.WriteAt(highScore, 200, 91);

    //display game count
    LCD.color = BLACK;
    LCD.WriteAt(gameCounter, 250, 130);

    BackButtonScreen::init();
}

// write time to file
static void writeTime(float t) {
    std::ofstream playerTimeFile("playerTimeFile.txt", std::ofstream::app);
    playerTimeFile  << t << '\n';
}

GameOverScreen::GameOverScreen(float score) : score(score), touching(false) {
    writeTime(score);
}

void GameOverScreen::init() {
    LCD.bgcolor = BLACK;
    LCD.Clear();

    Sprite gameOverScreen("menu/game_over_background", Vector2(80, 60));
    gameOverScreen.anchorPoint = Vector2(0.5, 0.5);
    gameOverScreen.move(Vector2(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/2));

    // write final time to screen
    LCD.color = CYAN;
    LCD.WriteAt(score, 187, 166);
}

ScreenUpdateReturn GameOverScreen::update() {
    LCD.Update();

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_QUIT:
            return std::monostate();
        case SDL_MOUSEBUTTONDOWN:
            if (ev.button.button == SDL_BUTTON_LEFT) touching = true;
            break;
        case SDL_MOUSEBUTTONUP:
            if (ev.button.button == SDL_BUTTON_LEFT && touching) return ScreenPtr(new MenuScreen);
        default:
            break;
        }
    }
    return nullptr;
}