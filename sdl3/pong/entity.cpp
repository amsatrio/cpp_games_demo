#include "entity.hpp"
#include <SDL3_image/SDL_image.h>

Entity::Entity(SDL_Renderer* renderer, const char* filePath, float x, float y, float w, float h, float speed)
    : x(x), y(y), w(w), h(h), speed(speed), dx(1.0f), dy(1.0f) {
    texture = IMG_LoadTexture(renderer, filePath);
}

Entity::~Entity() {
    if (texture) SDL_DestroyTexture(texture);
}

void Entity::draw(SDL_Renderer* renderer) {
    SDL_FRect rect = getRect();
    SDL_RenderTexture(renderer, texture, nullptr, &rect);
}