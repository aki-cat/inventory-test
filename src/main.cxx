// Main

#include "graphics/Sprite.h"

#include <raylib.h>

void run_loop() {
    graphics::Sprite inventory_window = graphics::Sprite("assets/inventory.png");
    graphics::Sprite bucket = graphics::Sprite("assets/items/bucket.png");

    while (!WindowShouldClose()) {
        // Debug quit
        if (IsKeyPressed(KeyboardKey::KEY_F8)) {
            break;
        }

        // Rendering
        BeginDrawing();

        ClearBackground(Color{0x20, 0x20, 0x20, 0xff});
        inventory_window.draw(0, 0);
        bucket.draw(32, 32);

        EndDrawing();
    }
}

int main() {
    InitWindow(1280, 720, "UITest");
    run_loop();
    CloseWindow();
    return 0;
}