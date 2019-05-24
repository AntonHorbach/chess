#include "figure.h"

std::shared_ptr<SDL_Texture> make_shared_texture(SDL_Texture* texture) {
    return {texture, SDL_DestroyTexture};
}

Figure::Figure() {}

Figure::Figure(size_t _x, size_t _y, FIGURES_TYPE _type)
    : x(_x), y(_y), type(_type)
{}

size_t Figure::getX() const {
    return x;
}

size_t Figure::getY() const {
    return y;
}

size_t Figure::getType() const {
    return (size_t)type;
}

void Figure::setAttacks(const std::vector<MOVE>& attacks) {
    for(size_t i = 0; i < attacks.size(); ++i) {
        this->attacks.push_back(attacks[i]);
    }
}

void Figure::setMoves(const std::vector<MOVE>& moves) {
    for(size_t i = 0; i < moves.size(); ++i) {
        this->moves.push_back(moves[i]);
    }
}

void Figure::kill() {
    dead = true;
}

bool Figure::isDead() const {
    return dead;
}

bool Figure::isDragging() const {
    return dragging;
}

bool Figure::init(SDL_Renderer* renderer, std::string path_to_sprite) {
    this->renderer = renderer;
    texture = make_shared_texture(TextureManager::loadTexture(renderer, path_to_sprite.c_str()));

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

void Figure::render() {
    SDL_RenderCopy(renderer, texture.get(), &srcrect, &dstrect);
}

void Figure::onPressed(SDL_Event* event) {
    dx = -(dstrect.x - event->button.x);
    dy = -(dstrect.y - event->button.y);
    dragging = true;
}

void Figure::onReleased(SDL_Event* event) {
    dragging = false;

    const int move_dx = - (x - event->button.x / (480 / 8));
    const int move_dy = - (y - event->button.y / (480 / 8));

    if(std::find(std::begin(attacks), std::end(attacks),
     std::pair<int, int>(move_dx, move_dy)) != std::end(attacks))
    {
        x += move_dx;
        y += move_dy;
    }
    else if(std::find(std::begin(moves), std::end(moves),
     std::pair<int, int>(move_dx, move_dy)) != std::end(moves)) {
         x += move_dx;
         y += move_dy;
     }

    dstrect.x = x * (480 / 8);
    dstrect.y = y * (480 / 8);
}

void Figure::onMotion(SDL_Event* event) {
    if(dragging) {
        dstrect.x += dx;
        dstrect.y += dy;
    }

    dx = event->button.x - dstrect.x;
    dy = event->button.y - dstrect.y;
}

void Figure::handleEvents(SDL_Event* event) {
    switch(event->type) {
        case SDL_MOUSEBUTTONDOWN:
            onPressed(event);
            break;
        case SDL_MOUSEBUTTONUP:
            onReleased(event);
            break;
        case SDL_MOUSEMOTION:
            onMotion(event);
            break;
        default:
            break;
    }
}