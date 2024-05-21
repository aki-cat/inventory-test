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
                        "Could hold\nsomething.",
                        "bucket.png",
                        {.55f, .55f});
    items.register_item("Light Bulb",
                        "Small and\nthin. Hopefully\nit still works.",
                        "bulb.png",
                        {.5f, .5f});
    items.register_item("Lighter",
                        "Great for\narson.",
                        "lighter.png",
                        {.5f, .5f});
    items.register_item("Magnifying\nGlass",
                        "Makes you\nfeel like an\nold timey\ndetective.",
                        "mglass.png",
                        {.5f, .5f});
    items.register_item("Tape",
                        "A little\nthinner than\nyou´d like,\nbut it'll work.",
                        "tape.png",
                        {.55f, .5f});
    items.register_item("Wrench",
                        "Could be\nused as a\nweapon. Or\nto disassemble\nstuff.",
                        "wrench.png",
                        {.5f, .6f});
}

static const GameDatabase GAME_DATABASE{};

}
#endif //INVENTORY_TEST_GAMEDATABASE_H
