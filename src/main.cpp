#include "SDL.hpp"

// paige libraries :)
#include "Screens.hpp"

// c++ libraries
#include <cstdlib>
#include <exception>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

int showStatsScreen();

void writeTime(float);
float playerHighScore();
int gameCount();

#ifndef __EMSCRIPTEN__
inline bool
#else
void
#endif
loop_inner(void *data) {
    ScreenPtr &cur_screen = *(ScreenPtr*)data;
    
    auto ret = cur_screen->update();
    if (ret == ScreenUpdateReturn(std::monostate()))
#ifndef __EMSCRIPTEN__
        return false;
#else
        throw std::exception();
#endif
    else if (std::get<1>(ret) != nullptr) {
        cur_screen = std::move(std::get<1>(ret));
        cur_screen->init();
    }
#ifndef __EMSCRIPTEN__
    return true;
#endif
}

/*
 * Entry point to the application
 */
int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    SDL_Init(SDL_INIT_VIDEO);
    std::srand(std::time(nullptr));

    ScreenPtr cur_screen(new MenuScreen);
    cur_screen->init();

#ifndef __EMSCRIPTEN__
    while (loop_inner(&cur_screen));
#else
    emscripten_set_main_loop_arg(loop_inner, &cur_screen, 0, true);
#endif
    SDL_Quit();
    return 0;
}