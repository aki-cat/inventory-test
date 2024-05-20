#ifndef INVENTORY_TEST_INVENTORY_H
#define INVENTORY_TEST_INVENTORY_H

#include "game/Item.h"
#include "graphics/Sprite.h"

#include <array>

namespace game {

constexpr size_t BACKPACK_SIZE = 30;
constexpr size_t BELT_SIZE = 10;

struct Inventory {
    std::array<game::ItemId, BACKPACK_SIZE> backpack = {NoItem};
    std::array<game::ItemId, BELT_SIZE> belt = {NoItem};
};

}
#endif //INVENTORY_TEST_INVENTORY_H
