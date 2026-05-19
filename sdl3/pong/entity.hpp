#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>

class Entity {
  public:
    float x, y, w, h, dx, dy, speed;

    Entity(SDL_Texture* sheet, float sx, float sy, float sw, float sh, float x, float y,
           float w, float h, float speed);
    ~Entity();

    void draw(SDL_Renderer *renderer);
    SDL_FRect getRect() const { return {x, y, w, h}; }
  private:
    SDL_Texture *texture;
    SDL_FRect src_rect;
      
};

#endif