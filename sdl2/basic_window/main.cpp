#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

App app;

int sdl_init() {
// initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
    }

    // create window
    const int screen_width = 800;
    const int screen_height = 480;
    app.window = SDL_CreateWindow(
        "basic window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screen_width,
        screen_height,
        SDL_WINDOW_SHOWN
    );

    if(!app.window) {
        SDL_Quit();
        return 1;
    }

    // create renderer
    app.renderer = SDL_CreateRenderer(
        app.window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if(!app.renderer) {
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        return 1;
    }

    return 0;
}

int main() {
    if (sdl_init() == 1) return 1;

    // main loop
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(app.renderer);

        // update screen
        SDL_RenderPresent(app.renderer);
    }

    // clean up resource
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();

    return 0;
}
