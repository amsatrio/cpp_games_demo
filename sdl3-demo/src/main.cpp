#include <vector>
#include <string>
#include <array>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include <glm/glm.hpp>

#include "gameobject.h"

struct SDLState {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    int width, height, logW, logH;
    const bool *keys;
    SDLState() : keys(SDL_GetKeyboardState(nullptr)) {}
};

const size_t LAYER_INDEX_LEVEL = 0;
const size_t LAYER_INDEX_CHARACTERS = 1;
const int MAP_ROWS = 5;
const int MAP_COLS = 50;
const int TILE_SIZE = 32;

struct GameState {
    std::array<std::vector<GameObject>, 2> layers;
    int playerIndex;
    GameState() {
        playerIndex = 0;
    }
};

struct Resources {
    const int ANIMATION_PLAYER_IDLE = 0;
    const int ANIMATION_PLAYER_RUN = 1;
    std::vector<Animation> playerAnimations;
    std::vector<SDL_Texture *> textures;
    SDL_Texture *textureIdle, *textureRun;

    SDL_Texture *loadTexture(SDL_Renderer *renderer, const std::string &filepath) {
        SDL_Texture *texture = IMG_LoadTexture(renderer, filepath.c_str());
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
        textures.push_back(texture);
        return texture;
    }

    void load(SDLState &state) {
        playerAnimations.resize(5);
        playerAnimations[ANIMATION_PLAYER_IDLE] = Animation(8, 1.6f);
        playerAnimations[ANIMATION_PLAYER_RUN] = Animation(4, 1.6f);
        textureIdle = loadTexture(state.renderer, "./assets/image/idle.png");
        textureRun = loadTexture(state.renderer, "./assets/image/run.png");
    }
    void unload() {
        for(SDL_Texture *texture : textures) {
            SDL_DestroyTexture(texture);
        }
    }
};

// SDL_RenderTextureRotated + SDL_FLIP_HORIZONTAL shears sprites on GPU renderers
// (OpenGL/Vulkan); use affine corners or plain RenderTexture instead.
void drawSprite(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect &src,
                const SDL_FRect &dst, bool flipHorizontal) {
    if (!flipHorizontal) {
        SDL_RenderTexture(renderer, texture, &src, &dst);
        return;
    }

    const SDL_FPoint origin{dst.x + dst.w, dst.y};
    const SDL_FPoint right{dst.x, dst.y};
    const SDL_FPoint down{dst.x + dst.w, dst.y + dst.h};
    SDL_RenderTextureAffine(renderer, texture, &src, &origin, &right, &down);
}

bool initialize(SDLState &state);
void cleanup(SDLState &state);
void drawObject(const SDLState &state, GameState &gameState, GameObject &gameObject, float deltaTime);
void update(const SDLState &state, GameState &gameSatet, Resources &resources, GameObject &gameObject, float deltaTime);
void createTiles(const SDLState &state, GameState &gs, const Resources &res);

int main(int argc, char *argv[]) {
    SDLState state;
    state.width = 1600;
    state.height = 900;
    state.logH = 320;
    state.logW = 640;

    if(!initialize(state)){
        return 1;
    }

    // load game assets
    Resources resources;
    resources.load(state);

    // setup game data
    GameState gameState;
    createTiles(state, gameState, resources);
    uint64_t prevTime = SDL_GetTicks();

    // game loop
    bool running = true;
    while (running) {
        uint64_t nowTime = SDL_GetTicks();
        float deltaTime = (nowTime - prevTime) / 1000.0f;

        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: {
                    running = false;
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: {
                    state.width = event.window.data1;
                    state.height = event.window.data2;
                    break;
                }
            }
        }

        for(auto &layer : gameState.layers) {
            for (auto &obj: layer) {
                update(state, gameState, resources, obj, deltaTime);
                if(obj.currentAnimation != -1) { obj.animations[obj.currentAnimation].step(deltaTime); }
            }
        }

        // performs drawing command
        SDL_SetRenderDrawColor(state.renderer, 33, 33, 33, 255);
        SDL_RenderClear(state.renderer);

        for(auto &layer : gameState.layers) {
            for (auto &obj: layer) {
                drawObject(state, gameState, obj, deltaTime);
            }
        }

        // swap buffers and present
        SDL_RenderPresent(state.renderer);

        // SDL_Delay(16);
        prevTime = nowTime;
    }

    // cleanup
    resources.unload();
    cleanup(state);
    return 0;
}

