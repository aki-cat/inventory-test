#ifndef INVENTORYTEST_GAME_H
#define INVENTORYTEST_GAME_H

#include "game/GameDatabase.h"
#include "game/GameState.h"

namespace game {

class Game {
public:
    Game();
    ~Game();

    void init();
    void run();
private:
    GameState _state;
};

} // game

#endif //INVENTORYTEST_GAME_H
