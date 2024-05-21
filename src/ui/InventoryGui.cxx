#include "ui/InventoryGui.h"

#include "game/GameDatabase.h"
#include "common/Math.h"
#include "ui/SlotInfo.h"

#include <raymath.h>

namespace ui {

InventoryGui::InventoryGui(game::Inventory &inventory) :
        sprite_window(graphics::Sprite("inventory.png")),
        inventory(inventory),
        active(false) {
}

InventoryGui::~InventoryGui() {
    for (auto *slot_icon: sprite_backpack_items) {
        delete slot_icon;
    }
    for (auto *slot_icon: sprite_belt_items) {
        delete slot_icon;
    }
}

void InventoryGui::draw() const {
    if (!active) {
        DrawText("Open inventory with [TAB], [ESCAPE], or [I].",
                 640 - 256, 360, 24, Color{0xff, 0xff, 0xff, 0x80});
        return;
    }

    sprite_window.draw();

    if (dragging.valid()) {
        const Rectangle origin_rect = get_slot_rect(dragging);
        DrawRectangleLinesEx(origin_rect, 2.f, ColorAlpha(BLUE, .5f));

        Vector2 mouse_pos = GetMousePosition();
        auto hovered_slot = get_hovered_slot(mouse_pos);
        if (hovered_slot.index != -1) {
            const Rectangle target_rect =
                    hovered_slot.type == SlotInfo::Backpack
                    ? get_backpack_slot_rect(hovered_slot.index)
                    : get_belt_slot_rect(hovered_slot.index);
            DrawRectangleLinesEx(target_rect, 2.f, ColorAlpha(YELLOW, .5f));
        }
    }

    for (ClickableIcon *item_icon: sprite_backpack_items) {
        if (item_icon == nullptr) {
            continue;
        }
        item_icon->draw();
    }

    for (ClickableIcon *item_icon: sprite_belt_items) {
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
    for (int slot_index = 0; slot_index < static_cast<int>(game::BACKPACK_SIZE); slot_index++) {
        game::ItemId item_id = inventory.backpack[slot_index];
        setup_slot(item_id, {
                slot_index,
                SlotInfo::Backpack
        });
    }

    // belt items
    for (int slot_index = 0; slot_index < static_cast<int>( game::BELT_SIZE); slot_index++) {
        game::ItemId item_id = inventory.belt[slot_index];
        setup_slot(item_id, {
                slot_index,
                SlotInfo::Belt
        });
    }
}

void InventoryGui::setup_slot(game::ItemId item_id, SlotInfo slot_info) {
    if (item_id == game::NoItem) {
        return;
    }

    const game::Item &item = game::GAME_DATABASE.items.get_item(item_id);

    ClickableIcon *icon_sprite;
    switch (slot_info.type) {
        case SlotInfo::Backpack:
            icon_sprite = sprite_backpack_items[slot_info.index] != nullptr
                          ? sprite_backpack_items[slot_info.index]
                          : new ClickableIcon("items/" + item.icon, SLOT_SIZE);
            sprite_backpack_items[slot_info.index] = icon_sprite;
            break;
        case SlotInfo::Belt:
            icon_sprite = sprite_belt_items[slot_info.index] != nullptr
                          ? sprite_belt_items[slot_info.index]
                          : new ClickableIcon("items/" + item.icon, SLOT_SIZE);
            sprite_belt_items[slot_info.index] = icon_sprite;
            break;
        case SlotInfo::None:
            throw;
    }

    if (icon_sprite->moved) {
        icon_sprite->sprite.scale = 0.75f;
        icon_sprite->moved = false;
    } else {
        icon_sprite->sprite.scale = 0.6f;
    }
    icon_sprite->sprite.offset = item.icon_offset;
    icon_sprite->sprite.position = get_slot_pivot(slot_info);

}

void InventoryGui::process_input() {
    if (IsKeyReleased(KeyboardKey::KEY_TAB) || IsKeyReleased(KeyboardKey::KEY_ESCAPE) ||
        IsKeyReleased(KeyboardKey::KEY_I)) {
        active = !active;
    }

    if (!active) {
        return;
    }

    bool dirty = false;

    // backpack items
    for (int slot_index = 0; slot_index < static_cast<int>(game::BACKPACK_SIZE); slot_index++) {
        dirty = process_slot_input({
                                           slot_index,
                                           SlotInfo::Backpack
                                   }) || dirty;
    }

    // belt items
    for (int slot_index = 0; slot_index < static_cast<int>(game::BELT_SIZE); slot_index++) {
        dirty = process_slot_input({
                                           slot_index,
                                           SlotInfo::Belt
                                   }) || dirty;
    }

    if (dirty) {
        sync();
    }
}

bool InventoryGui::process_slot_input(const SlotInfo slot_info) {
    if (!slot_info.valid()) {
        return false;
    }

    ClickableIcon *item_icon = nullptr;
    switch (slot_info.type) {
        case SlotInfo::Backpack:
            item_icon = sprite_backpack_items[slot_info.index];
            break;
        case SlotInfo::Belt:
            item_icon = sprite_belt_items[slot_info.index];
            break;
        case SlotInfo::None:
            throw;
    }

    if (item_icon == nullptr) {
        return false;
    }

    bool dirty = false;
    const Rectangle rect = item_icon->clickable_rect();
    const Vector2 mouse_pos = GetMousePosition();

    if (!dragging.valid()
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        && common::is_point_in_rect(mouse_pos, rect)) {
        dragging = slot_info;
        item_icon->sprite.scale = .75f;
    }

    if (dragging.index == slot_info.index && dragging.type == slot_info.type
        && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        SlotInfo hovered_slot = get_hovered_slot(mouse_pos);
        if (hovered_slot.valid()) {
            swap_slots(slot_info, hovered_slot);
            dirty = true;
        }
        dragging = {-1, SlotInfo::None};
    }

    if (!dragging.valid() && abs(item_icon->sprite.scale - .6f) > .001f) {
        item_icon->sprite.scale += (.6f - item_icon->sprite.scale) * GetFrameTime() * 24;
    }
    return dirty;
}

void InventoryGui::swap_slots(SlotInfo lhs, SlotInfo rhs) {
    TraceLog(LOG_INFO, "Move slot %d:%d to slot %d:%d", lhs.type, lhs.index, rhs.type, rhs.index);

    ClickableIcon **slot_container_lhs;
    ClickableIcon **slot_container_rhs;
    game::ItemId *item_container_lhs;
    game::ItemId *item_container_rhs;

    switch (lhs.type) {
        case SlotInfo::None:
            throw;
        case SlotInfo::Backpack:
            slot_container_lhs = sprite_backpack_items;
            item_container_lhs = inventory.backpack;
            break;
        case SlotInfo::Belt:
            slot_container_lhs = sprite_belt_items;
            item_container_lhs = inventory.belt;
            break;
    }

    switch (rhs.type) {
        case SlotInfo::None:
            throw;
        case SlotInfo::Backpack:
            slot_container_rhs = sprite_backpack_items;
            item_container_rhs = inventory.backpack;
            break;
        case SlotInfo::Belt:
            slot_container_rhs = sprite_belt_items;
            item_container_rhs = inventory.belt;
    }

    std::swap(item_container_lhs[lhs.index], item_container_rhs[rhs.index]);
    std::swap(slot_container_lhs[lhs.index], slot_container_rhs[rhs.index]);

    if (slot_container_lhs[lhs.index] != nullptr) {
        slot_container_lhs[lhs.index]->moved = true;
    }
    if (slot_container_rhs[rhs.index] != nullptr) {
        slot_container_rhs[rhs.index]->moved = true;
    }
}


} // ui
