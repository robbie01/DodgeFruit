#include "SDL.hpp"

// sim libraries
#include "LCD.hpp"

// paige libraries :)
#include "Vector.hpp"
#include "Sprite.hpp"
#include "Fruit.hpp"

// c++ libraries
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <chrono>

void showMenu();

int showStatsScreen();
int showHowToScreen();
int showCredits();

int doGameplayLoop(int, int);
Fruit makeRandomFruit();
void showGameOverScreen(float);

int waitForBackButtonPress();

void writeTime(float);
float playerHighScore();
int gameCount();

/*
 * Entry point to the application
 */
int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    std::srand(std::time(nullptr));
    showMenu();
    return 0;
}
/*
 * menu screens!
 */
void showMenu() {
begin_menu:
    // display menu
    Sprite logo("menu/logo", Vector2(41, 21));
    logo.scale = MENU_SCALE;
    logo.move(Vector2(7, 4) * MENU_SCALE);

    Sprite play("menu/play_button", Vector2(46, 25));
    play.scale = MENU_SCALE;
    play.move(Vector2(4, 30) * MENU_SCALE);

    Sprite stats("menu/stats_button", Vector2(26, 19));
    stats.scale = MENU_SCALE;
    stats.move(Vector2(52, 2) * MENU_SCALE);

    Sprite howto("menu/how_to_button", Vector2(27, 16));
    howto.scale = MENU_SCALE;
    howto.move(Vector2(52, 22) * MENU_SCALE);

    Sprite credits("menu/credits_button", Vector2(26, 15));
    credits.scale = MENU_SCALE;
    credits.move(Vector2(52, 41) * MENU_SCALE);

    SDL_Event ev;

    while (true) {
        LCD.SetBackgroundColor(BLACK);
        LCD.Clear();

        // draw menu
        logo.draw();
        play.draw();
        stats.draw();
        howto.draw();
        credits.draw();
        
        LCD.Update();
    
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                return;
            case SDL_MOUSEBUTTONDOWN: {
                SDL_MouseButtonEvent mev = ev.button;
                if (mev.button != SDL_BUTTON_LEFT) break;
                Vector2 screenPoint(mev.x/PIXEL_SCALE, mev.y/PIXEL_SCALE);
                if (play.isPointWithin(screenPoint)) {
                    if (doGameplayLoop(mev.x, mev.y)) return;
                } else if (stats.isPointWithin(screenPoint)) {
                    if (showStatsScreen()) return;
                } else if (howto.isPointWithin(screenPoint)) {
                    if (showHowToScreen()) return;
                } else if (credits.isPointWithin(screenPoint)) {
                    if (showCredits()) return;
                } else break;
            }
            default:
                break;
            }
        }
    }
}


int showStatsScreen() {
    LCD.Clear();

    // draw stats background in the center of the screen
    Sprite statsBack("menu/stats_background", Vector2(80, 60));
    statsBack.scale = 3;
    statsBack.anchorPoint = Vector2(0.5, 0.5);
    statsBack.pos = Vector2(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/2);

    statsBack.draw();

    //display high score
    float highScore = playerHighScore();
    LCD.SetFontColor(BLACK);
    LCD.WriteAt(highScore, 200, 91);

    //display game count
    int gameCounter = gameCount();
    LCD.SetFontColor(BLACK);
    LCD.WriteAt(gameCounter, 250, 130);
    LCD.Update();

    return waitForBackButtonPress();
}


int showHowToScreen() {
    LCD.Clear();

    // draw howto screen in the middle
    Sprite howToBack("menu/instruction_background", Vector2(80, 60));
    howToBack.scale = 3;
    howToBack.anchorPoint = Vector2(0.5, 0.5);
    howToBack.pos = Vector2(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/2);
    howToBack.draw();
    LCD.Update();

    return waitForBackButtonPress();
}


int showCredits() {
    LCD.Clear();

    Sprite creditsBack("menu/credits_background", Vector2(80, 60));
    creditsBack.scale = 4;
    creditsBack.anchorPoint = Vector2(0.5, 0.5);
    creditsBack.pos = Vector2(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/2);
    creditsBack.draw();
    LCD.Update();

    return waitForBackButtonPress();
}


