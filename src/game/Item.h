#ifndef INVENTORY_TEST_ITEM_H
#define INVENTORY_TEST_ITEM_H

#include <cstdint>
#include <string>
#include <vector>

namespace game {

using ItemId = size_t;

constexpr ItemId NoItem = 0;

struct Item {
public:
    const size_t id;
    const std::string name;
    const std::string desc;
    const std::string icon;
};

class ItemDatabase {
public:
    ItemId register_item(const char *name, const char *desc, const char *icon);

    Item get_item(ItemId id) const;
    std::vector<ItemId> all_ids() const;
private:
    size_t _count = 0;
    std::vector<Item> _items = {};
};

}

#endif //INVENTORY_TEST_ITEM_H
