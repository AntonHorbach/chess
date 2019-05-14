#include <SDL2/SDL_image.h>
#include <iostream>

const uint8_t SIZE = 8;

class Field {
    uint8_t field[SIZE][SIZE];

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