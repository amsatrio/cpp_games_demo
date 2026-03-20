#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

App app;

int sdl_init() {
    // SDL3 uses a more unified initialization
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 1;
    }

    // SDL_CreateWindow no longer needs position parameters unless you specifically want them
    // You can also chain the renderer creation or use the new combined call
    const int screen_width = 800;
    const int screen_height = 480;

    if (!SDL_CreateWindowAndRenderer("SDL3 Basic Window", screen_width, screen_height, 0, &app.window, &app.renderer)) {
        SDL_Quit();
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (sdl_init() == 1) return 1;

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // Draw a dark grey background
        SDL_SetRenderDrawColor(app.renderer, 33, 33, 33, 255);
        SDL_RenderClear(app.renderer);

        // Update screen
        SDL_RenderPresent(app.renderer);
    }

    // Clean up
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();

    return 0;
}