#include "game/game.h"

int main(int, char**) {
    Game game;

    if(!game.init("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, false)) {
        return 1;
    }

    return game.exec();
}
