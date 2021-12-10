#pragma once

#include <string>
#include "Vector.hpp"
#include "Constants.hpp"
#include "LCD.hpp"

class Sprite {
	pixel_t image[SPRITE_SIZE_X + 1][SPRITE_SIZE_Y + 1];

	void drawScaledPixel(Vector2) const;
public:
	Vector2 pos, size, scale, anchorPoint;

	Sprite(std::string, const Vector2&);
	void move(const Vector2&);
	void swapImage(std::string, const Vector2&);
	void draw();
	bool isPointWithin(const Vector2&);
};