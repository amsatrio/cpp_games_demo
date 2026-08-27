#ifndef GAME_HPP
#define GAME_HPP

#include <SDL3/SDL.h>
#include "entity.hpp"

enum class GameState { START, PLAYING, PAUSE, GAME_OVER };

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void run();
    void handleEvents(float dt);
    void update(float deltaTime);
    void render();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    GameState current_state;
    int screen_width, screen_height;

    Entity* player;
    Entity* ball;
    Entity* background;

    void resetBall();

    SDL_Texture* sprite_sheet;
};

#endif