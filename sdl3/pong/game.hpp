#ifndef GAME_HPP
#define GAME_HPP

#include <SDL3/SDL.h>
#include "entity.hpp"

enum class GameState { START, PLAYING, GAME_OVER };

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void run();
    void handleEvents();
    void update(float deltaTime);
    void render();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    GameState currentState;
    int screenWidth, screenHeight;

    Entity* player;
    Entity* ball;

    void resetBall();
};

#endif