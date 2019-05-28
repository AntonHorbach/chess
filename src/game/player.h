#ifndef PLAYER
#define PLAYER

#include <SDL2/SDL.h>
#include <vector>
#include <stack>
#include <string>

#include "figure.h"
#include "../tools/tools.h"

using Color = bool;

#define WHITE true
#define BLACK false

class Player {
    std::vector<Figure> figures;
    std::stack<size_t> prevs;
    std::stack<size_t> futures;
    Color color;

public:
    Player(Color);

    bool init(SDL_Renderer* renderer);
    bool handleEvents(SDL_Event* event);
    bool killIfFind(size_t x, size_t y);
    bool isMove();
    bool isAttack();
    void rollback();
    const Figure* getFigure(size_t x, size_t y);
    void render();
    void update(const Player* another_player);
};

#endif