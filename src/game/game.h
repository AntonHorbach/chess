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

    int halfMove = 0;
    Player* current_player = &white_player;
    Player* another_player = &black_player;    

    bool isRunning;

    bool initMenu();

    void render();
    void handleEvents();
    void update();

    void handleFieldEvent(SDL_Event* event);

    bool checkmate();
    bool pat();

    void newGame();
    void surrender();
    void back();
    void exit();

public:
    Game();
    ~Game();

    bool init(const char* title, int x, int y, int width, int height, bool fullscreen);
    int exec();
};

#endif