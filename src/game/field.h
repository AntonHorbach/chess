#ifndef FIELD
#define FIELD

#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "TextureManager.h"
#include "figure.h"

#define BLACK_SQUARE 16
#define WHITE_SQUARE 17

const size_t SIZE = 8;

class Field {
    size_t field[SIZE][SIZE];

    SDL_Texture* white_square;
    SDL_Texture* black_square;

    SDL_Rect square_srcrect;
    SDL_Rect square_dstrect;

public:
    Field();
    ~Field();

    bool init(SDL_Renderer* renderer);
    void handleEvents(SDL_Event* event);
    void update();
    void render(SDL_Renderer* renderer);
};

#endif