#ifndef INVENTORYTEST_INVENTORYGUI_H
#define INVENTORYTEST_INVENTORYGUI_H

#include "graphics/Sprite.h"
#include "game/Inventory.h"
#include "ui/ClickableIcon.h"

namespace ui {

class InventoryGui {
public:
    ClickableIcon *sprite_backpack_items[game::BACKPACK_SIZE] = {nullptr};
    ClickableIcon *sprite_belt_items[game::BELT_SIZE] = {nullptr};
    graphics::Sprite sprite_window;

    game::Inventory &inventory;
    bool active;
    int dragging = -1;

    explicit InventoryGui(game::Inventory &inventory);
    ~InventoryGui();

    void draw() const;
    void sync();
    void process_input();
};

} // ui

#endif //INVENTORYTEST_INVENTORYGUI_H
