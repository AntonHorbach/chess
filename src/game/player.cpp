#include "player.h"

Player::Player(Color _color)
    : color(_color), figures(16)
{
    int step = (color == BLACK ? -1 : 1);
    size_t x = 0;
    size_t y = (color == BLACK ? 0 : 7);

    for(size_t i = ind_FT::LEFT_ROOK; i < ind_FT::PAWN_1; ++i, x += 1) {
        figures[i] = Figure(x, y, (FT)i);
    }

    x = 0;
    y -= step;

    for(size_t i = ind_FT::PAWN_1; i <= ind_FT::PAWN_8; ++i, x += 1) {
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

    std::vector<MOVE> actions;
    for(size_t i = 0; i < figures.size(); ++i) {
        if(figures[i].getType() == FT::LEFT_ROOK
         || figures[i].getType() == FT::RIGHT_ROOK)
        {
            actions = tools::generateLongStraightMoves();
            figures[i].setAttacks(actions);
            figures[i].setMoves(actions);
        }
        else if(figures[i].getType() == FT::LEFT_HORSE
            || figures[i].getType() == FT::RIGHT_HORSE)
        {
            actions = tools::generateMoves(
                {{2, 1}, {1, 2}}
            );

            figures[i].setAttacks(actions);
            figures[i].setMoves(actions);
        }
        else if(figures[i].getType() == FT::LEFT_ELEPHANT
            || figures[i].getType() == FT::RIGHT_ELEPHANT)
        {
            actions = tools::generateLongDiagonalMoves();
            figures[i].setAttacks(actions);
            figures[i].setMoves(actions);
        }
        else if(figures[i].getType() == FT::QUEEN) {
            actions = tools::generateLongStraightMoves();
            figures[i].setAttacks(actions);
            figures[i].setMoves(actions);

            actions = tools::generateLongDiagonalMoves();
            figures[i].setAttacks(actions);
            figures[i].setMoves(actions);
        }
        else if(figures[i].getType() == FT::KING) {
            actions = tools::generateMoves(
                {{1, 1}, {0, 1}, {1, 0}}
            );
            
            figures[i].setAttacks(actions);
            figures[i].setMoves(actions);
        }
        else {
            if(color == WHITE) {
                figures[i].setMoves({{0, -1}, {0, -2}});
                figures[i].setAttacks({{1, -1}, {-1, -1}});
            }
            else {
                figures[i].setMoves({{0, 1}, {0, 2}});
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
            if(figures[i].handleEvents(event) && event->type == SDL_MOUSEBUTTONUP)
            {
                prevs.push(i);
                futures = std::stack<size_t>();

                return true;
            }
            else if(event->type == SDL_MOUSEMOTION) {
                return true;
            }
            else {
                return false;
            }
        }
        else if(event->type == SDL_MOUSEBUTTONDOWN && figures[i].getX() == x &&
         figures[i].getY() == y && !figures[i].isDead())
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

void Player::update(const Player* enemy) {
    std::vector<MOVE> available_moves;
    std::vector<MOVE> available_attacks;
    std::vector<POS> poses;

    std::vector<POS> enemy_poses;
    for(size_t j = 0; j < enemy->figures.size(); ++j) {
        if(!enemy->figures[j].isDead())
            enemy_poses.push_back({enemy->figures[j].getX(), enemy->figures[j].getY()});
    }

    for(size_t j = 0; j < figures.size(); ++j) {
        if(!figures[j].isDead())
            poses.push_back({figures[j].getX(), figures[j].getY()});
    }
    poses += enemy_poses;
    
    for(size_t i = 0; i < figures.size(); ++i) {
        if(figures[i].isDead()) continue;

        available_moves.clear();

        available_moves = tools::clipping(
            figures[i].getX(), figures[i].getY(),
            figures[i].getMoves(), poses
        );

        available_attacks = tools::findMatches(
            figures[i].getX(), figures[i].getY(),
            tools::clipping(
                figures[i].getX(), figures[i].getY(),
                figures[i].getAttacks(), poses, false
            ), enemy_poses
        );

        if(i == ind_FT::KING) {
            available_moves += MOVE{figures[i].getX(), figures[i].getY()};
            available_attacks += MOVE{figures[i].getX(), figures[i].getY()};

            std::vector<POS> coveredFigures;

            for(const Figure& figure : enemy->getFigures()) {
                if(figure.isDead()) continue;

                std::vector<MOVE> delta;

                if(figure.getType() >= FT::PAWN_1) {
                    delta = figure.getAttacks() + MOVE{figure.getX(), figure.getY()};
                }
                else {
                    delta = figure.getAvailableMoves() + figure.getAvailableAttacks() +
                            MOVE{figure.getX(), figure.getY()};
                }

                available_moves -= delta;
                available_attacks -= delta;

                coveredFigures += tools::findMatches(
                    figure.getX(), figure.getY(),
                    tools::clipping(
                        figure.getX(), figure.getY(),
                        figure.getAttacks(), enemy_poses, false
                    ), enemy_poses
                ) + MOVE{figure.getX(), figure.getY()};
            }

            available_attacks -= coveredFigures;
            available_moves -= MOVE{figures[i].getX(), figures[i].getY()};
            available_attacks -= MOVE{figures[i].getX(), figures[i].getY()};
        }
        if(i >= ind_FT::PAWN_1) {
            if(color == WHITE) {
                if(figures[i].getY() != 6) {
                    available_moves.erase(std::remove(std::begin(available_moves), 
                                                    std::end(available_moves),
                                                    MOVE{0, -2}
                    ), std::end(available_moves));
                }
            }
            else {
                if(figures[i].getY() != 1) {
                    available_moves.erase(std::remove(std::begin(available_moves), 
                                                    std::end(available_moves),
                                                    MOVE{0, 2}
                    ), std::end(available_moves));
                }
            }
        }

        figures[i].updateAvailableAttacks(std::move(available_attacks));
        figures[i].updateAvailableMoves(std::move(available_moves));
    }
}

bool Player::killIfFind(size_t x, size_t y) {
    x = x / (480 / 8);
    y = y / (480 / 8);

    for(size_t i = 0; i < figures.size(); ++i) {
        if(figures[i].getX() == x && figures[i].getY() == y) {
            prevs.push(i);
            futures = std::stack<size_t>();
            figures[i].kill();
            return true;
        }
    }

    return false;
}

void Player::reset() {
    int step = (color == BLACK ? -1 : 1);
    size_t begin_x = (color == BLACK ? 7 : 0);
    size_t x = begin_x - step;
    size_t y = (color == BLACK ? 0 : 7);

    for(size_t i = ind_FT::LEFT_ROOK; i <= ind_FT::PAWN_8; ++i, x += step) {
        if(i == ind_FT::PAWN_1) {
            x = begin_x;
            y -= step;
        }

        figures[i].reset(x, y);
    }
}

void Player::rollback() {
    if(prevs.empty()) return;

    figures[prevs.top()].rollback();
    futures.push(prevs.top());
    prevs.pop();
}

bool Player::isMove() const {
    return figures[prevs.top()].isMove();
}

bool Player::isAttack() const {
    return figures[prevs.top()].isAttack();
}

STATUS Player::getStatus() const {
    return status;
}

void Player::setStatus(STATUS status) {
    this->status = status;
}

const Figure* Player::getFigure(size_t x, size_t y) const {
    x = x / (480 / 8);
    y = y / (480 / 8);

    for(const Figure& figure : figures) {
        if(figure.getX() == x && figure.getY() == y)
            return &figure;
    }

    return nullptr;
}

const std::vector<Figure>& Player::getFigures() const {
    return figures;
}

const Figure* Player::getFigure(FT type) const {
    for(const Figure& figure : figures) {
        if(figure.getType() == type)
            return &figure;
    }

    return nullptr;
}

Player::operator std::string() {
    return color == WHITE ? "White player" : "Black player";
}