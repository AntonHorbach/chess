#include "player.h"

Player::Player(Color _color)
    : color(_color), figures(16)
{
    int step = (color == BLACK ? +1 : -1);
    size_t begin_x = (color == BLACK ? 0 : 7);
    size_t x = begin_x - step;
    size_t y = (color == BLACK ? 7 : 0);

    figures[ind_FT::LEFT_ROOK] = Figure(x += step, y, FT::LEFT_ROOK);
    figures[ind_FT::LEFT_HORSE] = Figure(x += step, y, FT::LEFT_HORSE);
    figures[ind_FT::LEFT_ELEPHANT] = Figure(x += step, y, FT::LEFT_ELEPHANT);
    figures[ind_FT::QUEEN] = Figure(x += step, y, FT::QUEEN);
    figures[ind_FT::KING] = Figure(x += step, y, FT::KING);
    figures[ind_FT::RIGHT_ELEPHANT] = Figure(x += step, y, FT::RIGHT_ELEPHANT);
    figures[ind_FT::RIGHT_HORSE] = Figure(x += step, y, FT::RIGHT_HORSE);
    figures[ind_FT::RIGHT_ROOK] = Figure(x += step, y, FT::RIGHT_ROOK);

    x = begin_x;
    y -= step;

    for(size_t i = ind_FT::PAWN_1; i <= ind_FT::PAWN_8; ++i, x += step) {
        figures[i] = Figure(x, y, (FT)i);
    }
}

bool Player::init(SDL_Renderer* renderer) {
    std::string folder = (color == WHITE ? "./assets/white/" : "./assets/black/");

    size_t step = 2;
    for(size_t i = 0; i < figures.size(); ++i) {
        if(i < 5) {
            if(!figures[i].init(renderer, folder + std::to_string(i + 1) + ".png")) {
                return false;
            }
        }
        else if(i < 8) {
            if(!figures[i].init(renderer, folder + std::to_string(i - step) + ".png")) {
                return false;
            }
            step += 2;
        }
        else {
            if(!figures[i].init(renderer, folder + "6.png")) {
                return false;
            }
        }
    }

    return true;
}

void Player::handleEvents(SDL_Event* event) {

}

void Player::render(SDL_Renderer* renderer) {
    for(Figure& figure : figures) {
        if(!figure.isDead())
            figure.render(renderer);
    }
}

void Player::update(size_t ind, size_t x, size_t y) {
    figures[ind].update(x, y);
}