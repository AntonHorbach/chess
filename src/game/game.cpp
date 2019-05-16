#include "game.h"

Game::Game(): window(nullptr), renderer(nullptr),
    white_player(new Player(WHITE)), black_player(new Player(BLACK))
{}

bool Game::init(const char* title, int x, int y, int width, int height, bool fullscrean) {
    Uint32 flags = fullscrean ? (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN) : SDL_WINDOW_SHOWN;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
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

    if(!field.init(renderer) || !white_player->init(renderer)
     || !black_player->init(renderer))
    {
        return false;
    }

    isRunning = true;

    return true;
}

void Game::handleEvents() {
    SDL_Event event;

    SDL_PollEvent(&event);

    switch(event.type) {
        case SDL_EventType::SDL_QUIT:
            isRunning = false;
            break;
        case SDL_EventType::SDL_MOUSEBUTTONDOWN:
            field.handleEvents(&event);
            break;
        case SDL_EventType::SDL_MOUSEBUTTONUP:
            field.handleEvents(&event);
            break;
        default:
            break;
    }
}

void Game::update() {

}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255);
    SDL_RenderClear(renderer);

    field.render(renderer);
    white_player->render(renderer);
    black_player->render(renderer);

    SDL_RenderPresent(renderer);
}

int Game::exec() {
    while(isRunning) {
        handleEvents();
        update();
        render();
    }

   return 0;
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