/*
 * gameplay!!
 */
int doGameplayLoop(int xpos, int ypos) {
    // make character
    Sprite character("pointer", Vector2(8, 8));
    character.anchorPoint = Vector2(0.5, 0.5);
    character.scale = 2;

    // fruit list
    std::vector<Fruit> projectiles;

    // timing vars
    auto lastTime = std::chrono::system_clock::now();
    auto lastFruitSpawnTime = lastTime - std::chrono::seconds(4);

    // gameplay vars
    float playerTime = 0.0;
    bool gameOver = false;

    bool touching = true;

    // game should keep playing as long as the screen is being touched
    while (touching && !gameOver) {
        Vector2 mousePos = Vector2(xpos/PIXEL_SCALE, ypos/PIXEL_SCALE);

        // clear
        LCD.Clear();

        // draw background
        LCD.SetBackgroundColor(BLACK);

        // draw character
        character.move(mousePos);

        auto now = std::chrono::system_clock::now();

        // step fruits
        float dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastTime).count();
        for(Fruit &f : projectiles) {

            f.stepPath(dt);

            // check if its hitting the mouse
            if(f.sprite().isPointWithin(mousePos)){
                gameOver = true;
            }

        }
        lastTime = now;

        // create new fruit
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
                return 1;
            case SDL_MOUSEBUTTONUP:
                if (ev.button.button == SDL_BUTTON_LEFT) touching = false;
                break;
            case SDL_MOUSEMOTION: {
                SDL_MouseMotionEvent mev = ev.motion;
                xpos = mev.x;
                ypos = mev.y;
            }
            default:
                break;
            }
        }
    }

    showGameOverScreen(playerTime);

    // write time to file
    writeTime(playerTime);

    touching = false;
    SDL_Event ev;
    while (true) {
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEBUTTONDOWN:
                if (ev.button.button == SDL_BUTTON_LEFT) touching = true;
                break;
            case SDL_MOUSEBUTTONUP:
                if (ev.button.button == SDL_BUTTON_LEFT && touching) return 0;
                break;
            default:
                break;
            }
        }
    }
}

// makes and returns a new random fruit to be used in gameplay
Fruit makeRandomFruit() {
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

// final screen after the player touches a fruit
void showGameOverScreen(float finalTime) {
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();

    // draw gameover screen in the middle
    Sprite gameOverScreen("menu/game_over_background", Vector2(80, 60));
    gameOverScreen.anchorPoint = Vector2(0.5, 0.5);
    gameOverScreen.move(Vector2(SCREEN_SIZE_X/2, SCREEN_SIZE_Y/2));

    // write final time to screen
    LCD.SetFontColor(CYAN);
    LCD.WriteAt(finalTime, 187, 166);

    LCD.Update();
}

int waitForBackButtonPress() {
    // draw back button
    Sprite backButton("menu/back_button", Vector2(16, 8));
    backButton.anchorPoint = Vector2(0, 1);
    backButton.pos = Vector2(8, SCREEN_SIZE_Y - 8);//
    backButton.draw();
    LCD.Update();

    // wait for player to touch the button
    bool touching = false;
    int xpos=0, ypos=0;

    SDL_Event ev;
    while (true) {
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEBUTTONDOWN: {
                SDL_MouseButtonEvent mev = ev.button;
                if (mev.button == SDL_BUTTON_LEFT && backButton.isPointWithin(Vector2(mev.x/PIXEL_SCALE, mev.y/PIXEL_SCALE))) return 0;
                break;
            }
            default:
                break;
            }
        }
    }
}

// write time to file
void writeTime(float t) {
    std::ofstream playerTimeFile("playerTimeFile.txt", std::ofstream::app);
    playerTimeFile  << t << '\n';
}

// find the player's highscore from the file
float playerHighScore() {
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
int gameCount() {
    std::ifstream playerTimeFile("playerTimeFile.txt");

    int counter = 0;
    float temp;
    while (playerTimeFile >> temp){
        counter++;
    }

    return counter;
}