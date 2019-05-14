#include "game/game.h"

int main(int, char**) {
    Game game;

    if(!game.init("Chess", 300, 150, 640, 480, false)) {
        return 1;
    }

    return game.exec();
}
