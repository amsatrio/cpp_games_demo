#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>

struct SDLState {
    SDL_Renderer *renderer;
    SDL_Window *window;
};

SDLState state;

void cleanup(SDLState &state);

int init();

int main(int argc, char* argv[]) {
    if (init() == 1) return 1;

    bool quit = false;
    SDL_Event event{0};

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                quit = true;
                break;
            }
        }

        SDL_SetRenderDrawColor(state.renderer, 33, 33, 33, 255);
        SDL_RenderClear(state.renderer);

        SDL_RenderPresent(state.renderer);
    }

    cleanup(state);

    return 0;
}

int init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 1;
    }

    const int screen_width = 800;
    const int screen_height = 480;

    if (!SDL_CreateWindowAndRenderer("SDL3 Basic Window", screen_width, screen_height, 0, &state.window, &state.renderer)) {
        SDL_Quit();
        return 1;
    }

    return 0;
}

void cleanup(SDLState &state) {
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}