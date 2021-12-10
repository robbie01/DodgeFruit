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

#ifdef __EMSCRIPTEN__
void em_loop_callback(void *data) {
    ScreenPtr &cur_screen = *(ScreenPtr*)data;
    
    auto ret = cur_screen->update();
    if (ret == ScreenUpdateReturn(std::monostate())) throw std::exception();
    else if (std::get<1>(ret) != nullptr) {
        cur_screen = std::move(std::get<1>(ret));
        cur_screen->init();
    }
}
#endif

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

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(em_loop_callback, &cur_screen, 0, true);
#else
    while (true) {
        auto ret = cur_screen->update();
        if (ret == ScreenUpdateReturn(std::monostate())) break;
        else if (std::get<1>(ret) != nullptr) {
            cur_screen = std::move(std::get<1>(ret));
            cur_screen->init();
        }
    }
#endif
    SDL_Quit();
    return 0;
}