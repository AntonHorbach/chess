#ifndef FIGURE
#define FIGURE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>

#include "TextureManager.h"

enum class FIGURES_TYPE: size_t {
    LEFT_ROOK = 0, LEFT_HORSE, LEFT_ELEPHANT, QUEEN, KING,
    RIGHT_ELEPHANT, RIGHT_HORSE, RIGHT_ROOK,
    PAWN_1, PAWN_2, PAWN_3, PAWN_4, PAWN_5,
    PAWN_6, PAWN_7, PAWN_8 = 15
};

using FT = FIGURES_TYPE;
using MOVE = std::pair<int, int>;
using POS = std::pair<int, int>;

#define ind_FT (size_t)FT

class Figure {
    SDL_Renderer* renderer = nullptr;
    std::shared_ptr<SDL_Texture> texture = nullptr;
    SDL_Rect srcrect;
    SDL_Rect dstrect;

    std::stack<MOVE> prevs;
    std::stack<MOVE> futures;
    
    size_t x;
    size_t y;
    int dx = 0;
    int dy = 0;
    FIGURES_TYPE type;

    std::vector<MOVE> moves;
    std::vector<MOVE> attacks;
    std::vector<MOVE> availableMoves;
    std::vector<MOVE> availableAttacks;

    bool dead = false;
    bool dragging = false;

    void onPressed(SDL_Event* pressedEvent);
    bool onReleased(SDL_Event* event);
    void onMotion(SDL_Event* event);

    void setX_Y(size_t x, size_t y);

public:
    Figure();
    Figure(size_t _x, size_t _y, FIGURES_TYPE type);

    void setMoves(const std::vector<MOVE>& moves);
    void setAttacks(const std::vector<MOVE>& attacks);

    size_t getX() const;
    size_t getY() const;
    FT getType() const;
    const std::vector<MOVE>& getMoves() const;
    const std::vector<MOVE>& getAttacks() const;
    const std::vector<MOVE>& getAvailableMoves() const;
    const std::vector<MOVE>& getAvailableAttacks() const;

    void kill();
    void rollback();
    void reset(size_t x, size_t y);
    bool isDead() const;
    bool isDragging() const;
    bool isMove() const;
    bool isAttack() const;

    bool init(SDL_Renderer* renderer, const std::string& path_to_sprite);
    void updateAvailableMoves(std::vector<MOVE>&& moves);
    void updateAvailableAttacks(std::vector<MOVE>&& attacks);
    void render();
    bool handleEvents(SDL_Event* event);
};

#endif