#include "game/Item.h"

namespace game {

ItemId ItemDatabase::register_item(const char *name, const char *desc, const char *icon,
                                   const Vector2 &icon_offset) {
    // ID zero is special, so we increment then assign.
    ItemId id = ++_count;
    _items.push_back(Item{id, name, desc, icon, icon_offset});
    return id;
}

const Item &ItemDatabase::get_item(ItemId id) const {
    return _items.at(id - 1);
}

std::vector<ItemId> ItemDatabase::all_ids() const {
    std::vector<ItemId> ids = {};
    ids.resize(_count);
    for (size_t i = 0; i < _count; ++i) {
        ids[i] = _items[i].id;
    }
    return ids;
}

}
