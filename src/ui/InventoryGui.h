#ifndef INVENTORYTEST_INVENTORYGUI_H
#define INVENTORYTEST_INVENTORYGUI_H

#include "graphics/Sprite.h"
#include "game/Inventory.h"
#include "ui/ClickableIcon.h"

namespace ui {

struct SlotInfo {
    int index = -1;
    enum SlotType {
        None,
        Backpack,
        Belt,
    } type = None;

    bool valid() const { return index != -1 && type != None; }
};

class InventoryGui {
public:
    ClickableIcon *sprite_backpack_items[game::BACKPACK_SIZE] = {nullptr};
    ClickableIcon *sprite_belt_items[game::BELT_SIZE] = {nullptr};
    graphics::Sprite sprite_window;

    game::Inventory &inventory;
    SlotInfo dragging = {};
    bool active;

    explicit InventoryGui(game::Inventory &inventory);
    ~InventoryGui();

    void draw() const;
    void sync();
    void process_input();
    bool process_slot_input(SlotInfo slot_info);
    void swap_slots(SlotInfo, SlotInfo);
    void setup_slot(game::ItemId item_id, SlotInfo slot_info);
};

} // ui

#endif //INVENTORYTEST_INVENTORYGUI_H
