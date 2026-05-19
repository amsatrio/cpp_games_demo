#include "entity.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

Entity::Entity(SDL_Texture *sheet, float sx, float sy, float sw, float sh,
               float x, float y, float w, float h, float speed)
    : texture(sheet), x(x), y(y), w(w), h(h), speed(speed), dx(1.0f), dy(1.0f) {
    src_rect = {sx, sy, sw, sh};
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
}

Entity::~Entity() {
    if (texture)
        SDL_DestroyTexture(texture);
}

void Entity::draw(SDL_Renderer *renderer) {
    SDL_FRect rect = getRect();
    SDL_RenderTexture(renderer, texture, &src_rect, &rect);
}