#include "figure.h"

Figure::Figure() {}

Figure::Figure(size_t _x, size_t _y, FIGURES_TYPE _type)
    : x(_x), y(_y), type(_type), dead(false), texture(nullptr)
{}

const std::vector<int>& Figure::getMoves() {
    return moves;
}

const std::vector<int>& Figure::getAttacks() {
    return attacks;
}

size_t Figure::getX() const {
    return x;
}

size_t Figure::getY() const {
    return y;
}

bool Figure::isDead() {
    return dead;
}

bool Figure::init(SDL_Renderer* renderer, std::string path_to_sprite) {
    texture = TextureManager::loadTexture(renderer, path_to_sprite.c_str());

    if(texture == nullptr) {
        std::cout << "texture for figure can't be load: "
                  << SDL_GetError() << '\n';

        return false;
    }

    srcrect.x = srcrect.y = 0;
    srcrect.h = srcrect.w = 80;
    dstrect.h = dstrect.w = 64;
    dstrect.x = x * (480 / 8);
    dstrect.y = y * (480 / 8);
    
    return true;
}

void Figure::update(size_t x, size_t y) {
    this->x = x;
    this->y = y;
}

void Figure::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
}

Figure::~Figure() {
    SDL_DestroyTexture(texture);
}