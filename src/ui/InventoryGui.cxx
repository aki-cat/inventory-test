#include "ui/InventoryGui.h"

#include "game/GameDatabase.h"
#include "common/Math.h"

#include <raymath.h>

namespace ui {

constexpr Vector2 SLOT_SIZE{56.f, 56.f};

InventoryGui::InventoryGui(game::Inventory &inventory) :
        sprite_window(graphics::Sprite("inventory.png")),
        inventory(inventory),
        active(false) {
}

InventoryGui::~InventoryGui() {
    for (auto &sprite_backpack_item: sprite_backpack_items) {
        if (sprite_backpack_item != nullptr) {
            delete sprite_backpack_item;
        }
    }
}

void InventoryGui::draw() const {
    if (!active) {
        DrawText("Open inventory with [TAB], [ESCAPE], or [I].",
                 640 - 256, 360, 24, Color{0xff, 0xff, 0xff, 0x80});
        return;
    }

    sprite_window.draw();
    for (auto sprite_item: sprite_backpack_items) {
        if (sprite_item == nullptr) {
            continue;
        }
        sprite_item->draw();
    }
}

/**
 * Syncs visuals with actual inventory.
 * Slow and messes with memory, so shouldn't be called frequently.
 */
void InventoryGui::sync() {
    const Vector2 origin{522, 216};
    const float margin = 4.f;

    sprite_window.position = {640, 360};

    // backpack items
    for (size_t slot_index = 0; slot_index < game::BACKPACK_SIZE; slot_index++) {
        game::ItemId itemId = inventory.backpack[slot_index];
        if (itemId == game::NoItem) {
            continue;
        }
        const game::Item &item = game::GAME_DATABASE.items.get_item(itemId);

        if (sprite_backpack_items[slot_index] != nullptr) {
            delete sprite_backpack_items[slot_index];
            sprite_backpack_items[slot_index] = nullptr;
            continue;
        }

        auto &icon_sprite = *new ClickableIcon("items/" + item.icon, {56, 56});
        sprite_backpack_items[slot_index] = &icon_sprite;

        icon_sprite.sprite.scale = 0.6f;
        icon_sprite.sprite.offset = item.icon_offset;

        const auto x = static_cast<float>(slot_index % 6);
        const auto y = static_cast<float>(slot_index / 6);
        const Vector2 local_pos = {(margin + SLOT_SIZE.x) * x, (margin + SLOT_SIZE.y) * y};
        icon_sprite.sprite.position = Vector2Add(origin, local_pos);
    }
}

void InventoryGui::process_input() {
    if (IsKeyReleased(KeyboardKey::KEY_TAB) || IsKeyReleased(KeyboardKey::KEY_ESCAPE) ||
        IsKeyReleased(KeyboardKey::KEY_I)) {
        active = !active;
    }

    for (auto *item_icon: sprite_backpack_items) {
        if (item_icon == nullptr) {
            continue;
        }

        Rectangle rect = item_icon->clickable_rect();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            common::is_point_in_rect(GetMousePosition(), rect)) {
            item_icon->sprite.scale = .8f;
        } else if (abs(item_icon->sprite.scale - .5f) > .01f) {
            item_icon->sprite.scale += (.5f - item_icon->sprite.scale) * GetFrameTime() * 16;
        }
    }
}

} // ui
