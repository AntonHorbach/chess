#include "game.h"

Game::Game(): window(nullptr), renderer(nullptr),
    white_player(WHITE), black_player(BLACK)
{}

bool Game::init(const char* title, int x, int y, int width, int height, bool fullscreen) {
    Uint32 flags = fullscreen ? (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN) : SDL_WINDOW_SHOWN;

    if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cout << "Bad sdl init: " << SDL_GetError() << '\n';
        return false;
    }

    window = SDL_CreateWindow(title, x, y, width, height, flags);
    if(window == nullptr) {
        std::cout << "The window has not been created: " << SDL_GetError() << '\n';
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if(renderer == nullptr) {
        std::cout << "Renderer has not been created: " << SDL_GetError() << '\n';
        return false;
    }

    if(!field.init(renderer) || !white_player.init(renderer)
     || !black_player.init(renderer))
    {
        return false;
    }

    isRunning = true;
    SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255);

    return true;
}

bool Game::checkmate(Player* current_player, Player* another_player) {
    const Figure* enemy_king = another_player->getFigure(FT::KING);

    if(enemy_king->isDead()) return true;

    if(enemy_king->getAvailableMoves().size() + enemy_king->getAvailableAttacks().size() != 0) {
        return false;
    }

    std::vector<std::reference_wrapper<const Figure>> direct_threats;

    for(const auto& figure : current_player->getFigures()) {
        std::vector<MOVE> actions = figure.getAvailableAttacks() + figure.getAvailableMoves();
        for(const auto& [dx, dy] : actions) {
            if(figure.getX() + dx == enemy_king->getX() && figure.getY() + dy == enemy_king->getY())
            {
                direct_threats.push_back(std::reference_wrapper<const Figure>(figure));
                break;
            }
        }
    }

    if(direct_threats.empty()) return false;
    else if(direct_threats.size() > 1) return true;

    for(const Figure& threat : direct_threats) {
        bool flag = false;
        for(const Figure& figure : another_player->getFigures()) {
            if(figure.isDead()) continue;

            std::vector<POS> attacks = figure.getAvailableAttacks() +
                                        POS{figure.getX(), figure.getY()};

            if(std::find(std::begin(attacks), std::end(attacks), POS{threat.getX(), threat.getY()})
                != std::end(attacks))
            {
                return false;
            }
            
            if(threat.getType() == FT::LEFT_HORSE || threat.getType() == FT::RIGHT_HORSE)
                continue;

            if(figure.getType() != FT::PAWN_7) continue;
            if(tools::findMatches(threat.getX(), threat.getY(), enemy_king->getX(), enemy_king->getY(),
                                figure.getAvailableMoves() + POS{figure.getX(), figure.getY()})
                .size() != 0)
            {
                return false;
            }
        }
    }

    return true;
}

bool Game::pat(Player* current_player) {
    for(const auto& figure : current_player->getFigures()) {
        if(!figure.isDead() && figure.getType() != FT::KING) {
            return false;
        }
    }

    if(current_player->getFigure(FT::KING)->getAvailableMoves().empty() &&
        current_player->getFigure(FT::KING)->getAvailableAttacks().empty())
    {
        return true;
    }

    return false;
}

void Game::handleEvents() {
    static int halfMove = 0;
    Player* current_player = (halfMove % 2 == 0) ? &white_player : &black_player;
    Player* another_player = (current_player == &white_player) ? &black_player : &white_player;

    if(pat(current_player)) {
        std::string message(*current_player);
        std::cout << "It's pat for " + message << '\n';

        isRunning = false;
        return;
    }

    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN: {
            if(current_player->handleEvents(&event)){
                current_player->update(another_player);

                field.changeSquares(
                    event.button.x / (480 / 8), event.button.y / (480 / 8),
                    current_player->getFigure(event.button.x, event.button.y)->getAvailableMoves()
                );

                field.changeSquares(
                    event.button.x / (480 / 8), event.button.y / (480 / 8),
                    current_player->getFigure(event.button.x, event.button.y)->getAvailableAttacks(),
                    SQUARE::ATTACK_SQUARE
                );
            }
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            if(current_player->handleEvents(&event)) {
                if(current_player->isAttack() &&
                    another_player->killIfFind(event.button.x, event.button.y)) {
                    ++halfMove;
                }
                else if(!current_player->isMove()) {
                    current_player->rollback();
                }
                else {
                    ++halfMove;
                }
                
                current_player->update(another_player);
                another_player->update(current_player);
                if(checkmate(current_player, another_player)) {
                    std::string message(*current_player);
                    std::cout << message + " did checkmate.\n";

                    isRunning = false;
                    return;
                }
            }

            field.resetSquares();
            break;
        }
        case SDL_MOUSEMOTION: {
            current_player->handleEvents(&event);
            break;
        }
        default:
            break;
        }
    }
}

void Game::update() {

}

void Game::render() {
    SDL_RenderClear(renderer);

    field.render(renderer);
    white_player.render();
    black_player.render();

    SDL_RenderPresent(renderer);
}

int Game::exec() {
    const int fps = 30;
    const int ms_for_frame = 1000 / fps;

    while(isRunning) {
        handleEvents();
        update();
        render();
        
        SDL_Delay(ms_for_frame);
    }

   return 0;
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
