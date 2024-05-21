#include "game/Game.h"

#include "ui/InventoryGui.h"

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
    inventory_gui.active = false;

    while (!WindowShouldClose()) {
        // Input events
        inventory_gui.process_input();

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