bool initialize(SDLState &state) {
    // init sdl3
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                                 "Error initializing SDL3", nullptr);
        return false;
    }

    // init window and render
    if (!SDL_CreateWindowAndRenderer("SDL Demo", state.width, state.height, 0, &state.window,
                                     &state.renderer)) {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                                 "Error creating window", nullptr);
        SDL_Quit();
        return false;
    }

    // configure presentation
    SDL_SetRenderLogicalPresentation(state.renderer, state.logW, state.logH, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return true;
}

void drawObject(const SDLState &state, GameState &gameState, GameObject &gameObject, float deltaTime) {
        const int spriteSize = 32;
        float srcX = gameObject.currentAnimation != -1 ? gameObject.animations[gameObject.currentAnimation].currentFrame() * spriteSize : 0.0f;
        SDL_FRect srcrect{.x = srcX, .y = 0, .w = spriteSize, .h = spriteSize};
        SDL_FRect dstrect{.x = gameObject.position.x, .y = gameObject.position.y, .w = spriteSize, .h = spriteSize};
        drawSprite(state.renderer, gameObject.texture, srcrect, dstrect, gameObject.direction == -1);
}

void update(const SDLState &state, GameState &gameSatet, Resources &resources, GameObject &gameObject, float deltaTime) {
    if(gameObject.type == ObjectType::player){
        float currentDirection = 0;
        if(state.keys[SDL_SCANCODE_A]) {
            currentDirection += -1;
        } 
        if (state.keys[SDL_SCANCODE_D]) {
            currentDirection += 1;
        }
        if(currentDirection) {
            gameObject.direction = currentDirection;
        }

        switch (gameObject.data.player.state) {
            case PlayerState::idle: 
                if(currentDirection) {
                    gameObject.data.player.state = PlayerState::running;
                    gameObject.texture = resources.textureRun;
                    gameObject.currentAnimation = resources.ANIMATION_PLAYER_RUN;
                } else {
                    // decelerate
                    if (gameObject.velocity.x) {
                        const float factor = gameObject.velocity.x > 0 ? -1.5f : 1.5f;
                        float amount = factor * gameObject.acceleration.x * deltaTime;
                        if(std::abs(gameObject.velocity.x) < std::abs(amount)) { gameObject.velocity.x = 0; }
                        else { gameObject.velocity.x += amount; }
                    }
                }
                break;
            case PlayerState::running:
                if(!currentDirection) {
                    gameObject.data.player.state = PlayerState::idle;
                    gameObject.texture = resources.textureIdle;
                    gameObject.currentAnimation = resources.ANIMATION_PLAYER_IDLE;
                }
                break;
            case PlayerState::jumping:
                break;
        }
        // add acceleration to velocity
        gameObject.velocity += currentDirection * gameObject.acceleration * deltaTime;
        if(std::abs(gameObject.velocity.x) > gameObject.maxSpeedX) {
            gameObject.velocity.x = currentDirection * gameObject.maxSpeedX;
        }
        // add velocity to position
        gameObject.position += gameObject.velocity * deltaTime;
    }
}

void cleanup(SDLState &state) {
    if (state.renderer) {
        SDL_DestroyRenderer(state.renderer);
    }
    if (state.window) {
        SDL_DestroyWindow(state.window);
    }
    SDL_Quit();
}


void createTiles(const SDLState &state, GameState &gs, const Resources &res) {
    /*
        1 - ground
        2 - panel
        3 - enemy
        4 - player
        5 - grass
        6 - brick
    */
    short map[MAP_ROWS][MAP_COLS] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
    for(int r = 0; r < MAP_ROWS; r++){
        for(int c = 0; c < MAP_COLS; c++) {
            switch (map[r][c]) {
                case 4: //player
                    GameObject player;
                    player.position = glm::vec2(
                        c * TILE_SIZE,
                        state.logH - (MAP_ROWS - r) * TILE_SIZE
                    );
                    player.type = ObjectType::player;
                    player.data.player = PlayerData();
                    player.texture = res.textureIdle;
                    player.animations = res.playerAnimations;
                    player.acceleration = glm::vec2(300, 0);
                    player.maxSpeedX = 100;
                    player.currentAnimation = res.ANIMATION_PLAYER_IDLE;
                    gs.layers[LAYER_INDEX_CHARACTERS].push_back(player);
                    break;
            }
        }
    }
}