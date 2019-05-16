#ifndef GAME
#define GAME

#include <SDL2/SDL.h>
#include <iostream>
#include <memory>

#include "field.h"
#include "player.h"

class Game {
    SDL_Window* window;
    SDL_Renderer* renderer;

    Field field;

    std::unique_ptr<Player> white_player;
    std::unique_ptr<Player> black_player;

    bool isRunning;

    void render();
    void handleEvents();
    void update();

public:
    Game();
    ~Game();

    bool init(const char* title, int x, int y, int width, int height, bool fullscrean);
    int exec();
};

#endif