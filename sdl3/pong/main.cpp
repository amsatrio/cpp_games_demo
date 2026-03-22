#include "game.hpp"

int main(int argc, char* argv[]) {
    Game game;
    if (game.init("SDL3 Breakout OOP", 800, 480)) {
        game.run();
    }
    return 0;
}