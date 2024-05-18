// Main

#include "graphics/Sprite.h"
#include "graphics/Drawable.h"

#include <raylib.h>

void run_loop() {
    graphics::Drawable inventory_window("assets/inventory.png");
    graphics::Drawable bucket("assets/items/bucket.png");
    bucket.position.x += 256.f;
    bucket.position.y += 32.f;
    bucket.scale = 0.8f;

    while (!WindowShouldClose()) {
        // Debug quit
        if (IsKeyPressed(KeyboardKey::KEY_F8)) {
            break;
        }

        // Rendering
        BeginDrawing();

        // Clear Background
        ClearBackground(Color{0x20, 0x20, 0x20, 0xff});

        // Draw Sprites
        inventory_window.draw();
        bucket.draw();

        EndDrawing();
    }
}

int main() {
    InitWindow(1280, 720, "UITest");
    run_loop();
    CloseWindow();
    return 0;
}
