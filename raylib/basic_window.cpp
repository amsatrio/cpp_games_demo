#include <raylib.h>

int main(void) {
    const int screen_width = 800;
    const int screen_height = 480;
    const int fps = 60;

    InitWindow(screen_width, screen_height, "basic window");
    SetTargetFPS(fps);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Hello World", 200, 200, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}