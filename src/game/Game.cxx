#include "game/Game.h"

#include "common/Math.h"
#include "graphics/Sprite.h"

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
    graphics::Sprite inventory_window("inventory.png");
    inventory_window.position = {640, 360};

    std::vector<graphics::Sprite *> items{};
    Vector2 origin{522, 216};
    Vector2 size{56, 56};
    Vector2 half_size = Vector2Scale(size, .5f);
    float margin = 4.f;
    size_t i = 0;
    for (const auto &itemId: _state.inventory.backpack) {
        if (itemId == NoItem) {
            continue;
        }
        const auto &item = GAME_DATABASE.items.get_item(itemId);

        items.emplace_back(new graphics::Sprite("items/" + item.icon));
        graphics::Sprite &icon = *items[i];
        icon.scale = 0.6f;

        const auto x = static_cast<float>(i % 6);
        const auto y = static_cast<float>(i / 6);
        const Vector2 local_pos = {(margin + size.x) * x, (margin + size.y) * y};
        icon.position = Vector2Add(origin, local_pos);

        i++;
    }

    std::cout << "items[" << items.size() << "]: " << items.data() << std::endl;

    bool inventory_open = true;

    while (!WindowShouldClose()) {
        // Input events
        for (auto *item: items) {
            Rectangle rect{
                    item->position.x - half_size.x, item->position.y - half_size.y,
                    size.x, size.y
            };
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                common::is_point_in_rect(GetMousePosition(), rect)) {
                item->scale = 1.f;
            } else {
                item->scale += (0.5f - item->scale) * GetFrameTime() * 8;
            }
        }
        if (IsKeyReleased(KeyboardKey::KEY_TAB) || IsKeyReleased(KeyboardKey::KEY_ESCAPE) ||
            IsKeyReleased(KeyboardKey::KEY_I)) {
            inventory_open = !inventory_open;
        }

        // Rendering
        BeginDrawing();

        // Clear Background
        ClearBackground(Color{0x20, 0x20, 0x20, 0xff});

        // Draw Sprites
        if (inventory_open) {
            inventory_window.draw();
            for (auto item: items) {
                item->draw();
                Rectangle rect{
                        item->position.x - half_size.x, item->position.y - half_size.y,
                        size.x, size.y
                };
                DrawRectangleRec(rect, ColorAlpha(RED, 0.5f));
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
