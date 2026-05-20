#include "game.hpp"
#include <SDL3_image/SDL_image.h>
#include <algorithm>

Game::Game()
    : window(nullptr), renderer(nullptr), isRunning(false),
      current_state(GameState::START), player(nullptr), ball(nullptr) {}

Game::~Game() {
    delete player;
    delete ball;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::init(const char *title, int width, int height) {
    if (!SDL_Init(SDL_INIT_VIDEO))
        return false;

    screen_width = width;
    screen_height = height;

    if (!SDL_CreateWindowAndRenderer(title, width, height, 0, &window,
                                     &renderer)) {
        return false;
    }

    sprite_sheet = IMG_LoadTexture(renderer, "assets/img/sprite-sheet.png");
    player = new Entity(sprite_sheet, 0, 0, 16, 16, 100, screen_height - 100, 128, 16, 100);
    ball = new Entity(sprite_sheet, 16, 0, 16, 16, screen_width / 2.0f, 100.0f,
                      24, 24, 200);
    background = new Entity(sprite_sheet, 32, 0, 16, 16, 0, 0, screen_width, screen_height, 0);

    isRunning = true;
    return true;
}

void Game::resetBall() {
    ball->x = screen_width / 2.0f;
    ball->y = 100.0f;
    ball->dx = 1.0f;
    ball->dy = 1.0f;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT)
            isRunning = false;

        if (event.type == SDL_EVENT_KEY_DOWN &&
            event.key.scancode == SDL_SCANCODE_SPACE) {
            if (current_state == GameState::START || current_state == GameState::GAME_OVER) {
                resetBall();
                current_state = GameState::PLAYING;
            } else if (current_state == GameState::PLAYING) {
                current_state = GameState::PAUSE;
            } else if (current_state == GameState::PAUSE) {
                current_state = GameState::PLAYING;
            }
        }
    }

    const bool *keys = SDL_GetKeyboardState(NULL);
    float dt = 0.016f;
    if (keys[SDL_SCANCODE_LEFT] && player->x > 0)
        player->x -= player->speed * dt;
    if (keys[SDL_SCANCODE_RIGHT] && player->x + player->w < screen_width)
        player->x += player->speed * dt;
}

void Game::update(float dt) {
    if (current_state != GameState::PLAYING)
        return;

    ball->x += ball->dx * ball->speed * dt;
    ball->y += ball->dy * ball->speed * dt;

    // Boundary Collisions
    if (ball->x < 0 || ball->x + ball->w > screen_width)
        ball->dx *= -1;
    if (ball->y < 0)
        ball->dy *= -1;
    if (ball->y + ball->h > screen_height)
        current_state = GameState::GAME_OVER;

    // Paddle ball Collision
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

    background->draw(renderer);

    if (current_state == GameState::START || current_state == GameState::PAUSE) {
        player->draw(renderer);
        ball->draw(renderer);
    } else if (current_state == GameState::PLAYING) {
        player->draw(renderer);
        ball->draw(renderer);
    }

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