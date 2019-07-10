#ifndef GAME
#define GAME

#include <SDL2/SDL.h>
#include <iostream>
#include <memory>

#include "field.h"
#include "player.h"
#include "../tools/tools.h"
#include "menu/menu.h"

class Game {
    SDL_Window* window;
    SDL_Renderer* renderer;

    Menu menu;
    Field field;

    Player white_player;
    Player black_player;

    bool isRunning;

    bool initMenu();

    void render();
    void handleEvents();
    void update();

    bool checkmate(Player* current_player, Player* another_player);
    bool pat(Player* current_player);

public:
    Game();
    ~Game();

    bool init(const char* title, int x, int y, int width, int height, bool fullscreen);
    int exec();
};

#endif