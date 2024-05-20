#include "ui/InventoryGui.h"

#include "game/GameDatabase.h"
#include "common/Math.h"

#include <raymath.h>

namespace ui {

static constexpr Vector2 SLOT_SIZE{56.f, 56.f};
static constexpr Vector2 BACKPACK_SLOTS_ORIGIN{522, 216};
static constexpr Vector2 BACKPACK_SLOTS_MARGIN = {4.f, 2.f};

int get_hovered_slot(Vector2 point);
Rectangle get_slot_rect(int slot_index);
Vector2 get_slot_pivot(int slot_index);

InventoryGui::InventoryGui(game::Inventory &inventory) :
        sprite_window(graphics::Sprite("inventory.png")),
        inventory(inventory),
        active(false) {
}

InventoryGui::~InventoryGui() {
    for (auto &sprite_backpack_item: sprite_backpack_items) {
        delete sprite_backpack_item;
    }
}

void InventoryGui::draw() const {
    if (!active) {
        DrawText("Open inventory with [TAB], [ESCAPE], or [I].",
                 640 - 256, 360, 24, Color{0xff, 0xff, 0xff, 0x80});
        return;
    }

    sprite_window.draw();

    if (dragging != -1) {
        const Rectangle origin_rect = get_slot_rect(dragging);
        DrawRectangleLinesEx(origin_rect, 2.f, ColorAlpha(BLUE, .5f));

        Vector2 mouse_pos = GetMousePosition();
        int hovered_slot_index = get_hovered_slot(mouse_pos);
        if (hovered_slot_index != -1) {
            const Rectangle target_rect = get_slot_rect(hovered_slot_index);
            DrawRectangleLinesEx(target_rect, 2.f, ColorAlpha(YELLOW, .5f));
        }
    };

    for (ClickableIcon *item_icon: sprite_backpack_items) {
        if (item_icon == nullptr) {
            continue;
        }
        item_icon->draw();
    }
}

/**
 * Syncs visuals with actual inventory.
 * Slow and messes with memory, so shouldn't be called frequently.
 */
void InventoryGui::sync() {
    sprite_window.position = {640, 360};

    // backpack items
    for (int slot_index = 0; slot_index < static_cast<int>( game::BACKPACK_SIZE); slot_index++) {
        game::ItemId itemId = inventory.backpack[slot_index];

        if (itemId == game::NoItem) {
            continue;
        }

        const game::Item &item = game::GAME_DATABASE.items.get_item(itemId);

        ClickableIcon *icon_sprite =
                sprite_backpack_items[slot_index] != nullptr
                ? sprite_backpack_items[slot_index]
                : new ClickableIcon("items/" + item.icon, {56, 56});

        sprite_backpack_items[slot_index] = icon_sprite;

        if (icon_sprite->moved) {
            icon_sprite->sprite.scale = 0.75f;
            icon_sprite->moved = false;
        } else {
            icon_sprite->sprite.scale = 0.6f;
        }
        icon_sprite->sprite.offset = item.icon_offset;
        icon_sprite->sprite.position = get_slot_pivot(slot_index);
    }
}

void InventoryGui::process_input() {
    if (IsKeyReleased(KeyboardKey::KEY_TAB) || IsKeyReleased(KeyboardKey::KEY_ESCAPE) ||
        IsKeyReleased(KeyboardKey::KEY_I)) {
        active = !active;
    }

    bool dirty = false;
    for (int slot_index = 0; slot_index < static_cast<int>(game::BACKPACK_SIZE); slot_index++) {
        auto *item_icon = sprite_backpack_items[slot_index];
        if (item_icon == nullptr) {
            continue;
        }

        Rectangle rect = item_icon->clickable_rect();
        Vector2 mouse_pos = GetMousePosition();

        if (dragging == -1
            && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
            && common::is_point_in_rect(mouse_pos, rect)) {
            dragging = slot_index;
            item_icon->sprite.scale = .75f;
        }

        if (dragging == slot_index
            && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            int hovered_slot_index = get_hovered_slot(mouse_pos);
            if (hovered_slot_index >= 0) {
                game::ItemId this_item_id = inventory.backpack[slot_index];
                inventory.backpack[slot_index] = inventory.backpack[hovered_slot_index];
                inventory.backpack[hovered_slot_index] = this_item_id;

                ClickableIcon *hovered_icon = sprite_backpack_items[hovered_slot_index];
                sprite_backpack_items[slot_index] = hovered_icon;
                sprite_backpack_items[hovered_slot_index] = item_icon;
                if (hovered_icon != nullptr) {
                    hovered_icon->moved = true;
                }
                item_icon->moved = true;
                dirty = true;
            }
            TraceLog(LOG_INFO, "Move slot#%d to slot#%d", slot_index, hovered_slot_index);
            dragging = -1;
        }

        if (dragging == -1 && abs(item_icon->sprite.scale - .6f) > .01f) {
            item_icon->sprite.scale += (.6f - item_icon->sprite.scale) * GetFrameTime() * 16;
        }
    }

    if (dirty) {
        sync();
    }
}

int get_hovered_slot(Vector2 point) {
    for (int slot_index = 0; slot_index < static_cast<int>(game::BACKPACK_SIZE); slot_index++) {
        const Rectangle rect = get_slot_rect(slot_index);

        if (common::is_point_in_rect(point, rect)) {
            return slot_index;
        }
    }

    return -1;
}

Rectangle get_slot_rect(int slot_index) {
    const Vector2 slot_pivot = get_slot_pivot(slot_index);
    return {
            slot_pivot.x - SLOT_SIZE.x * 0.5f, slot_pivot.y - SLOT_SIZE.y * 0.5f,
            SLOT_SIZE.x, SLOT_SIZE.y
    };
}

Vector2 get_slot_pivot(int slot_index) {
    const int col = slot_index % 6;
    const int row = slot_index / 6;
    Vector2 slot_position = {(BACKPACK_SLOTS_MARGIN.x + SLOT_SIZE.x) * static_cast<float>(col),
                             (BACKPACK_SLOTS_MARGIN.y+ SLOT_SIZE.y) * static_cast<float>(row)};

    return Vector2Add(slot_position, BACKPACK_SLOTS_ORIGIN);
}

} // ui
