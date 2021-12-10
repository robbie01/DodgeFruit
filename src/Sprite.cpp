// Sprite class
// Paige Mickol

#include <cstring>
#include <fstream>
#include <iostream>

#include "Constants.hpp"
#include "Vector.hpp"
#include "LCD.hpp"
#include "Sprite.hpp"

Sprite::Sprite(std::string spriteName, const Vector2 &spriteSize) :
    pos(0, 0), size(spriteSize), scale(GAME_SCALE), anchorPoint(0, 0)
{
    swapImage(spriteName, spriteSize);
}

// sets the positions, then draws the sprite to the graphics buffe
void Sprite::move(const Vector2 &pos) {
    this->pos = pos;
    draw();
}

// replaces the sprite image
void Sprite::swapImage(std::string spriteName, const Vector2 &spriteSize) {
    std::ifstream sprFile("sprites/" + spriteName + ".spr");

    int pixelCount = 0;
    pixel_t next;

    while(sprFile >> next){
        int x = pixelCount / (int)spriteSize.x();
        int y = pixelCount % (int)spriteSize.x();
        image[x][y] = next;
        pixelCount++;
    }
}

// draw the sprite onto the graphics buffer
void Sprite::draw() {
    for(int i = 0; i < size.x(); i++) {
        for(int j = 0; j < size.y(); j++) {
            pixel_t color = image[j][i];
            bool transparent = (color == -1);

            // draw pixel on the screen
            if (!transparent) {
                LCD.SetFontColor(color);
                drawScaledPixel(pos + Vector2(i, j) * scale);
            }

        }
    }
}

// checks whether screenpoint is overlapping the sprite (useful for collision)
bool Sprite::isPointWithin(const Vector2 &screenPoint) {
    // need to mod position because the simulator does that as well
    // eg: if you draw off the screen to the right it'll wrap around to the left
    float modX = (int)pos.x() % SCREEN_SIZE_X;
    float modY = (int)pos.y() % SCREEN_SIZE_Y;

    float leftBound = modX - size.x() * scale.x() * anchorPoint.x();
    float topBound = modY - size.y() * scale.y() * anchorPoint.y();
    float rightBound = leftBound + size.x() * scale.x();
    float bottomBound = topBound + size.y() * scale.y();

    float screenPointModX = (int)screenPoint.x() % SCREEN_SIZE_X;
    float screenPointModY = (int)screenPoint.y() % SCREEN_SIZE_Y;

    // check that the screenpoint is within the bounds, returning false if any of them fail
    return
        leftBound <= screenPointModX &&
        rightBound >= screenPointModX &&
        topBound <= screenPointModY &&
        bottomBound >= screenPointModY;
}

// draws a pixel the size of the sprite's scale
void Sprite::drawScaledPixel(Vector2 drawPos) const {
    drawPos -= size * scale * anchorPoint;

    for(int i = 0; i < scale.x(); i++) {
        for(int j = 0; j < scale.y(); j++) {
            LCD.DrawPixel(drawPos.x() + i, drawPos.y() + j);
        }
    }
}