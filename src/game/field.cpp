#include "field.h"

Field::Field()
    : white_square(nullptr), black_square(nullptr)
{
    for(size_t i = 0; i < SIZE; ++i){
        for(size_t j = 0; j < SIZE; ++j){
            field[i][j] = (i + j)%2 == 0 ? WHITE_SQUARE : BLACK_SQUARE;
        }
    }
}

bool Field::init(SDL_Renderer* renderer) {
    white_square = TextureManager::loadTexture(renderer, "./assets/white_square.png");
    black_square = TextureManager::loadTexture(renderer, "./assets/black_square.png");

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
            SDL_Texture* cur_texture = nullptr;

            switch(field[i][j]) {
            case WHITE_SQUARE:
                cur_texture = white_square;
                break;
            case BLACK_SQUARE:
                cur_texture = black_square;
                break;
            }

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