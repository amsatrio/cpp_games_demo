#include "game.hpp"
#include <SDL3_image/SDL_image.h>
#include <algorithm>

Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), 
               currentState(GameState::START), player(nullptr), ball(nullptr) {}

Game::~Game() {
    delete player;
    delete ball;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::init(const char* title, int width, int height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) return false;

    screenWidth = width;
    screenHeight = height;

    if (!SDL_CreateWindowAndRenderer(title, width, height, 0, &window, &renderer)) {
        return false;
    }

    player = new Entity(renderer, "assets/img/paddle.png", 100, 400, 128, 16, 100);
    ball = new Entity(renderer, "assets/img/ball.png", 100, 200, 24, 24, 200);

    isRunning = true;
    return true;
}

void Game::resetBall() {
    ball->x = screenWidth / 2.0f;
    ball->y = 100.0f;
    ball->dx = 1.0f;
    ball->dy = 1.0f;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) isRunning = false;
        
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_SPACE) {
            if (currentState != GameState::PLAYING) {
                resetBall();
                currentState = GameState::PLAYING;
            }
        }
    }

    const bool* keys = SDL_GetKeyboardState(NULL);
    float dt = 0.016f;
    if (keys[SDL_SCANCODE_LEFT]) player->x -= player->speed * dt;
    if (keys[SDL_SCANCODE_RIGHT]) player->x += player->speed * dt;
}

void Game::update(float dt) {
    if (currentState != GameState::PLAYING) return;

    ball->x += ball->dx * ball->speed * dt;
    ball->y += ball->dy * ball->speed * dt;

    // Boundary Collisions
    if (ball->x < 0 || ball->x + ball->w > screenWidth) ball->dx *= -1;
    if (ball->y < 0) ball->dy *= -1;
    if (ball->y + ball->h > screenHeight) currentState = GameState::GAME_OVER;

    // Paddle Collision
    SDL_FRect pRect = player->getRect();
    SDL_FRect bRect = ball->getRect();
    if (SDL_HasRectIntersectionFloat(&pRect, &bRect) && ball->dy > 0) {
        ball->dy *= -1;
        ball->y = player->y - ball->h;
    }
}
 
void Game::render() {
    SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
    SDL_RenderClear(renderer);

    player->draw(renderer);
    ball->draw(renderer);

    SDL_RenderPresent(renderer);
}

void Game::run() {
    Uint64 lastTime = SDL_GetTicks();
    while (isRunning) {
        Uint64 currentTime = SDL_GetTicks();
        float dt = std::min((currentTime - lastTime) / 1000.0f, 0.1f);
        lastTime = currentTime;

        handleEvents();
        update(dt);
        render();
    }
}