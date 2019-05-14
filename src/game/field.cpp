#include "filed.h"

Field::Field(): white_square(nullptr), black_square(nullptr)
{
    for(size_t i = 0; i < SIZE; ++i){
        for(size_t j = 0; j < SIZE; ++j){
            field[i][j] = (i + j)%2 == 0 ? 0 : 1;
        }
    }
}

bool Field::init(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("./assets/white_square.png");
    white_square = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./assets/black_square.png");
    black_square = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if(white_square == nullptr || black_square == nullptr) {
        std::cout << "Can't load textures: " << SDL_GetError() << std::endl;
        return false;
    }

    square_srcrect.h = 32;
    square_srcrect.w = 32;
    square_srcrect.x = 0;
    square_srcrect.y = 0;

    square_dstrect.h = 480 / SIZE;
    square_dstrect.w = 480 / SIZE;
    square_dstrect.x = 0;
    square_dstrect.y = 0;

    return true;
}

void Field::render(SDL_Renderer* renderer) {
    square_dstrect.y = 0;

    for(size_t i = 0; i < SIZE; ++i) {
        square_dstrect.x = 0;

        for(size_t j = 0; j < SIZE; ++j) {
            SDL_Texture* cur_texture = (field[i][j] == 0 ? white_square : black_square);
            SDL_RenderCopy(renderer, cur_texture, &square_srcrect, &square_dstrect);

            square_dstrect.x += square_dstrect.w;
        }

        square_dstrect.y += square_dstrect.h;
    }
}

void Field::update() {

}

void Field::handleEvents(SDL_Event* event) {

}

Field::~Field() {
    SDL_DestroyTexture(white_square);
    SDL_DestroyTexture(black_square);
}