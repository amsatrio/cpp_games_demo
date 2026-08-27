#include "game.hpp"
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[]) {
    Game game;
    if (game.init("SDL3 pong", 800, 480)) {
        game.run();
    }
    return 0;
}