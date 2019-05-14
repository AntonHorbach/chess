#include <SDL2/SDL.h>
#include <iostream>

#include "filed.h"

class Game {
    SDL_Window* window;
    SDL_Renderer* renderer;

    Field field;

    bool isRunning;

    void render();
    void handleEvents();
    void update();

public:
    Game();
    ~Game();

    bool init(char* title, int x, int y, int width, int height, bool fullscrean);
    int exec();
};