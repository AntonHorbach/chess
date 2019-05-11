#include "game.h"

Game::Game(): window(nullptr), renderer(nullptr)
{}

int Game::init(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Bad sdl init: " << SDL_GetError() << '\n';
        return 1;
    }

    if(SDL_GetDisplayMode(0, 0, &mode) != 0) {
        std::cout << "Problems with diplay mode\n";
        return 1;
    }

    window = SDL_CreateWindow("Chess", 300, 150, mode.w, mode.h, SDL_WINDOW_SHOWN);
    if(window == nullptr) {
        std::cout << "The window has not been created: " << SDL_GetError() << '\n';
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if(renderer == nullptr) {
        std::cout << "Renderer has not been created: " << SDL_GetError() << '\n';
        return 1;
    }

    return 0;
}

int Game::exec() {
    if(init() != 0) {
        return 1;
    }

    std::cout << "ALL RIGHT!!!\n";
/*
    while(true) {
        //event loop
    }
    */

   return 0;
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
