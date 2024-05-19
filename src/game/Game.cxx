#include "game/Game.h"
#include "graphics/Drawable.h"
#include "common/Math.h"

#include <raylib.h>
#include <iostream>

namespace game {

Game::Game() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_BORDERLESS_WINDOWED_MODE);
    InitWindow(1280, 720, "UITest");
}

Game::~Game() {
    CloseWindow();
}

void Game::init() {
    std::vector<ItemId> all_items = GAME_DATABASE.items.all_ids();

    for (size_t slot_index = 0; slot_index < all_items.size(); slot_index++) {
        const auto &item_id = all_items[slot_index];
        _state.inventory.backpack[slot_index] = item_id;
    }
}

void Game::run() {
    graphics::Drawable inventory_window("assets/inventory.png");
    inventory_window.position = {640, 360};

    graphics::Drawable item0("assets/items/bucket.png");
    item0.position = {240, 104};

    bool inventory_open = false;

    while (!WindowShouldClose()) {
        // Debug quit
        if (IsKeyPressed(KeyboardKey::KEY_F8)) {
            break;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            common::is_point_in_rect(GetMousePosition(), item0.rect())) {
            item0.scale = 1.1f;
        } else {
            item0.scale += (1.0f - item0.scale) * GetFrameTime() * 50;
        }

        // Rendering
        BeginDrawing();

        // Clear Background
        ClearBackground(Color{0x20, 0x20, 0x20, 0xff});

        // Draw Sprites
        if (IsKeyReleased(KeyboardKey::KEY_TAB) || IsKeyReleased(KeyboardKey::KEY_ESCAPE) ||
            IsKeyReleased(KeyboardKey::KEY_I)) {
            inventory_open = !inventory_open;
        }

        if (inventory_open) {
            inventory_window.draw();
            item0.draw();
        } else {
            DrawText("Open inventory with [TAB], [ESCAPE], or [I].",
                     640 - 256, 360, 24, Color{0xff, 0xff, 0xff, 0x80});
        }

        DrawFPS(0, 720 - 16);
        EndDrawing();
    }
}

} // game
