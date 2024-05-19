#ifndef INVENTORYTEST_GAME_H
#define INVENTORYTEST_GAME_H

#include "game/GameDatabase.h"
#include "game/GameState.h"
#include "ui/Ui.h"

namespace game {

class Game {
public:
    Game();
    ~Game();

    void init();
    void run();
private:
    GameState _state;
    ui::Ui _ui;
};

} // game

#endif //INVENTORYTEST_GAME_H
