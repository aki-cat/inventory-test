#ifndef INVENTORYTEST_SLOTINFO_H
#define INVENTORYTEST_SLOTINFO_H

#include <raylib.h>

static constexpr Vector2 SLOT_SIZE{56.f, 56.f};
static constexpr Vector2 BACKPACK_SLOTS_ORIGIN{522., 216.};
static constexpr Vector2 BACKPACK_SLOTS_MARGIN = {4.f, 2.f};
static constexpr Vector2 BELT_SLOTS_ORIGIN{390., 512.};
static constexpr Vector2 BELT_SLOTS_MARGIN = {4.f, 2.f};
static constexpr int BACKPACK_COLUMN_COUNT = 6;

struct SlotInfo {
    int index = -1;
    enum SlotType {
        None,
        Backpack,
        Belt,
    } type = None;

    bool valid() const { return index != -1 && type != None; }
};

SlotInfo get_hovered_slot(Vector2 point);
Rectangle get_slot_rect(SlotInfo slot_info);
Vector2 get_slot_pivot(SlotInfo slot_info);

Rectangle get_belt_slot_rect(int slot_index);
Vector2 get_belt_slot_pivot(int slot_index);

Rectangle get_backpack_slot_rect(int slot_index);
Vector2 get_backpack_slot_pivot(int slot_index);

#endif //INVENTORYTEST_SLOTINFO_H
