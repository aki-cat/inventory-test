#ifndef INVENTORYTEST_INVENTORYGUI_H
#define INVENTORYTEST_INVENTORYGUI_H

#include "graphics/Sprite.h"
#include "game/Inventory.h"
#include "ui/ClickableIcon.h"
#include "ui/SlotInfo.h"

namespace ui {

class InventoryGui {
public:
    ClickableIcon *sprite_backpack_items[game::BACKPACK_SIZE] = {nullptr};
    ClickableIcon *sprite_belt_items[game::BELT_SIZE] = {nullptr};
    graphics::Sprite sprite_window;

    game::Inventory &inventory;
    SlotInfo dragging = {};
    SlotInfo selected = {};
    bool active;

    explicit InventoryGui(game::Inventory &inventory);
    ~InventoryGui();

    void draw() const;
    void draw_interactions() const;
    void draw_description() const;
    void sync();
    void setup_slot(game::ItemId item_id, SlotInfo slot_info);
    void process_input();
    bool process_slot_input(SlotInfo slot_info);
    void swap_slots(SlotInfo, SlotInfo);
};

} // ui

#endif //INVENTORYTEST_INVENTORYGUI_H
