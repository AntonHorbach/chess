#include "game.h"

Game::Game(): window(nullptr), renderer(nullptr),
    white_player(WHITE), black_player(BLACK)
{}

bool Game::init(const char* title, int x, int y, int width, int height, bool fullscrean) {
    Uint32 flags = fullscrean ? (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN) : SDL_WINDOW_SHOWN;

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

void Game::handleEvents() {
    static int halfMove = 0;
    Player* current_player = (halfMove % 2 == 0) ? &white_player : &black_player;
    Player* another_player = (current_player == &white_player) ? &black_player : &white_player;

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
                else ++halfMove;
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
