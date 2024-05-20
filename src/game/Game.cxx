#include "game/Game.h"

#include "common/Math.h"
#include "graphics/Sprite.h"
#include "ui/InventoryGui.h"

#include <iostream>
#include <raylib.h>
#include <raymath.h>

namespace game {

Game::Game() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(1280, 720, "UITest");

    // Debug quit
    SetExitKey(KEY_F8);
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
    ui::InventoryGui inventory_gui = ui::InventoryGui(_state.inventory);
    inventory_gui.sync();
    inventory_gui.active = true;

    while (!WindowShouldClose()) {
        // Input events
        for (auto *item: inventory_gui.sprite_backpack_items) {
            if (item == nullptr) {
                continue;
            }
            Rectangle rect = item->clickable_rect();
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                common::is_point_in_rect(GetMousePosition(), rect)) {
                item->icon.scale = .8f;
            } else if (abs(item->icon.scale - .5f) > .01f) {
                item->icon.scale += (.5f - item->icon.scale) * GetFrameTime() * 16;
            }
        }

        if (IsKeyReleased(KeyboardKey::KEY_TAB) || IsKeyReleased(KeyboardKey::KEY_ESCAPE) ||
            IsKeyReleased(KeyboardKey::KEY_I)) {
            inventory_gui.active = !inventory_gui.active;
        }

        // Rendering
        BeginDrawing();

        // Clear Background
        ClearBackground(Color{0x20, 0x20, 0x20, 0xff});

        // Draw Sprites
        inventory_gui.draw();

        DrawFPS(0, 720 - 16);
        EndDrawing();
    }
}

} // game
