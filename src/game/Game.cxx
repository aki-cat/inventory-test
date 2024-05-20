#include "game/Game.h"
#include "graphics/Drawable.h"
#include "common/Math.h"

#include <raylib.h>
#include <iostream>

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
    graphics::Drawable inventory_window("assets/inventory.png");
    inventory_window.position = {640, 360};

    std::vector<graphics::Drawable *> items{};
    Vector2 origin{522, 216};
    float margin = 4.f;
    size_t i = 0;
    for (const auto &itemId: _state.inventory.backpack) {
        if (itemId == NoItem) {
            continue;
        }
        const auto &item = GAME_DATABASE.items.get_item(itemId);

        items.emplace_back(new graphics::Drawable("assets/items/" + item.icon));
        graphics::Drawable &icon = *items[i];
        icon.size = {56, 56};
        icon.scale = 0.6f;

        const auto x = static_cast<float>(i % 6);
        const auto y = static_cast<float>(i / 6);
        const Vector2 local_pos = {(margin + icon.size.x) * x, (margin + icon.size.y) * y};
        std::cout << "dimensions | width: " << icon.size.x << ", height: " << icon.size.y
                  << std::endl;
        std::cout << "(col,row) | x: " << x << ", y: " << y << std::endl;
        std::cout << "local_pos | x: " << local_pos.x << ", y: " << local_pos.y << std::endl;
        icon.position = Vector2Add(origin, local_pos);

        const auto rect = icon.rect();
        std::cout << "icon.position | x: " << icon.position.x << ", y: " << icon.position.y
                  << std::endl;
        std::cout << "icon.scale | x: " << icon.scale << ", icon.rotation: " << icon.rotation
                  << std::endl;
        std::cout << "icon.rect | x: " << rect.x << ", y: " << rect.y << std::endl;

        i++;
    }

    std::cout << "items[" << items.size() << "]: " << items.data() << std::endl;

    bool inventory_open = true;

    while (!WindowShouldClose()) {
        for (auto *item: items) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                common::is_point_in_rect(GetMousePosition(), item->rect())) {
                item->scale = 1.f;
            } else {
                item->scale += (0.5f - item->scale) * GetFrameTime() * 8;
            }
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
            float cr = 0.f;
            for (auto item: items) {
                cr += 1.f;
                item->draw();
            }
        } else {
            DrawText("Open inventory with [TAB], [ESCAPE], or [I].",
                     640 - 256, 360, 24, Color{0xff, 0xff, 0xff, 0x80});
        }

        DrawFPS(0, 720 - 16);
        EndDrawing();
    }

    for (auto *item: items) {
        delete item;
    }
    items.clear();
}

} // game
