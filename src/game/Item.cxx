#include "game/Item.h"

namespace game {

ItemId ItemDatabase::register_item(const char *name, const char *desc, const char *icon) {
    ItemId id = _count;
    _items.push_back(Item{id, name, desc, icon});
    _count++;
    return id;
}

Item ItemDatabase::get_item(ItemId id) {
    return _items.at(id);
}

}
