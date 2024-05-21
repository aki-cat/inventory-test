#include "ui/SlotInfo.h"

#include "game/Inventory.h"
#include "common/Math.h"

#include <raylib.h>
#include <raymath.h>

// Helper functions

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
