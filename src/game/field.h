#ifndef FIELD
#define FIELD

#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <memory>

#include "TextureManager.h"
#include "figure.h"

#define BLACK_SQUARE 16
#define WHITE_SQUARE 17
#define MOVE_SQUARE 18
#define ATTACK_SQUARE 19

const size_t SIZE = 8;

class Field {
    int field[SIZE][SIZE];

    std::shared_ptr<SDL_Texture> white_square;
    std::shared_ptr<SDL_Texture> black_square;
    std::shared_ptr<SDL_Texture> move_square;
    std::shared_ptr<SDL_Texture> attack_square;

    SDL_Rect square_srcrect;
    SDL_Rect square_dstrect;

public:
    Field();
    ~Field();

    void changeSquares(size_t x, size_t y,
                       const std::vector<std::pair<int, int>>& squares,
                       int flag = MOVE_SQUARE);
    void resetSquares();

    bool init(SDL_Renderer* renderer);
    void handleEvents(SDL_Event* event);
    void update();
    void render(SDL_Renderer* renderer);
};

#endif