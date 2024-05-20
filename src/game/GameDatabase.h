#ifndef INVENTORY_TEST_GAMEDATABASE_H
#define INVENTORY_TEST_GAMEDATABASE_H

#include "game/Item.h"

namespace game {

struct GameDatabase {
    GameDatabase();

    // Here you'd add more readonly stuff regarding the game
    ItemDatabase items;
};

inline GameDatabase::GameDatabase() : items() {
    items.register_item("Bucket",
                        "Could hold something.",
                        "bucket.png",
                        {.55f, .55f});
    items.register_item("Light Bulb",
                        "Small and thin. Hopefully it still works.",
                        "bulb.png",
                        {.5f, .5f});
    items.register_item("Lighter",
                        "Great for arson.",
                        "lighter.png",
                        {.5f, .5f});
    items.register_item("Magnifying Glass",
                        "Makes you feel like an old timey detective.",
                        "mglass.png",
                        {.5f, .5f});
    items.register_item("Tape",
                        "A little thinner than you´d like, but it'll work.",
                        "tape.png",
                        {.55f, .5f});
    items.register_item("Wrench",
                        "Could be used as a weapon. Or to disassemble stuff.",
                        "wrench.png",
                        {.5f, .6f});
}

static const GameDatabase GAME_DATABASE{};

}
#endif //INVENTORY_TEST_GAMEDATABASE_H
