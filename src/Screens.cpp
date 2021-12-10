#include <fstream>

#include "SDL.hpp"

#include "Constants.hpp"
#include "Screens.hpp"
#include "Vector.hpp"
#include "LCD.hpp"

void Screen::init() {}

ScreenUpdateReturn Screen::update() {
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
    LCD.SetBackgroundColor(BLACK);
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

    LCD.Update();
}

ScreenUpdateReturn MenuScreen::update() {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_QUIT:
            return std::monostate();
        case SDL_MOUSEBUTTONDOWN: {
            SDL_MouseButtonEvent mev = ev.button;
            if (mev.button != SDL_BUTTON_LEFT) break;
            Vector2 screenPoint(mev.x/PIXEL_SCALE, mev.y/PIXEL_SCALE);
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
    lastTime(std::chrono::system_clock::now()),
    lastFruitSpawnTime(lastTime - std::chrono::seconds(4)),
    playerTime(0)
{
    character.anchorPoint = Vector2(0.5, 0.5);
    character.scale = 2;
}

void GameplayScreen::init() {
    SDL_GetMouseState(&xpos, &ypos);
    LCD.SetBackgroundColor(BLACK);
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
    newFruit.sprite().anchorPoint = Vector2(0.5, 0.5);
    newFruit.sprite().scale = 2;

    return newFruit;
}

ScreenUpdateReturn GameplayScreen::update() {
    LCD.Clear();

    Vector2 mousePos(xpos / PIXEL_SCALE, ypos / PIXEL_SCALE);
    character.move(mousePos);

    auto now = std::chrono::system_clock::now();
    float dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastTime).count();

    for(Fruit &f : projectiles) {
        f.stepPath(dt);
        if(f.sprite().isPointWithin(mousePos)) {
            return ScreenPtr(new GameOverScreen(playerTime));
        }
    }

    lastTime = now;

    if (now - lastFruitSpawnTime > std::chrono::seconds(3)) {
        projectiles.emplace_back(makeRandomFruit());
        lastFruitSpawnTime = now;
    }

    //increment timer
    playerTime = playerTime + dt;

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
    backButton("menu/back_button", Vector2(16, 8))
{
    backButton.anchorPoint = Vector2(0, 1);
    backButton.pos = Vector2(8, SCREEN_SIZE_Y - 8);
}

void BackButtonScreen::init() {
    backButton.draw();
    LCD.Update();
}

ScreenUpdateReturn BackButtonScreen::update() {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_QUIT:
            return std::monostate();
        case SDL_MOUSEBUTTONDOWN: {
            SDL_MouseButtonEvent mev = ev.button;
            if (mev.button == SDL_BUTTON_LEFT && backButton.isPointWithin(Vector2(mev.x/PIXEL_SCALE, mev.y/PIXEL_SCALE)))
                return ScreenPtr(new MenuScreen);
        }
        default:
            break;
        }
    }
    return nullptr;
}

void HowtoScreen::init() {
    LCD.SetBackgroundColor(BLACK);
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
    LCD.SetBackgroundColor(BLACK);
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
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();

    // draw stats background in the center of the screen
    Sprite statsBack("menu/stats_background", Vector2(80, 60));
    statsBack.scale = 3;
    statsBack.anchorPoint = Vector2(0.5, 0.5);
    statsBack.pos = Vector2(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/2);
    statsBack.draw();

    //display high score
    LCD.SetFontColor(BLACK);
    LCD.WriteAt(highScore, 200, 91);

    //display game count
    LCD.SetFontColor(BLACK);
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
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();

    Sprite gameOverScreen("menu/game_over_background", Vector2(80, 60));
    gameOverScreen.anchorPoint = Vector2(0.5, 0.5);
    gameOverScreen.move(Vector2(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/2));

    // write final time to screen
    LCD.SetFontColor(CYAN);
    LCD.WriteAt(score, 187, 166);

    LCD.Update();
}

ScreenUpdateReturn GameOverScreen::update() {
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