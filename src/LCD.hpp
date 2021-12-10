#pragma once

#include <sstream>
#include "SDL.hpp"

class LCDClass {
    LCDClass();
    ~LCDClass();
    SDL_Window *win;
    SDL_Surface *scr;
public:
    int color, bgcolor;

    static LCDClass &getInstance();

    LCDClass(LCDClass const&) = delete;
    void operator=(LCDClass const&) = delete;

    void SetFontColor(int);
    void SetBackgroundColor(int);
    void Clear();
    void DrawPixel(int, int);
    void Update();

    void WriteAt(char, int, int);
    void WriteAt(std::string, int, int);

    template <class T>
    void WriteAt(T z, int x, int y) {
        std::ostringstream out;
        out.precision(3);
        out << std::fixed << z;
        WriteAt(out.str(), x, y);
    }
};

constexpr int BLACK = 0;
constexpr int CYAN = 0x00FFFF;

#define LCD (LCDClass::getInstance())