#include "SDL.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include "Constants.hpp"
#include "LCD.hpp"

static unsigned char fontData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, // (space)
    0x00, 0x00, 0x5F, 0x00, 0x00, // !
    0x00, 0x07, 0x00, 0x07, 0x00, // "
    0x14, 0x7F, 0x14, 0x7F, 0x14, // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
    0x23, 0x13, 0x08, 0x64, 0x62, // %
    0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x1C, 0x22, 0x41, 0x00, // (
    0x00, 0x41, 0x22, 0x1C, 0x00, // )
    0x08, 0x2A, 0x1C, 0x2A, 0x08, // *
    0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x50, 0x30, 0x00, 0x00, // ,
    0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x60, 0x60, 0x00, 0x00, // .
    0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x00, 0x08, 0x14, 0x22, 0x41, // <
    0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x41, 0x22, 0x14, 0x08, 0x00, // >
    0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x32, 0x49, 0x79, 0x41, 0x3E, // @
    0x7E, 0x11, 0x11, 0x11, 0x7E, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x01, 0x01, // F
    0x3E, 0x41, 0x41, 0x51, 0x32, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x04, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x7F, 0x20, 0x18, 0x20, 0x7F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x03, 0x04, 0x78, 0x04, 0x03, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, // Z
    0x00, 0x00, 0x7F, 0x41, 0x41, // [
    0x02, 0x04, 0x08, 0x10, 0x20, // "\"
    0x41, 0x41, 0x7F, 0x00, 0x00, // ]
    0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x01, 0x02, 0x04, 0x00, // `
    0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x38, 0x44, 0x44, 0x48, 0x7F, // d
    0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x08, 0x14, 0x54, 0x54, 0x3C, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x20, 0x40, 0x44, 0x3D, 0x00, // j
    0x00, 0x7F, 0x10, 0x28, 0x44, // k
    0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x7C, 0x04, 0x18, 0x04, 0x78, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x38, 0x44, 0x44, 0x44, 0x38, // o
    0x7C, 0x14, 0x14, 0x14, 0x08, // p
    0x08, 0x14, 0x14, 0x18, 0x7C, // q
    0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x04, 0x3F, 0x44, 0x40, 0x20, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x0C, 0x50, 0x50, 0x50, 0x3C, // y
    0x44, 0x64, 0x54, 0x4C, 0x44, // z
    0x00, 0x08, 0x36, 0x41, 0x00, // {
    0x00, 0x00, 0x7F, 0x00, 0x00, // |
    0x00, 0x41, 0x36, 0x08, 0x00, // }
    0x08, 0x08, 0x2A, 0x1C, 0x08, // ->
    0x08, 0x1C, 0x2A, 0x08, 0x08  // <-
};

LCDClass::LCDClass() {
    SDL_Window *pwin;
    SDL_Renderer *pren;
    SDL_Texture *ptex;
    pwin = SDL_CreateWindow("Dodge Fruit", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE_X*PIXEL_SCALE, SCREEN_SIZE_Y*PIXEL_SCALE, 0);
    pren = SDL_CreateRenderer(pwin, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(pren, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    ptex = SDL_CreateTexture(pren, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING, SCREEN_SIZE_X, SCREEN_SIZE_Y);

    win = SDLWrap<SDL_Window>(pwin, SDL_DestroyWindow);
    ren = SDLWrap<SDL_Renderer>(pren, SDL_DestroyRenderer);
    tex = SDLWrap<SDL_Texture>(ptex, SDL_DestroyTexture);
}

LCDClass &LCDClass::getInstance() {
    static LCDClass instance;
    return instance;
}

void LCDClass::SetFontColor(int color) {
    this->color = color;
}

void LCDClass::SetBackgroundColor(int bgcolor) {
    this->bgcolor = bgcolor;
}

void LCDClass::DrawPixel(int x, int y) {
    SDL_Rect target { 
        .x = (x % SCREEN_SIZE_X),
        .y = (y % SCREEN_SIZE_Y),
        .w = 1, .h = 1 };
    //SDL_FillRect(SDL_GetWindowSurface(win.get()), &target, color);
    void *pixels;
    int pitch;
    SDL_LockTexture(tex.get(), &target, &pixels, &pitch);
    ((int*)(pixels))[0] = color | 0xFF000000;
    SDL_UnlockTexture(tex.get());
}

void LCDClass::Update() {
    //SDL_UpdateWindowSurface(win.get());
    SDL_RenderClear(ren.get());
    SDL_RenderCopy(ren.get(), tex.get(), NULL, NULL);
    SDL_RenderPresent(ren.get());
}

void LCDClass::Clear() {
    //SDL_FillRect(SDL_GetWindowSurface(win.get()), NULL, bgcolor);
    SDL_Surface *sur;
    SDL_LockTextureToSurface(tex.get(), NULL, &sur);
    SDL_FillRect(sur, NULL, bgcolor | 0xFF000000);
    SDL_UnlockTexture(tex.get());
}

void LCDClass::WriteAt(char c, int x, int y) {
    if (c > 125 || c < 32) {
        c = 32;
    }
    c -= 32;

    unsigned char charData[5];
    charData[0] = fontData[5 * c];
    charData[1] = fontData[5 * c + 1];
    charData[2] = fontData[5 * c + 2];
    charData[3] = fontData[5 * c + 3];
    charData[4] = fontData[5 * c + 4];

    x += 2;

    SDL_Rect tgt { .x = 0, .y = 0, .w = 2, .h = 2 };

    for (int col = 0; col < 5; col++)
    {
        for (int row = 0; row < 7; row++)
        {
            // If the current pixel is a 1 in the fontData bitmap
            if (((charData[col] >> row) & 0x01) == 1)
            {
                
                tgt.x = x + col*2;
                tgt.y = y + row*2;
                // Draw a 2x2 rectangle to represent each pixel since sizes are doubled
                //SDL_FillRect(SDL_GetWindowSurface(win.get()), &tgt, color);
                
                SDL_Surface *sur;
                SDL_LockTextureToSurface(tex.get(), &tgt, &sur);
                SDL_FillRect(sur, NULL, color | 0xFF000000);
                SDL_UnlockTexture(tex.get());
            }
        }
    }

}

void LCDClass::WriteAt(std::string s, int x, int y) {
    int i = 0;
    for (char c : s) {
        WriteAt(c, x+i++*12, y);
    }
}