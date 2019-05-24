#ifndef PLAYER
#define PLAYER

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "figure.h"
#include "../tools/tools.h"

using Color = bool;

#define WHITE true
#define BLACK false

class Player {
    std::vector<Figure> figures;
    Color color;

public:
    Player(Color);

    bool init(SDL_Renderer* renderer);

    bool handleEvents(SDL_Event* event);
    void killIfFind(size_t x, size_t y);
    void render();
};

#endif