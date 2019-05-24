#ifndef FIGURE
#define FIGURE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

#include "TextureManager.h"

static std::shared_ptr<SDL_Texture> make_shared_texture(SDL_Texture* texture);

enum class FIGURES_TYPE: size_t {
    LEFT_ROOK = 0, LEFT_HORSE, LEFT_ELEPHANT, QUEEN, KING,
    RIGHT_ELEPHANT, RIGHT_HORSE, RIGHT_ROOK,
    PAWN_1, PAWN_2, PAWN_3, PAWN_4, PAWN_5,
    PAWN_6, PAWN_7, PAWN_8 = 15
};

using FT = FIGURES_TYPE;
using MOVE = std::pair<int, int>;

#define ind_FT (size_t)FT

class Figure {
    SDL_Renderer* renderer = nullptr;
    std::shared_ptr<SDL_Texture> texture = nullptr;
    SDL_Rect srcrect;
    SDL_Rect dstrect;
    
    size_t x;
    size_t y;
    int dx = 0;
    int dy = 0;
    FIGURES_TYPE type;

    std::vector<MOVE> moves;
    std::vector<MOVE> attacks;

    bool dead = false;
    bool dragging = false;

    void onPressed(SDL_Event* pressedEvent);
    void onReleased(SDL_Event* event);
    void onMotion(SDL_Event* event);

public:
    Figure();
    Figure(size_t _x, size_t _y, FIGURES_TYPE value);

    void setMoves(const std::vector<MOVE>& moves);
    void setAttacks(const std::vector<MOVE>& attacks);

    size_t getX() const;
    size_t getY() const;
    size_t getType() const;

    void kill();
    bool isDead() const;
    bool isDragging() const;

    bool init(SDL_Renderer* renderer, std::string path_to_sprite);
    void update(size_t x, size_t y);
    void render();
    void handleEvents(SDL_Event* pressedEvent);
};

#endif