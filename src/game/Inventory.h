#ifndef INVENTORY_TEST_INVENTORY_H
#define INVENTORY_TEST_INVENTORY_H

#include "game/Item.h"
#include "graphics/Sprite.h"

#include <array>

namespace game {

constexpr size_t BACKPACK_SIZE = 30;
constexpr size_t BELT_SIZE = 10;

struct Inventory {
    game::ItemId backpack[BACKPACK_SIZE] = {NoItem};
    game::ItemId belt[BELT_SIZE] = {NoItem};
};

}
#endif //INVENTORY_TEST_INVENTORY_H
