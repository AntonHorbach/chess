#ifndef PLAYER
#define PLAYER

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "figure.h"

using Color = bool;

#define WHITE true
#define BLACK false

#define ind_FT (size_t)FT

class Player {
    std::vector<Figure> figures;
    Color color;

public:
    Player(Color);

    bool init(SDL_Renderer* renderer);

    void handleEvents(SDL_Event* event);
    void update(size_t ind, size_t x, size_t y);
    void render(SDL_Renderer* renderer);
};

#endif