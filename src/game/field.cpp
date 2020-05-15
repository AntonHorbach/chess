#include "field.h"

Field::Field()
{
    resetSquares();
}

void Field::changeSquares(size_t x, size_t y,
                          const std::vector<std::pair<int, int>>& squares,
                          SQUARE square_type)
{
    if(square_type != SQUARE::MOVE_SQUARE && square_type != SQUARE::ATTACK_SQUARE) return;

    for(const auto& square : squares) {
        if(x + square.first < 0 || x + square.first >= SIZE
            || y + square.second < 0 || y + square.second >=SIZE)
        {
            continue;
        }

        field[y + square.second][x + square.first] = (size_t)square_type;
    }
}

void Field::resetSquares() {
    for(size_t i = 0; i < SIZE; ++i){
        for(size_t j = 0; j < SIZE; ++j){
            field[i][j] = (i + j)%2 == 0 ? (size_t)SQUARE::WHITE_SQUARE : (size_t)SQUARE::BLACK_SQUARE;
        }
    }
}

bool Field::init(SDL_Renderer* renderer) {
    white_square = TextureManager::loadTexture(renderer, "./assets/textures/white_square.png");
    black_square = TextureManager::loadTexture(renderer, "./assets/textures/black_square.png");
    move_square = TextureManager::loadTexture(renderer, "./assets/textures/move_square.png");
    attack_square = TextureManager::loadTexture(renderer, "./assets/textures/attack_square.png");

    if(white_square == nullptr || black_square == nullptr || move_square == nullptr
        || attack_square == nullptr)
    {
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
            std::shared_ptr<SDL_Texture> cur_texture = nullptr;

            switch((SQUARE)field[i][j]) {
            case SQUARE::WHITE_SQUARE:
                cur_texture = white_square;
                break;
            case SQUARE::BLACK_SQUARE:
                cur_texture = black_square;
                break;
            case SQUARE::MOVE_SQUARE:
                cur_texture = move_square;
                break;
            case SQUARE::ATTACK_SQUARE:
                cur_texture = attack_square;
                break;
            }

            SDL_RenderCopy(renderer, cur_texture.get(), &square_srcrect, &square_dstrect);

            square_dstrect.x += square_dstrect.w;
        }

        square_dstrect.y += square_dstrect.h;
    }
}

void Field::update() {

}

void Field::handleEvents(SDL_Event* event) {

}

Field::~Field() {}