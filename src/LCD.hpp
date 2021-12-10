#pragma once

#include <sstream>
#include <memory>
#include <functional>
#include "SDL.hpp"

template <typename T>
using SDLWrap = std::unique_ptr<T, std::function<void(T*)>>;

using pixel_t = std::uint32_t;

struct WindowSize {
    int w;
    int h;
};

class LCDClass {
    SDLWrap<SDL_Window> win;
    SDLWrap<SDL_Surface> sur;
public:
    pixel_t color, bgcolor;

    static LCDClass &getInstance();

    LCDClass(LCDClass const&) = delete;
    void operator=(LCDClass const&) = delete;

    LCDClass();

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

    WindowSize size();
};

constexpr pixel_t BLACK = 0;
constexpr pixel_t CYAN = 0x00FFFF;

#define LCD (LCDClass::getInstance())