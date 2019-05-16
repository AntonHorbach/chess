#ifndef FIGURE
#define FIGURE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>

#include "TextureManager.h"

enum class FIGURES_TYPE: size_t {
    LEFT_ROOK = 0, LEFT_HORSE, LEFT_ELEPHANT, QUEEN, KING,
    RIGHT_ELEPHANT, RIGHT_HORSE, RIGHT_ROOK,
    PAWN_1, PAWN_2, PAWN_3, PAWN_4, PAWN_5,
    PAWN_6, PAWN_7, PAWN_8 = 15
};

using FT = FIGURES_TYPE;

class Figure {
    SDL_Texture* texture;
    SDL_Rect srcrect;
    SDL_Rect dstrect;

    size_t x;
    size_t y;
    FIGURES_TYPE type;

    std::vector<int> moves;
    std::vector<int> attacks;

    bool dead;

public:
    Figure();
    Figure(size_t _x, size_t _y, FIGURES_TYPE value);
    ~Figure();
    
    const std::vector<int>& getMoves();
    const std::vector<int>& getAttacks();

    size_t getX() const;
    size_t getY() const;

    bool isDead();

    bool init(SDL_Renderer* renderer, std::string path_to_sprite);
    void update(size_t x, size_t y);
    void render(SDL_Renderer* renderer);
};

#endif