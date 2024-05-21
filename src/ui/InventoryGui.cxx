#include "ui/InventoryGui.h"

#include "game/GameDatabase.h"
#include "common/Math.h"

#include <raymath.h>
#include <sstream>
#include <iostream>

namespace ui {

static constexpr Vector2 SLOT_SIZE{56.f, 56.f};
static constexpr Vector2 BACKPACK_SLOTS_ORIGIN{522, 216};
static constexpr Vector2 BACKPACK_SLOTS_MARGIN = {4.f, 2.f};
static constexpr int BACKPACK_COLUMN_COUNT = 6;
static constexpr Vector2 BELT_SLOTS_ORIGIN{390., 512.};
static constexpr Vector2 BELT_SLOTS_MARGIN = {4.f, 2.f};

// helper math functions
SlotInfo get_hovered_slot(Vector2 point);
Rectangle get_slot_rect(SlotInfo slot_info);
Vector2 get_slot_pivot(SlotInfo slot_info);

Rectangle get_belt_slot_rect(int slot_index);
Vector2 get_belt_slot_pivot(int slot_index);

Rectangle get_backpack_slot_rect(int slot_index);
Vector2 get_backpack_slot_pivot(int slot_index);

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

    if (IsKeyReleased(KeyboardKey::KEY_F5)) {
        std::stringstream stream{};

        stream << "\n";
        stream << "backpack: { ";
        for (int slot_index = 0; slot_index < static_cast<int>(game::BACKPACK_SIZE); slot_index++) {
            if (slot_index % 6 == 0) {
                stream << "\n";
            }
            stream << sprite_backpack_items[slot_index] << ", ";
        }
        stream << "}\n";
        stream << "belt: { ";
        for (const auto &slot: sprite_belt_items) {
            stream << slot << ", ";
        }
        stream << "}\n\n";

        std::cout << stream.str() << std::endl;
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
        item_icon->sprite.scale += (.6f - item_icon->sprite.scale) * GetFrameTime() * 16;
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

SlotInfo get_hovered_slot(Vector2 point) {
    for (int slot_index = 0; slot_index < static_cast<int>(game::BACKPACK_SIZE); slot_index++) {
        const Rectangle rect = get_backpack_slot_rect(slot_index);
        if (common::is_point_in_rect(point, rect)) {
            return {slot_index, SlotInfo::Backpack};
        }
    }

    for (int slot_index = 0; slot_index < static_cast<int>(game::BELT_SIZE); slot_index++) {
        const Rectangle rect = get_belt_slot_rect(slot_index);
        if (common::is_point_in_rect(point, rect)) {
            return {slot_index, SlotInfo::Belt};
        }
    }

    return {-1, SlotInfo::None};
}

Rectangle get_slot_rect(SlotInfo slot_info) {
    Rectangle rectangle;
    switch (slot_info.type) {
        case SlotInfo::None:
            throw;
        case SlotInfo::Backpack:
            rectangle = get_backpack_slot_rect(slot_info.index);
            break;
        case SlotInfo::Belt:
            rectangle = get_belt_slot_rect(slot_info.index);
            break;
    }
    return rectangle;
}

Vector2 get_slot_pivot(SlotInfo slot_info) {
    Vector2 pivot;
    switch (slot_info.type) {
        case SlotInfo::None:
            throw;
        case SlotInfo::Backpack:
            pivot = get_backpack_slot_pivot(slot_info.index);
            break;
        case SlotInfo::Belt:
            pivot = get_belt_slot_pivot(slot_info.index);
            break;
    }
    return pivot;
}

Rectangle get_backpack_slot_rect(int slot_index) {
    const Vector2 slot_pivot = get_backpack_slot_pivot(slot_index);
    return {
            slot_pivot.x - SLOT_SIZE.x * 0.5f, slot_pivot.y - SLOT_SIZE.y * 0.5f,
            SLOT_SIZE.x, SLOT_SIZE.y
    };
}

Vector2 get_backpack_slot_pivot(int slot_index) {
    const int col = slot_index % BACKPACK_COLUMN_COUNT;
    const int row = slot_index / BACKPACK_COLUMN_COUNT;
    Vector2 slot_position = {(BACKPACK_SLOTS_MARGIN.x + SLOT_SIZE.x) * static_cast<float>(col),
                             (BACKPACK_SLOTS_MARGIN.y + SLOT_SIZE.y) * static_cast<float>(row)};

    return Vector2Add(slot_position, BACKPACK_SLOTS_ORIGIN);
}

Rectangle get_belt_slot_rect(int slot_index) {
    const Vector2 slot_pivot = get_belt_slot_pivot(slot_index);
    return {
            slot_pivot.x - SLOT_SIZE.x * 0.5f, slot_pivot.y - SLOT_SIZE.y * 0.5f,
            SLOT_SIZE.x, SLOT_SIZE.y
    };
}

Vector2 get_belt_slot_pivot(int slot_index) {
    Vector2 slot_position = {(BELT_SLOTS_MARGIN.x + SLOT_SIZE.x) * static_cast<float>(slot_index),
                             (BELT_SLOTS_MARGIN.y + SLOT_SIZE.y)};

    return Vector2Add(slot_position, BELT_SLOTS_ORIGIN);
}

} // ui
