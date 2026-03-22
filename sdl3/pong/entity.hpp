#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL3/SDL.h>

class Entity {
  public:
    float x, y, w, h;
    float dx, dy, speed;
    SDL_Texture *texture;

    Entity(SDL_Renderer *renderer, const char *filePath, float x, float y,
           float w, float h, float speed);
    ~Entity();

    void draw(SDL_Renderer *renderer);
    SDL_FRect getRect() const { return {x, y, w, h}; }
};

#endif