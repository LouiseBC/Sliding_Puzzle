#include "Gamestate_Menu.h"

int main() {
    Game game;
    if (game.init(new MenuState()))
        game.loop();
    return 0;
}