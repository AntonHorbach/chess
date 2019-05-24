#include "player.h"

Player::Player(Color _color)
    : color(_color), figures(16)
{
    int step = (color == BLACK ? -1 : 1);
    size_t begin_x = (color == BLACK ? 7 : 0);
    size_t x = begin_x - step;
    size_t y = (color == BLACK ? 0 : 7);

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

    for(size_t i = 0; i < figures.size(); ++i) {
        if(figures[i].getType() == ind_FT::LEFT_ROOK
         || figures[i].getType() == ind_FT::RIGHT_ROOK)
        {
            figures[i].setAttacks(tools::generateLongStraightMoves());
        }
        else if(figures[i].getType() == ind_FT::LEFT_HORSE
            || figures[i].getType() == ind_FT::RIGHT_HORSE)
        {
            figures[i].setAttacks(tools::generateMoves(
                {{2, 1}, {1, 2}}
            ));
        }
        else if(figures[i].getType() == ind_FT::LEFT_ELEPHANT
            || figures[i].getType() == ind_FT::RIGHT_ELEPHANT)
        {
            figures[i].setAttacks(tools::generateLongVerticalMoves());
        }
        else if(figures[i].getType() == ind_FT::QUEEN) {
            figures[i].setAttacks(tools::generateLongStraightMoves());
            figures[i].setAttacks(tools::generateLongVerticalMoves());
        }
        else if(figures[i].getType() == ind_FT::KING) {
            figures[i].setAttacks(tools::generateMoves(
                {{1, 1}, {0, 1}, {1, 0}}
            ));
        }
        else {
            if(color == WHITE) {
                figures[i].setMoves({{0, -1}});
                figures[i].setAttacks({{1, -1}, {-1, -1}});
            }
            else {
                figures[i].setMoves({{0, 1}});
                figures[i].setAttacks({{1, 1}, {-1, 1}});
            }
        }

    }

    return true;
}

bool Player::handleEvents(SDL_Event* event) {
    size_t x = event->button.x / (480 / 8);
    size_t y = event->button.y / (480 / 8);

    for(size_t i = 0; i < figures.size(); ++i) {
        if((event->type == SDL_MOUSEBUTTONUP || event->type == SDL_MOUSEMOTION)
             && figures[i].isDragging())
        {
                figures[i].handleEvents(event);
                return true;
        }
        else if(event->type == SDL_MOUSEBUTTONDOWN && figures[i].getX() == x &&
         figures[i].getY() == y)
        {
            figures[i].handleEvents(event);
            return true;
        }
    }

    return false;
}

void Player::render() {
    for(Figure& figure : figures) {
        if(!figure.isDead())
            figure.render();
    }
}

void Player::killIfFind(size_t x, size_t y) {
    for(size_t i = 0; i < figures.size(); ++i) {
        if(figures[i].getX() == x && figures[i].getY() == y) {
            figures[i].kill();
            return;
        }
    }
}