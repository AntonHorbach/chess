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

enum class STATUS: size_t {
    CHECKMATE = 0, PAT, NONE
};

class Player {
    std::vector<Figure> figures;

    std::stack<size_t> prevs;
    std::stack<size_t> futures;

    Color color;
    STATUS status = STATUS::NONE;

public:
    Player(Color);

    bool init(SDL_Renderer* renderer);
    bool handleEvents(SDL_Event* event);
    void render();
    void update(const Player* another_player);

    bool killIfFind(size_t x, size_t y);
    bool isMove() const;
    bool isAttack() const;
    
    STATUS getStatus() const;
    void setStatus(STATUS status);

    void rollback();
    void reset();

    const Figure* getFigure(size_t x, size_t y) const;
    const std::vector<Figure>& getFigures() const;
    const Figure* getFigure(FT type) const;

    operator std::string();
};

#endif