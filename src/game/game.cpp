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
    using namespace tools;

    const Figure* enemy_king = another_player->getFigure(FT::KING);
    const std::vector<MOVE> kingAvailableMoves = enemy_king->getAvailableMoves() +
                                                  enemy_king->getAvailableAttacks();
    int available_moves_count = kingAvailableMoves.size();
    std::vector<std::reference_wrapper<const Figure>> threats;
    bool direct_threat = false;

    if(available_moves_count != 0) {
        return false;
    }

    for(const auto& figure : current_player->getFigures()) {
        for(const auto& [x, y] : figure.getAvailableAttacks()) {
            if(figure.getX() + x == enemy_king->getX() && figure.getY() + y == enemy_king->getY())
            {
                direct_threat = true;
                threats.push_back(std::reference_wrapper<const Figure>(figure));
                break;
            }

            if(std::find(std::begin(kingAvailableMoves), std::end(kingAvailableMoves), MOVE{x, y})
                != std::end(kingAvailableMoves))
            {
                threats.push_back(std::reference_wrapper<const Figure>(figure));
                --available_moves_count;
            }
        }
    }

    size_t threats_count = threats.size();

    for(const Figure& threat : threats) {
        for(const Figure& figure : another_player->getFigures()) {
            if(std::find(std::begin(figure.getAvailableAttacks()), 
                        std::end(figure.getAvailableAttacks()), 
                        POS{threat.getX(), threat.getY()}) != std::end(figure.getAvailableAttacks()))
            {
                --threats_count;
                break;
            }
        }
    }

    if(threats_count != 0 && available_moves_count == 0) return true;

    return false;
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
                    ATTACK_SQUARE
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
