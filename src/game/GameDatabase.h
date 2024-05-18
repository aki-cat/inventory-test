#ifndef INVENTORY_TEST_GAMEDATABASE_H
#define INVENTORY_TEST_GAMEDATABASE_H

#include "game/Item.h"

namespace game {

struct GameDatabase {
    GameDatabase();

    ItemDatabase items;
};

inline GameDatabase::GameDatabase() : items() {
    items.register_item("Bucket",
                        "Could hold something.",
                        "bucket.png");
    items.register_item("Light Bulb",
                        "Small and thin. Hopefully it still works.",
                        "bulb.png");
    items.register_item("Lighter",
                        "Great for arson.",
                        "lighter.png");
    items.register_item("Magnifying Glass",
                        "Makes you feel like an old timey detective.",
                        "mglass.png");
    items.register_item("Tape",
                        "A little thinner than you´d like, but it'll work.",
                        "tape.png");
    items.register_item("Wrench",
                        "Could be used as a weapon. Or to disassemble stuff.",
                        "wrench.png");
}

}
#endif //INVENTORY_TEST_GAMEDATABASE_H
